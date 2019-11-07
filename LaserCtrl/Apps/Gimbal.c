#include "Gimbal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "DR16.h"
#include "Servo.h"
#include "LED.h"
#include "Camera.h"

void Gimbal_Ctrl(void)
{
	static float rcYaw=0,rcPitch=0;/*遥控的值*/
	static uint16_t laserTim=0,laserFlag=0;/*激光3~5s控制*/
	static int16_t Xerr = 0, Yerr = 0;/*摄像头数据PID计算后的差值*/
	rcYaw=rcYaw>100?100:rcYaw<-100?-100:rcYaw - DR16.ch1*0.005;
	rcPitch=rcPitch>100?100:rcPitch<-100?-100:rcPitch+DR16.ch4*0.005;/*限幅*/
	
	if(DR16.switch_left==1)/*PID闭环模式*/
	{
		GPIO_WriteBit(LASER_GPIO_PORT,LASER_PIN,Bit_SET);/*PID闭环激光必须常亮*/
		
		Xerr=PID_Calc(&Laser.PIDx,Laser.X,Laser.TarX);
		Yerr=PID_Calc(&Laser.PIDy,Laser.Y,Laser.TarY);/*PID计算*/
		
		DXL1_setSyncMsg(USART_2,POSITION,2,0x01, 2715 + (int)rcYaw -  Xerr, 
			                                 0x02, 3030 + (int)rcPitch - Yerr);/*发送目标值*/
	}
	else if(DR16.switch_left==3)/*打靶模式*/
	{
			GPIO_WriteBit(LASER_GPIO_PORT,LASER_PIN,Bit_RESET);
		
			DXL1_setSyncMsg(USART_2,POSITION,2,0x01, 2715 + (int)rcYaw +(int)((-Laser.TarX+240)/3.8) , 
																			 0x02, 3030 + (int)rcPitch +(int)((-Laser.TarY+240)/3.8) );
			if(Laser.ON)
			{
				GPIO_WriteBit(LASER_GPIO_PORT,LASER_PIN,Bit_SET);
			}
			else 
			{
				GPIO_WriteBit(LASER_GPIO_PORT,LASER_PIN,Bit_RESET);

			}
			
			laserFlag=0;
			laserTim=0;
	}
	else/*校准模式*/
	{
		if(laserFlag==0)
		{
			laserTim++;
			if(laserTim<20)
			{
				GPIO_WriteBit(LASER_GPIO_PORT,LASER_PIN,Bit_RESET);
				rcYaw=0;
				rcPitch=0;
				
			}
			else if(laserTim<300)
			{
				GPIO_WriteBit(LASER_GPIO_PORT,LASER_PIN,Bit_SET);
			}
			else
			{
				GPIO_WriteBit(LASER_GPIO_PORT,LASER_PIN,Bit_RESET);
				laserFlag=1;
			}
		}
		else
		{
			laserTim=0;
		}		
		DXL1_setSyncMsg(USART_2,POSITION,2,0x01, 2715 + (int)rcYaw +(int)((-Laser.TarX+240)/3.8) , 
																			 0x02, 3030 + (int)rcPitch +(int)((-Laser.TarY+240)/3.8) );
	}
}

