/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Task_Ctrl.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-02-20               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Task_Ctrl.h" 
#include "Chassis.h"
#include "Joint.h"
#include "FreeRTOS.h"
#include "task.h"
#include "DR16.h"
#include "Servo.h"
#include "RoboModule.h"
#include "Gyro.h"
#include "LED.h"
#include "Camera.h"
#include "Curve.h"
#include "UsartHmi.h"
#include "IM1253B.h"
#include "Gimbal.h"
/*-------------------------- D E F I N E S -----------------------------------*/

static TaskHandle_t xHandleCtrlChassis = NULL;
static TaskHandle_t xHandleCtrlGimbal = NULL;
static TaskHandle_t xHandleCtrlJoint = NULL;


/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

static void vTaskCtrlChassis(void *pvParameters);
static void vTaskCtrlGimbal(void *pvParameters);
static void vTaskCtrlJoint(void *pvParameters);

/*------------------G L O B A L - F U N C T I O N S --------------------------*/
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:18
	* @brief   控制任务创建函数
	* @param   void
	* @retval  void
	*/
void ControlTaskCreate(void)
{
	xTaskCreate(vTaskCtrlChassis,            /* 任务函数  */   
						  "vTaskCtrlChassis",          /* 任务名    */ 
						  256,       			             /* 任务栈大小*/
						  NULL,                        /* 任务参数  */
					    2,       			               /* 任务优先级*/
						  &xHandleCtrlChassis);        /* 任务句柄  */ 
	xTaskCreate(vTaskCtrlGimbal,            
					    "vTaskCtrlGimbal",          
				    	256,       			   
				    	NULL,                 
					    2,       			   
					    &xHandleCtrlGimbal);
	xTaskCreate(vTaskCtrlJoint,            
					    "vTaskCtrlJoint",          
				    	1000,       			   
				    	NULL,                 
					    2,       			   
					    &xHandleCtrlJoint);
}

/*---------------------L O C A L - F U N C T I O N S--------------------------*/
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:18
	* @brief   底盘控制任务
	* @param   void
	* @retval  void
	*/
static void vTaskCtrlChassis(void *pvParameters)
{
	portTickType CurrentControlTick = 0;
	
	RoboModule_Init();/*RoboModule驱动初始化*/
  while(1)
	{
		
	//	Chassis_Ctrl();
		
	  vTaskDelayUntil(&CurrentControlTick, 10 / portTICK_RATE_MS);/*5ms延时*/
	}
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:18
	* @brief   云台控制任务
	* @param   void
	* @retval  void
	*/
static void vTaskCtrlGimbal(void *pvParameters)
{
	portTickType CurrentControlTick = 0;


	
	DXL1_setSyncMsg(USART_6,SPEED,2,0x00,50,0x01,50);/*设置舵机速度*/
	PID_StructInit(&Camera.xPID,POSITION_PID,200,100,0.1,0.01,0);
  while(1)
	{
		
    Gimbal_Ctrl();

		
#if 0		
		RcYaw=RcYaw>100?100:RcYaw<-100?-100:RcYaw - DR16.ch1*0.005;
		RcPitch=RcPitch>100?100:RcPitch<-100?-100:RcPitch+DR16.ch4*0.005;/*限幅*/
		
		if(DR16.switch_left==1)/*PID闭环模式*/
		{
			GPIO_WriteBit(LASER_GPIO_PORT,LASER_PIN,Bit_SET);/*PID闭环激光必须常亮*/
			
			Xerr=PID_Calc(&Laser.PIDx,Laser.X,Laser.TarX);
		  Yerr=PID_Calc(&Laser.PIDy,Laser.Y,Laser.TarY);/*PID计算*/
			
			DXL1_setSyncMsg(USART_2,POSITION,2,0x01, 2715 + (int)RcYaw -  Xerr, 
			                                   0x02, 3030 + (int)RcPitch - Yerr);
		}
		else if(DR16.switch_left==3)/*打靶模式*/
		{
			  GPIO_WriteBit(LASER_GPIO_PORT,LASER_PIN,Bit_RESET);
				DXL1_setSyncMsg(USART_2,POSITION,2,0x01, 2715 + (int)RcYaw +(int)((-Laser.TarX+240)/3.8) , 
			                                   0x02, 3030 + (int)RcPitch +(int)((-Laser.TarY+240)/3.8) );
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
					RcYaw=0;
					RcPitch=0;
					
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
		  DXL1_setSyncMsg(USART_2,POSITION,2,0x01, 2715 + (int)RcYaw +(int)((-Laser.TarX+240)/3.8) , 
			                                   0x02, 3030 + (int)RcPitch +(int)((-Laser.TarY+240)/3.8) );
		}
#endif		
   vTaskDelay(20);
	}
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:18
	* @brief   关节控制任务
	* @param   void
	* @retval  void
	*/
static void vTaskCtrlJoint(void *pvParameters)
{
//	portTickType CurrentControlTick = 0;
//	vTaskDelay(500);
//	GY955.targetYaw = GY955.Yaw;
//	GY955.targetPitch = GY955.Pitch;
//	GY955.targetRoll = GY955.Roll;
////	DXL1_setSyncMsg(USART_6,SPEED,2,0x01,600,0x02,600);

//	vTaskDelay(100);
//	
////			DXL1_setSyncMsg(USART_6,0x04,1,0x0B,0x01);
//	DXL1_setSyncMsg(USART_6,SPEED,2,0x0D,100,0x0E,200);
//	vTaskDelay(100);
//	DXL1_setSyncMsg(USART_6,SPEED,12,0x01,100,0x02,100,0x03,100,0x04,100
//	                                ,0x05,100,0x06,100,0x07,100,0x08,100
//	                                ,0x09,100,0x0A,100,0x0B,100,0x0C,100);
//	vTaskDelay(100);
//	Joint_NewTrotMotionModel(0, 0, 0, 0);
//	vTaskDelay(1000);
//	Joint_RobotArmCtrl(3180,2100);
//	vTaskDelay(100);
//	DXL1_setSyncMsg(USART_6,SPEED,12,0x01,0,0x02,0,0x03,0,0x04,0
//	                                ,0x05,0,0x06,0,0x07,0,0x08,0
//	                                ,0x09,0,0x0A,0,0x0B,0,0x0C,0);
	vTaskDelay(100);
  while(1)
	{

#if 0
		Joint.Vspin = PID_Calc(&Joint.PID_Spin, GY955.Yaw, GY955.targetYaw);
//		

//    if(DR16.switch_left == 2)
//		{
//		  Joint_RobotArmCtrl(1900,1024);
//		}
//		else
//		{
//		  Joint_RobotArmCtrl(890,2068);
//		}
		if(DR16.switch_right == 3)
		{
			
			Joint_NewTrotMotionModel(0, 20, 0, 0.25f);

		}
		else if(DR16.switch_right == 2)
		{
//			DXL1_setSyncMsg(USART_6,SPEED,12,0x01,100,0x02,100,0x03,100,
//																	0x04,100,0x05,100,0x06,100,
//																	0x07,100,0x08,100,0x09,100,
//																	0x0A,100,0x0B,100,0x0C,100);
		//Joint_NewTrotMotionModel(-DR16.ch3/2, DR16.ch2/2, DR16.ch1/2, 0.25f);

		Joint_StateMachine();

		}	
		else if(DR16.switch_right == 1)
    {
			
		  Joint_NewTrotMotionModel(-DR16.ch3/2,  DR16.ch2/2, DR16.ch1/2, 0.25f);
		}
#endif

		vTaskDelay(10);
		
	 // vTaskDelayUntil(&CurrentControlTick, 10 / portTICK_RATE_MS);/*5ms延时*/
	}
}
/*-----------------------------------FILE OF END------------------------------*/

