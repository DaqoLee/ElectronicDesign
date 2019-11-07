/**
  ************************************* Copyright ****************************** 
  *
  * (C) Copyright 2018,Daqo Lee,China, GCU.
  *		All Rights Reserved
  *                              
  * By(ZHE WOLF TEAM OF GCU ROBOT)
  * https://github.com/GCUWildwolfteam
  *    
  * FileName   : User.c   
  * Version    : v1.0		
  * Author     : Daqo Lee			
  * Date       : 2018-10-23         
  * Description:    
  * Function List:  
  	1. ....
  	   <version>: 		
  <modify staff>:
  		  <data>:
   <description>:  
  	2. ...
  ******************************************************************************
 */

#include "User.h"
uint8_t Ultrasonic_Message[20];
char RFID_RC522[8];
uint8_t Esp8266[14];
uint8_t Esp8266_Flag=0;
pid_t TextPID;	
Ultrasonic_Struct Ultrasonic[8];

 /*
  * @brief 电机的相关参数初始化
  * @param None
  * @retval None
  */
void Motor_Param_Init(void)
{
/********************* 底盘 M3508 电机参数初始化 *******************************/	
	ChassisParam.TargetVX=0;
	ChassisParam.TargetVY=0;
	
	ChassisParam.LF.KP=2.5f;
	ChassisParam.LF.KI=1.0f;
	ChassisParam.LF.KD=0;
	ChassisParam.LF.Target_Speed=0;
	ChassisParam.LF.IntegralLimit=800;
	PID_struct_init(&ChassisParam.LF.PID,DELTA_PID,M3508_Xianfu,ChassisParam.LF.IntegralLimit,\
									ChassisParam.LF.KP,ChassisParam.LF.KI,ChassisParam.LF.KD);
	
	ChassisParam.LB.KP=2.5f;
	ChassisParam.LB.KI=1.0f;
	ChassisParam.LB.KD=0;
	ChassisParam.LB.Target_Speed=0;
	ChassisParam.LB.IntegralLimit=800;
	PID_struct_init(&ChassisParam.LB.PID,DELTA_PID,M3508_Xianfu,ChassisParam.LB.IntegralLimit,\
									ChassisParam.LB.KP,ChassisParam.LB.KI,ChassisParam.LB.KD);
	
	ChassisParam.RF.KP=2.5f;
	ChassisParam.RF.KI=1.0f;
	ChassisParam.RF.KD=0;
	ChassisParam.RF.Target_Speed=0;
	ChassisParam.RF.IntegralLimit=800;
	PID_struct_init(&ChassisParam.RF.PID,DELTA_PID,M3508_Xianfu,ChassisParam.RF.IntegralLimit,\
									ChassisParam.RF.KP,ChassisParam.RF.KI,ChassisParam.RF.KD);
	
	ChassisParam.RB.KP=2.5f;
	ChassisParam.RB.KI=1.0f;
	ChassisParam.RB.KD=0;
	ChassisParam.RB.Target_Speed=0;
	ChassisParam.RB.IntegralLimit=800;
	PID_struct_init(&ChassisParam.RB.PID,DELTA_PID,M3508_Xianfu,ChassisParam.RB.IntegralLimit,\
									ChassisParam.RB.KP,ChassisParam.RB.KI,ChassisParam.RB.KD);
									
	PID_struct_init(&ChassisParam.Chassis_Gyro.Chassis_PID,POSITION_PID,4000,1000,3.5f,0,0);
/********************* 云台 M6623 电机参数初始化 *******************************/
  
	CloudParam.Pitch.KP=6.5;
	CloudParam.Pitch.KI=0;
	CloudParam.Pitch.KD=2;
	CloudParam.Pitch.IntegralLimit=2000;
	CloudParam.Pitch.Target_Angle=First_Pitch_Angle;
	PID_struct_init(&CloudParam.Pitch.PID.Out,POSITION_PID,M6623_Xianfu,CloudParam.Pitch.IntegralLimit,\
										  CloudParam.Pitch.KP,CloudParam.Pitch.KI,CloudParam.Pitch.KD);
	PID_struct_init(&CloudParam.Pitch.PID.In,POSITION_PID,6000,2000,0.8f,0,1);
	
	CloudParam.Yaw.KP=3.6;
	CloudParam.Yaw.KI=0;
	CloudParam.Yaw.KD=4.6;
	CloudParam.Yaw.IntegralLimit=1000;
	CloudParam.Yaw.Target_Angle=First_Yaw_Angle;
	PID_struct_init(&CloudParam.Yaw.PID.Out,POSITION_PID,M6623_Xianfu,CloudParam.Yaw.IntegralLimit,\
										    CloudParam.Yaw.KP,CloudParam.Yaw.KI,CloudParam.Yaw.KD);
	PID_struct_init(&CloudParam.Yaw.PID.In,POSITION_PID,6000,1000,1.2,0.1,2);
	

	PID_struct_init(&CloudParam.Cloud_Gyro.Pitch_PID.Out,POSITION_PID,6000,500,100,0,1000);
	PID_struct_init(&CloudParam.Cloud_Gyro.Pitch_PID.In,POSITION_PID,6000,1000,1.2,0.1,2);
	
	PID_struct_init(&CloudParam.Cloud_Gyro.Yaw_PID.Out,POSITION_PID,6000,500,120,0,100);
  PID_struct_init(&CloudParam.Cloud_Gyro.Yaw_PID.In ,POSITION_PID,6000,2000,0.8,0,1);

  PID_struct_init(&Auto_Pitch_PID,POSITION_PID,6000,500,1,0,0);
	PID_struct_init(&Auto_Yaw_PID,POSITION_PID,6000,500,1,0,0);

/********************* 拨弹 M2006 电机参数初始化 *******************************/	
	M2006.Last_Angle=M2006.Real_Angle;	
	
	M2006.Angle_KP=0.8f;
	M2006.Angle_KI=0;
	M2006.Angle_KD=2.8f;
	PID_struct_init(&M2006.PID.Out,POSITION_PID,M2006_Xianfu,M2006.IntegralLimit,\
										M2006.Angle_KP,M2006.Angle_KI,M2006.Angle_KD);
	
	M2006.Speed_KP=0.4f;
	M2006.Speed_KI=0.016f;
	M2006.Speed_KD=0;
	PID_struct_init(&M2006.PID.In,DELTA_PID,M2006_Xianfu,M2006.IntegralLimit,\
										M2006.Speed_KP,M2006.Speed_KI,M2006.Speed_KD);
	
	
	PID_struct_init(&TextPID,POSITION_PID,2500,500,10,0,5);
	
	PID_struct_init(&SRP.X_PID,POSITION_PID,1000,500,5,0,0);
	PID_struct_init(&SRP.Y_PID,POSITION_PID,1000,500,5,0,0);
	PID_struct_init(&SRP.Om_PID,POSITION_PID,2000,500,0,0,0);
	
	M2006.IntegralLimit=120;
	M2006.Target_Angle =0;
	M2006.Target_Speed =1500;
	M2006.All_error_Angle=0;
	M2006.Error_Angle=0;
	
	Set_moto_current(&hcan1,0x200, 0, 0, 0, 0);//电机ID 0x201-0x204
	Set_moto_current(&hcan1,0x1FF, 0, 0, 0, 0);//电机ID 0x205-0x208	
	
    HAL_Delay(100);
    CAN_RoboModule_DRV_Reset(0,1);                      //对0组1号驱动器进行复位
	CAN_RoboModule_DRV_Reset(0,2);                      //对0组1号驱动器进行复位
	CAN_RoboModule_DRV_Reset(0,3);                      //对0组1号驱动器进行复位
	CAN_RoboModule_DRV_Reset(0,4);                      //对0组1号驱动器进行复位
    HAL_Delay(1000);                                     //发送复位指令后的延时必须要有，等待驱动器复位完毕。
    CAN_RoboModule_DRV_Config(0,1,10,0);               //配置为1s传回一次数据
	CAN_RoboModule_DRV_Config(0,2,10,0);               //配置为1s传回一次数据
	CAN_RoboModule_DRV_Config(0,3,10,0);               //配置为1s传回一次数据
	CAN_RoboModule_DRV_Config(0,4,10,0);               //配置为1s传回一次数据
    HAL_Delay(1);
	
    CAN_RoboModule_DRV_Mode_Choice(0,1,Velocity_Mode);  //选择进入开环模式
	CAN_RoboModule_DRV_Mode_Choice(0,2,Velocity_Mode);  //选择进入开环模式
	CAN_RoboModule_DRV_Mode_Choice(0,3,Velocity_Mode);  //选择进入开环模式
	CAN_RoboModule_DRV_Mode_Choice(0,4,Velocity_Mode);  //选择进入开环模式
    HAL_Delay(500);  
	
//	CAN_RoboModule_DRV_Velocity_Mode(0,1,1000,500);
//	CAN_RoboModule_DRV_Velocity_Mode(0,2,1000,500);
//	CAN_RoboModule_DRV_Velocity_Mode(0,3,1000,500);
//	CAN_RoboModule_DRV_Velocity_Mode(0,4,1000,500);
}

void Bell_Init(void)
{	
	uint8_t i=1;
	TIM12->CCR1=200;
	for(;i<4;i++)
	{
		TIM12->ARR=1999-200*i;
		HAL_Delay(150);
	}
	TIM12->ARR=0;
}

void text(void)
{
	static float Target=1200;
	Target=ABS(DBUS_ReceiveData.ch2)>20?Target+DBUS_ReceiveData.ch2*0.05f:Target;
	Target=Target>1500?1500:Target;
	Target=Target<900?900:Target;
	Set_moto_current(&hcan1,0x200, -pid_calc(&TextPID,WDD35D4[0],Target), 0, 0, 0);//电机ID 0x201-0x204
}

void Analysis(void)
{
	static uint8_t n=0;
	for(uint8_t i=0;i<20;)
	{
		if(Ultrasonic_Message[i]==0xAA&&Ultrasonic_Message[i+1]==0xAA&&Ultrasonic_Message[i+18]==0xAA&&Ultrasonic_Message[i+19]==0xAA)
		{
			Ultrasonic[0].Distance_Str[n]=Ultrasonic_Message[i+2]<<8|Ultrasonic_Message[i+3];
			Ultrasonic[0].Distance+=Ultrasonic[0].Distance_Str[n];
			
			Ultrasonic[1].Distance_Str[n]=Ultrasonic_Message[i+4]<<8|Ultrasonic_Message[i+5];
			Ultrasonic[1].Distance+=Ultrasonic[1].Distance_Str[n];
			
			Ultrasonic[2].Distance_Str[n]=Ultrasonic_Message[i+6]<<8|Ultrasonic_Message[i+7];
			Ultrasonic[2].Distance+=Ultrasonic[2].Distance_Str[n];
			
			Ultrasonic[3].Distance_Str[n]=Ultrasonic_Message[i+8]<<8|Ultrasonic_Message[i+9];
			Ultrasonic[3].Distance+=Ultrasonic[3].Distance_Str[n];
			
			Ultrasonic[4].Distance_Str[n]=Ultrasonic_Message[i+10]<<8|Ultrasonic_Message[i+11];
			Ultrasonic[4].Distance+=Ultrasonic[4].Distance_Str[n];
			
			Ultrasonic[5].Distance_Str[n]=Ultrasonic_Message[i+12]<<8|Ultrasonic_Message[i+13];
			Ultrasonic[5].Distance+=Ultrasonic[5].Distance_Str[n];
			
			Ultrasonic[6].Distance_Str[n]=Ultrasonic_Message[i+14]<<8|Ultrasonic_Message[i+15];
			Ultrasonic[6].Distance+=Ultrasonic[6].Distance_Str[n];
			
			Ultrasonic[7].Distance_Str[n]=Ultrasonic_Message[i+16]<<8|Ultrasonic_Message[i+17];
			Ultrasonic[7].Distance+=Ultrasonic[7].Distance_Str[n];
			
			n++;
			
			if(n==5)
			{
				for(uint8_t j=0;j<8;j++)
				{
					Ultrasonic[j].Mean_Distance=Ultrasonic[j].Distance/5;
					Ultrasonic[j].Distance=0;
					if(Ultrasonic[j].Mean_Distance<150)
						Ultrasonic[j].Stop_Flag=1;
					else
						Ultrasonic[j].Stop_Flag=0;
				}
				n=0;
			}
			
			return;
		}
		else if(Ultrasonic_Message[i]==0xFF&&Ultrasonic_Message[i+1]==0xFF&&Ultrasonic_Message[i+10]==0xFF&&Ultrasonic_Message[i+11]==0xFF)
		{
			RFID_RC522[0]=Ultrasonic_Message[2];
			RFID_RC522[1]=Ultrasonic_Message[3];
			RFID_RC522[2]=Ultrasonic_Message[4];
			RFID_RC522[3]=Ultrasonic_Message[5];
			RFID_RC522[4]=Ultrasonic_Message[6];
			RFID_RC522[5]=Ultrasonic_Message[7];
			RFID_RC522[6]=Ultrasonic_Message[8];
			RFID_RC522[7]=Ultrasonic_Message[9];
			return;
		}
		else
			i++;
	}
}

void Esp8266_init(void)
{
	printf("AT+CWMODE=1\r\n");
	HAL_Delay(500);
	printf("AT+RST\r\n");
	HAL_Delay(500);
	printf("AT+CIPMUX=1\r\n");
	HAL_Delay(500);
	printf("AT+CIPSERVER=1,8230\r\n");
}

void Esp8266_Analysis(void)
{
	for(uint8_t i=0;i<12;)
	{
		if(Esp8266[i+2]=='+'&&Esp8266[i+12]=='&')
		{
			Esp8266_Flag=Esp8266[i+11];
			return;
		}
		else
			i++;
	}
}


