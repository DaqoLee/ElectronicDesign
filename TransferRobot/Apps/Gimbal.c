#include "Gimbal.h"
#include "Curve.h"
#include "UsartHmi.h"
#include "IM1253B.h"
#include "DR16.h"
#include "Camera.h" 
#include "Servo.h"
#include "Curve.h"
#define YawMid 2048
#define PitchMid 2048

uint8_t ShootFlag=0;
float RcYaw=0,RcPitch=0;/*遥控的值*/

void Gimbal_Ctrl(void)
{
	static uint8_t flag=0, ShootFlag=0,runFlag=0;;
  static float sinTemp=0.0f,shootTemp=0.0f;
	static	int16_t xErr = 0;/*摄像头数据PID计算后的差值*/
	

	
	
	RcYaw=RcYaw>1000?1000:RcYaw<-1000?-1000:RcYaw - DR16.ch1*0.005;
	RcPitch=RcPitch>1000?1000:RcPitch<-1000?-1000:RcPitch+DR16.ch4*0.005;/*限幅*/	


//	if(DR16.switch_right==1)
	{
		
	  if(UsartHmi.num==0)
	 {
		
		 if(UsartHmi.reset)
		 {
		    Camera.shoot=0;
			  RcYaw=0;
        RcPitch=0;
		 }

			if(UsartHmi.flag)
			{
			  Camera.shoot=1;
				
				if(UsartHmi.d>=200&&UsartHmi.d<=300)
				{
					RcPitch=(int)get_alpha(UsartHmi.d)*4096/360 ;
				}
				else
			 {
				 RcPitch=0;
			 }			
				if(ABS(UsartHmi.a)<=30)
				{
						RcYaw=UsartHmi.a*4096/360 ;
				}
				else
				{
					RcYaw=0;
				
				}
			}
			
			if(DR16.switch_right==1)
				Camera.shoot=1;
			
			DXL1_setSyncMsg(USART_6,POSITION,2,0x00, YawMid + (int)RcYaw , 
														 0x01, PitchMid + (int)RcPitch );
//			else
//      {
//				Camera.shoot=0;
//			}

		}
		else if(UsartHmi.num==1)
		{
			 if(UsartHmi.reset)
			 {
				 runFlag=0;
				 Camera.shoot=0;
			 }
			switch(runFlag)
			{
				case 0:
						RcPitch=0;
						DXL1_setSyncMsg(USART_6,POSITION,2,0x00, YawMid , 
													 0x01, PitchMid);
						if(UsartHmi.flag)
						{
							runFlag=1;
						}
					break;
				case 1:
				  	sinTemp = (sinTemp>2*PI)?0:(sinTemp+0.02);

						if(Camera.flag==1&&ABS(Camera.err)<=5)
						{
							Camera.shoot=1;
							
							if(Camera.d>=180&&Camera.d<=320)
							{
									RcPitch=(int)get_alpha(Camera.d-25)*4096/360 ;
								
									DXL1_setSyncMsg(USART_6,POSITION,1, 0x01, PitchMid + (int)RcPitch );
							}
							vTaskDelay(5500);
							runFlag=0;
							sinTemp=0;
						
						}
						else
						{
							RcYaw=(int)Curve_Sin(340,1,0,0,sinTemp);
				  		DXL1_setSyncMsg(USART_6,POSITION,1,0x00, YawMid + (int)RcYaw );
				
						}
							
					break;
				default:
					break;
			
			}
		
		}
		
		else if(UsartHmi.num==2)
		{
			 if(UsartHmi.reset)
			 {
				 runFlag=0;
				 Camera.shoot=0;
				// ShootFlag=0;
			 }
			switch(runFlag)
			{
				case 0:

						RcPitch=0;
						DXL1_setSyncMsg(USART_6,POSITION,2,0x00, YawMid ,  0x01, PitchMid);
						if(UsartHmi.flag)
						{
							runFlag=1;
							ShootFlag=0;
						}
							
					break;
				case 1:
						sinTemp = (sinTemp>2*PI)?0:(sinTemp+0.01);
						RcYaw=(int)Curve_Sin(340,1,0,0,sinTemp);
				    RcPitch=(int)get_alpha(218)*4096/360 ;
				
				  	DXL1_setSyncMsg(USART_6,POSITION,2,0x00, YawMid + (int)RcYaw , 
													       0x01, PitchMid + (int)RcPitch );
						
						if(Camera.flag==1&&ABS(Camera.err)<=60&&ShootFlag==0)
						{
							Camera.shoot=1;
							ShootFlag=1;
							
						}
						if(UsartHmi.flag)
						{
							ShootFlag=0;
						
						}
					break;
				
				
				default:
					break;
			
			}
		}
			
		else if(UsartHmi.num==3)
		{
			 if(UsartHmi.reset)
			 {
				 runFlag=0;
				 Camera.shoot=0;
				// ShootFlag=0;
			 }
			switch(runFlag)
			{
				case 0:

						RcPitch=0;
						DXL1_setSyncMsg(USART_6,POSITION,2,0x00, YawMid , 0x01, PitchMid);
						if(UsartHmi.flag)
						{
							runFlag=1;
						//	ShootFlag=0;
						}
							
					break;
				case 1:
						sinTemp = (sinTemp>2*PI)?0:(sinTemp+0.01);
						RcYaw=(int)Curve_Sin(340,1,0,0,sinTemp);
				   
						DXL1_setSyncMsg(USART_6,POSITION,1,0x00, YawMid + (int)RcYaw  );
						if(Camera.flag==1&&ABS(Camera.err)<=60&&ShootFlag==0)
						{
							Camera.shoot=1;
							ShootFlag=1;
							
							RcPitch=(int)get_alpha(Camera.d-25)*4096/360 ;
							
				    	DXL1_setSyncMsg(USART_6,POSITION,1, 0x01, PitchMid + (int)RcPitch );
							
						}
						if(UsartHmi.flag)
						{
							ShootFlag=0;
						
						}
					break;
				
				
				default:
					break;
			
			}
		
		}
		

	}
	
	

}