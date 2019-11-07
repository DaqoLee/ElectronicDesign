/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Joint.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-09               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Joint.h"
#include "delay.h"
#include "Motor.h"
#include "Servo.h"
#include "DR16.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Curve.h"
#include "Filter.h" 
#include "Gyro.h"
#include "Camera.h"
#include "Photoelectric.h"
#include "Key.h"
/*-------------------------- D E F I N E S -----------------------------------*/
Joint_t Joint;
uint8_t RunFlag=0;
float LH=0,RH=0;
/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

static int16_t Joint_getThighTarAng(int16_t TarAng ,float Phase,float Temp,float coe);
static int16_t Joint_getCrusTarAng(int16_t TarAng ,float Phase,float Temp,float coe);
//static int16_t Joint_getRopeCrusTarAng(int16_t TarAng ,float Phase,float Temp,float coe);
static int16_t Joint_getHipTarAng(int16_t TarAng ,float Phase,float Temp);
static void Joint_walkModel(int16_t Vx, int16_t Vy, int16_t Omega,float T);
static void Joint_UpslopeModel(int16_t Vx, int16_t Vy, int16_t Omega,float T);
static void Joint_RopeModel(int16_t Vx, int16_t Vy, int16_t Omega,float T);
static void Joint_RopeModelF(int16_t Vx, int16_t Vy, int16_t Omega,float T);
static void Joint_RopeModelH(int16_t Vx, int16_t Vy, int16_t Omega,float T);
static void Joint_SandDuneModel(int16_t Vx, int16_t Vy, int16_t Omega,float T);
/*------------------G L O B A L - F U N C T I O N S --------------------------*/
	/**
	* @Data    2019-01-08 14:49
	* @brief   关节初始化
	* @param   void
	* @retval  void
	*/
void Joint_Init(void)
{
	Joint.PID_Spin.maxInput=360;
#if 0
	DigitalServo.MX_64[0].MiddleAngle = 2600;
	DigitalServo.MX_64[1].MiddleAngle = 2500;
	DigitalServo.MX_64[2].MiddleAngle = 2748;
	DigitalServo.MX_64[3].MiddleAngle = 2548;
	DigitalServo.MX_64[4].MiddleAngle = 2600;
	DigitalServo.MX_64[5].MiddleAngle = 2400;
	DigitalServo.MX_64[6].MiddleAngle = 2600;
	DigitalServo.MX_64[7].MiddleAngle = 2648;
	DigitalServo.MX_64[8].MiddleAngle = 2324;
	DigitalServo.MX_64[9].MiddleAngle = 1766;
	DigitalServo.MX_64[10].MiddleAngle = 2324;
	DigitalServo.MX_64[11].MiddleAngle = 1766;
#endif
#if 1
	//大腿
	DigitalServo.MX_64[0].MiddleAngle = 2048;
	DigitalServo.MX_64[2].MiddleAngle = 2048;
	DigitalServo.MX_64[4].MiddleAngle = 2048;
	DigitalServo.MX_64[6].MiddleAngle = 2048;	
	
	//小腿
	DigitalServo.MX_64[1].MiddleAngle = 2048;
	DigitalServo.MX_64[3].MiddleAngle = 2048;
	DigitalServo.MX_64[5].MiddleAngle = 2100;
	DigitalServo.MX_64[7].MiddleAngle = 2700;
	
//	DigitalServo.MX_64[4].MiddleAngle = 1348;
//	DigitalServo.MX_64[5].MiddleAngle = 2620;
//	DigitalServo.MX_64[6].MiddleAngle = 1348;
//	DigitalServo.MX_64[7].MiddleAngle = 2848;

	DigitalServo.MX_64[8].MiddleAngle = 2068;
	DigitalServo.MX_64[9].MiddleAngle = 2048;
	DigitalServo.MX_64[10].MiddleAngle = 2048;
	DigitalServo.MX_64[11].MiddleAngle = 2048;
	
	DigitalServo.MX_64[12].MiddleAngle = 3180;//2150
	DigitalServo.MX_64[13].MiddleAngle = 2100;//3050
	
	
#endif
	
	PID_StructInit(&Joint.PID_X, POSITION_PID, 800, 500, 2, 0, 0);
	PID_StructInit(&Joint.PID_Y, POSITION_PID, 800, 500, 2, 0, 0);
	PID_StructInit(&Joint.PID_Z, POSITION_PID, 300, 200, -20, 0, 0);
	PID_StructInit(&Joint.PID_P, POSITION_PID, 500, 200, 20, 0, 0.01);
	PID_StructInit(&Joint.PID_Spin, POSITION_PID, 100, 100, 4, 0, 0);
}
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 14:49
	* @brief   关节电机控制
	* @param   void
	* @retval  void
	*/
void Thigh_M6020Ctrl(void)
{
	Motor.p_M6020setTarAngle(0,-DR16.ch1);
	Motor.p_M6020setTarAngle(1,-DR16.ch1);
	Motor.p_M6020setTarAngle(2,-DR16.ch1);
	Motor.p_M6020setTarAngle(3,-DR16.ch1);
	
	for(uint8_t i=0;i<4;i++)
	{
	  Motor.M6020[i].targetCurrent=PID_Calc(&Motor.M6020[i].OutPID, \
	  Motor.M6020[i].realAngle, Motor.M6020[i].targetAngle);
	}
	Motor.p_M6020setCur(CAN_1);  
}


/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-09 11:33
	* @brief   对角步态关节运动模型
	* @param   void
	* @retval  void
	*/
void Joint_TrotMotionModel(int16_t Vx, int16_t Vy, int16_t Omega)
{	
	static float temp=0;
	static int16_t  thighErr=-100,crusErr=300,hipErr=50;
	float thigh_anglecoe = 5,crus_anglecoe = 5;
	
	if(Vx == 0 && Vy == 0 && Omega == 0)
		temp=0;
	else
	  temp=temp>2*PI?0:temp+0.07f;

	Joint.Vz = PID_Calc(&Joint.PID_Z,GY955.Roll,GY955.targetRoll);
	DXL1_setSyncMsg(USART_6,POSITION,12,
	0x01,DigitalServo.MX_64[0].MiddleAngle +thighErr + Joint.Vz 
	- Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), /*LH*/               
	0x02,DigitalServo.MX_64[1].MiddleAngle +crusErr + Joint.Vz 
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega),0.25f,temp,crus_anglecoe),
	
	0x03,DigitalServo.MX_64[2].MiddleAngle +thighErr + Joint.Vz  
	- Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), /*RH*/ 
	0x04,DigitalServo.MX_64[3].MiddleAngle +crusErr + Joint.Vz 
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega),1.25f,temp,crus_anglecoe),
	
	0x05,DigitalServo.MX_64[4].MiddleAngle +thighErr- 0*Joint.Vz 
	+ Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), /*RF*/ 
	0x06,DigitalServo.MX_64[5].MiddleAngle +crusErr + 0*Joint.Vz 
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega),0.25f,temp,crus_anglecoe),

	0x07,DigitalServo.MX_64[6].MiddleAngle +thighErr- 0*Joint.Vz 
	+ Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), /*LF*/ 
	0x08,DigitalServo.MX_64[7].MiddleAngle +crusErr + 0* Joint.Vz 
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega),1.25f,temp,crus_anglecoe),
	
	0x09,DigitalServo.MX_64[8].MiddleAngle + hipErr + Joint_getThighTarAng(Omega + Vx,0,temp,thigh_anglecoe),
	0x0A,DigitalServo.MX_64[9].MiddleAngle - hipErr + Joint_getThighTarAng(Omega + Vx,1.0f,temp,thigh_anglecoe),
	0x0B,DigitalServo.MX_64[10].MiddleAngle + hipErr + Joint_getThighTarAng(Omega - Vx,0,temp,thigh_anglecoe),
	0x0C,DigitalServo.MX_64[11].MiddleAngle - hipErr + Joint_getThighTarAng(Omega - Vx,1.0f,temp,thigh_anglecoe));

//	vTaskDelay(5);	
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-09 11:33
	* @brief   三角步态关节运动模型
	* @param   void
	* @retval  void
	*/
void Joint_WalkMotionModel(int16_t Vx, int16_t Vy, int16_t Omega)
{	
	static float temp=0;
  float thigh_anglecoe = 5,crus_anglecoe = 5;

	if(Vx == 0 && Vy == 0 && Omega == 0)
		temp=0;
	else
	  temp=temp>2*PI?0:temp+0.04f;
	
	DXL1_setSyncMsg(USART_6,POSITION,12,
	                0x01,DigitalServo.MX_64[0].MiddleAngle 
                  - Joint_getThighTarAng(Vy,-0.5f,temp,thigh_anglecoe), /*LF左前*/               
						      0x02,DigitalServo.MX_64[1].MiddleAngle 
                  + Joint_getCrusTarAng(Vy + Vx + Omega,-0.25f,temp,crus_anglecoe),
									
									0x03,DigitalServo.MX_64[2].MiddleAngle 
                  - Joint_getThighTarAng(Vy,-1.5f,temp,thigh_anglecoe), /*RH*/ 
									0x04,DigitalServo.MX_64[3].MiddleAngle 
                  + Joint_getCrusTarAng(Vy + Vx + Omega,-1.25f,temp,crus_anglecoe),
								
                  0x05,DigitalServo.MX_64[4].MiddleAngle 
                  + Joint_getThighTarAng(Vy,-1.0,temp,thigh_anglecoe), /*RH*/ 
									0x06,DigitalServo.MX_64[5].MiddleAngle 
                  + Joint_getCrusTarAng(Vy + Vx + Omega,-0.75f,temp,crus_anglecoe),
								
									0x07,DigitalServo.MX_64[6].MiddleAngle 
                  + Joint_getThighTarAng(Vy,0.0f,temp,thigh_anglecoe), /*LF*/ 
									0x08,DigitalServo.MX_64[7].MiddleAngle 
                  + Joint_getCrusTarAng(Vy + Vx + Omega,0.25f,temp,crus_anglecoe),
									
                  0x09,DigitalServo.MX_64[8].MiddleAngle 
                  + Joint_getThighTarAng(Omega + Vx,0,temp,thigh_anglecoe),
									0x0A,DigitalServo.MX_64[9].MiddleAngle 
                  + Joint_getThighTarAng(Omega + Vx,1.0f,temp,thigh_anglecoe),
									0x0B,DigitalServo.MX_64[10].MiddleAngle 
                  + Joint_getThighTarAng(Omega - Vx,0,temp,thigh_anglecoe),
									0x0C,DigitalServo.MX_64[11].MiddleAngle 
                  + Joint_getThighTarAng(Omega - Vx,1.0f,temp,thigh_anglecoe));
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-09 11:33
	* @brief   对角步态关节运动模型
	* @param   void
	* @retval  void
	*/
void Joint_NewTrotMotionModel(int16_t Vx, int16_t Vy, int16_t Omega,float T)
{	
	static float temp=0;
//	static int16_t  thighErr=-400,crusErr=-800,hipErr=-50,crusErr1=0,crusErr2=0,thighErr1=0;
	if(Vx == 0 && Vy == 0 && Omega == 0)
		temp=0;
	else
	  temp = (temp>2*PI)?0:(temp+T);
	
#if 0
 	PhotoelectricScan();/*光电开关扫描*/	
	if(!PhoFlg.RF||!PhoFlg.LF)
	{
 		flag=1;
	}
	else
	{
	
	}
	if(!PhoFlg.RH||!PhoFlg.LH)
	{
	  flag1=1;
	}
	else
	{
	  flag1=0;
	}
//	if(DR16.switch_left==1)
//	{
//	  thighErr=-300;
//		crusErr=-900;
//		crusErr1=200;
//		hipErr=-50;
//	}
	if(flag==0)
	{
		thighErr=-300;
		crusErr=-800;
		crusErr1=0;
		crusErr2=0;
		thighErr1=0;
		hipErr=-100;
	  
	}
	else if(flag==1&&flag1==0)
	{
		crusErr1=crusErr1<=-300?-300:crusErr1-10;
	//	crusErr2=crusErr2>=200?200:crusErr2+10;
		
#if 0
		if(GY955.Roll<-8)
		{
			flag1=1;
		}
		
		if(flag1==0)
		{
			crusErr1=crusErr1<=-300?-300:crusErr1-10;
			//thighErr1=thighErr1>=200?200:thighErr1+10;
			Joint.Vz =0;// PID_Calc(&Joint.PID_Z,GY955.Roll,GY955.targetRoll);
    	Joint.Vp = 0;//PID_Calc(&Joint.PID_P,GY955.Pitch,GY955.targetPitch);
		}
		else if(flag1==1&&GY955.Roll>-5)
		{
			crusErr1=crusErr1>=50?50:crusErr1+10;
			//thighErr1=thighErr1<=0?0:thighErr1-10;
//			flag=0;
//			flag1=0;
		}
#endif
	}
	else if(flag1==1)
	{
	  crusErr2=crusErr2<=-300?-300:crusErr2-10;
		//crusErr1=crusErr1>=200?200:crusErr1+10;
	}
	else
	{
	  crusErr1=crusErr1>=200?200:crusErr1+10;
		crusErr2=crusErr2>=200?200:crusErr2+10;
	}
	
	
#endif

//	Joint.Vz = PID_Calc(&Joint.PID_Z,GY955.Roll,GY955.targetRoll);
//	Joint.Vp = PID_Calc(&Joint.PID_P,GY955.Pitch,GY955.targetPitch);

//	DXL1_setSyncMsg(USART_6,POSITION,12,
//	0x01, DigitalServo.MX_64[0].MiddleAngle +thighErr - 0*Joint.Vz +thighErr1 
//	+ Joint_getThighTarAng(Vy,0,temp), /*LH*/
//	0x02,DigitalServo.MX_64[1].MiddleAngle +crusErr + Joint.Vz + Joint.Vp + crusErr1 
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp),
//	
//	0x03,DigitalServo.MX_64[2].MiddleAngle +thighErr-0* Joint.Vz+thighErr1 
//	+ Joint_getThighTarAng(Vy,1.0f,temp), /*RH*/ 
//	0x04,DigitalServo.MX_64[3].MiddleAngle +crusErr+ Joint.Vz - Joint.Vp +crusErr1
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp),
//	
//	0x05,DigitalServo.MX_64[4].MiddleAngle +thighErr + 0*Joint.Vz 
//	+ Joint_getThighTarAng(Vy,0,temp), /*RF*/
//	0x06,DigitalServo.MX_64[5].MiddleAngle +crusErr - Joint.Vz - Joint.Vp+crusErr2 
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp),

//	0x07,DigitalServo.MX_64[6].MiddleAngle +thighErr + 0*Joint.Vz 
//	+ Joint_getThighTarAng(Vy,1.0f,temp), /*LF*/
//	0x08,DigitalServo.MX_64[7].MiddleAngle +crusErr- Joint.Vz + Joint.Vp+crusErr2
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp),
//	
//	0x09,DigitalServo.MX_64[8].MiddleAngle + hipErr + Joint_getHipTarAng(Omega + Vx, 0, temp),
//	0x0A,DigitalServo.MX_64[9].MiddleAngle - hipErr + Joint_getHipTarAng(Omega + Vx, 1.0f, temp),
//	0x0B,DigitalServo.MX_64[10].MiddleAngle + hipErr + Joint_getHipTarAng(Omega - Vx, 0, temp),
//	0x0C,DigitalServo.MX_64[11].MiddleAngle - hipErr + Joint_getHipTarAng(Omega - Vx, 1.0f, temp));
		Joint_UpslopeModel(Vx, Vy, Omega, T);
		PhotoelectricScan();/*光电开关扫描*/

}



//沙丘
static void Joint_SandDuneModel(int16_t Vx, int16_t Vy, int16_t Omega,float T)
{	
	static float temp=0;
//	static uint16_t flag=0,flag1=0;
	static int16_t  thighErr=-400,crusErr=-750,hipErr=-50,crusErr1=0,crusErr2=0,thighErr1=0;
  float thigh_anglecoe = 3,crus_anglecoe = 10;
//	static int16_t  thighErr=-800,crusErr=1700,hipErr=50;
	
	T = 0.25;
	
	Joint.Vz = PID_Calc(&Joint.PID_Z,GY955.Roll,GY955.targetRoll);
	Joint.Vp = 0;//PID_Calc(&Joint.PID_P,GY955.Pitch,GY955.targetPitch);
	
	if(Vx == 0 && Vy == 0 && Omega == 0)
		temp=0;
	else
	  temp = (temp>2*PI)?0:(temp+T);

	DXL1_setSyncMsg(USART_6,POSITION,12,
	/*LH*/	
	0x01, DigitalServo.MX_64[0].MiddleAngle +thighErr - 0*Joint.Vz +thighErr1 
	+ Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), 
	/*RH*/
	0x03,DigitalServo.MX_64[2].MiddleAngle +thighErr-0* Joint.Vz+thighErr1 
	+ Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), 
	/*RF*/
	0x05,DigitalServo.MX_64[4].MiddleAngle +thighErr + 0*Joint.Vz 
	+ Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), 
	/*LF*/
	0x07,DigitalServo.MX_64[6].MiddleAngle +thighErr + 0*Joint.Vz 
	+ Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), 	
	
	/*LH*/	
	0x02,DigitalServo.MX_64[1].MiddleAngle +crusErr + Joint.Vz + Joint.Vp + crusErr1 
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,crus_anglecoe),	
	/*RH*/ 
	0x04,DigitalServo.MX_64[3].MiddleAngle +crusErr+ Joint.Vz - Joint.Vp +crusErr1
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,crus_anglecoe),
	/*RF*/	
	0x06,DigitalServo.MX_64[5].MiddleAngle +crusErr - Joint.Vz - Joint.Vp+crusErr2
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,crus_anglecoe),
	/*LF*/	
	0x08,DigitalServo.MX_64[7].MiddleAngle +crusErr- Joint.Vz + Joint.Vp+crusErr2
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,crus_anglecoe),
	
	0x09,DigitalServo.MX_64[8].MiddleAngle + hipErr + Joint_getHipTarAng(Omega + Vx, 0, temp),
	0x0A,DigitalServo.MX_64[9].MiddleAngle - hipErr + Joint_getHipTarAng(Omega + Vx, 1.0f, temp),
	0x0B,DigitalServo.MX_64[10].MiddleAngle + hipErr + Joint_getHipTarAng(Omega - Vx, 0, temp),
	0x0C,DigitalServo.MX_64[11].MiddleAngle - hipErr + Joint_getHipTarAng(Omega - Vx, 1.0f, temp));
	
}



//绳子
static void Joint_RopeModel(int16_t Vx, int16_t Vy, int16_t Omega,float T)
{	
//	static float temp=0;
////	static uint16_t flag=0,flag1=0;
//	static int16_t  thighErr=-300,crusErr=-400,hipErr=-50,hipErr1 = -50,crusErr1=-500,crusErr2=-500,thighErr1=0;
// float thigh_anglecoe = 1,crus_anglecoe = 1;
////	static int16_t  thighErr=-800,crusErr=1700,hipErr=50;

//	if(Vx == 0 && Vy == 0 && Omega == 0)
//		temp=0;
//	else
//	  temp = (temp>2*PI)?0:(temp+T);


//	DXL1_setSyncMsg(USART_6,POSITION,12,
//	/*LH*/	
//	0x01, DigitalServo.MX_64[0].MiddleAngle +thighErr - 0*Joint.Vz +thighErr1 
//	+ Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), 
//	/*RH*/
//	0x03,DigitalServo.MX_64[2].MiddleAngle +thighErr-0* Joint.Vz+thighErr1 
//	+ Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), 
//	/*RF*/
//	0x05,DigitalServo.MX_64[4].MiddleAngle +thighErr + 0*Joint.Vz 
//	+ Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), 
//	/*LF*/
//	0x07,DigitalServo.MX_64[6].MiddleAngle +thighErr + 0*Joint.Vz 
//	+ Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), 	
//	
//	/*LH*/	
//	0x02,DigitalServo.MX_64[1].MiddleAngle  + Joint.Vz + Joint.Vp + crusErr1 
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,2),	
//	/*RH*/ 
//	0x04,DigitalServo.MX_64[3].MiddleAngle + Joint.Vz - Joint.Vp +crusErr1
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,2),
//	/*RF*/	
//	0x06,DigitalServo.MX_64[5].MiddleAngle  - Joint.Vz - Joint.Vp+crusErr2
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,2),
//	/*LF*/	
//	0x08,DigitalServo.MX_64[7].MiddleAngle - Joint.Vz + Joint.Vp+crusErr2
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,2),
//	
//	0x09,DigitalServo.MX_64[8].MiddleAngle + hipErr + Joint_getHipTarAng(Omega + Vx, 0, temp),
//	0x0A,DigitalServo.MX_64[9].MiddleAngle - hipErr + Joint_getHipTarAng(Omega + Vx, 1.0f, temp),
//	0x0B,DigitalServo.MX_64[10].MiddleAngle + hipErr1 + Joint_getHipTarAng(Omega - Vx, 0, temp),
//	0x0C,DigitalServo.MX_64[11].MiddleAngle - hipErr1 + Joint_getHipTarAng(Omega - Vx, 1.0f, temp));
//	
}


//绳子
static void Joint_RopeModelF(int16_t Vx, int16_t Vy, int16_t Omega,float T)
{	
//	static float temp=0;
//	static int16_t  thighErr=-300,crusErr=-600,hipErr=-50,hipErr1 = -50,crusErr1=-500,crusErr2=-500,thighErr1=0;
//  float thigh_anglecoe = 1,crus_anglecoe = 10;
////	static int16_t  thighErr=-800,crusErr=1700,hipErr=50;

//	if(Vx == 0 && Vy == 0 && Omega == 0)
//		temp=0;
//	else
//	  temp = (temp>2*PI)?0:(temp+T);


//	DXL1_setSyncMsg(USART_6,POSITION,12,
//	/*LH*/	
//	0x01, DigitalServo.MX_64[0].MiddleAngle +thighErr - 0*Joint.Vz +thighErr1 
//	+ Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), 
//	/*RH*/
//	0x03,DigitalServo.MX_64[2].MiddleAngle +thighErr-0* Joint.Vz+thighErr1 
//	+ Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), 
//	/*RF*/
//	0x05,DigitalServo.MX_64[4].MiddleAngle +thighErr + 0*Joint.Vz 
//	+ Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), 
//	/*LF*/
//	0x07,DigitalServo.MX_64[6].MiddleAngle +thighErr + 0*Joint.Vz 
//	+ Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), 	
//	
//	/*LH*/	
//	0x02,DigitalServo.MX_64[1].MiddleAngle  + Joint.Vz + Joint.Vp + crusErr1 
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,crus_anglecoe),	
//	/*RH*/ 
//	0x04,DigitalServo.MX_64[3].MiddleAngle + Joint.Vz - Joint.Vp +crusErr1
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,crus_anglecoe),
//	/*RF*/	
//	0x06,DigitalServo.MX_64[5].MiddleAngle  - Joint.Vz - Joint.Vp+crusErr2
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,0.5f),
//	/*LF*/	
//	0x08,DigitalServo.MX_64[7].MiddleAngle - Joint.Vz + Joint.Vp+crusErr2
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,0.5f),
//	
//	0x09,DigitalServo.MX_64[8].MiddleAngle + hipErr + Joint_getHipTarAng(Omega + Vx, 0, temp),
//	0x0A,DigitalServo.MX_64[9].MiddleAngle - hipErr + Joint_getHipTarAng(Omega + Vx, 1.0f, temp),
//	0x0B,DigitalServo.MX_64[10].MiddleAngle + hipErr1 + Joint_getHipTarAng(Omega - Vx, 0, temp),
//	0x0C,DigitalServo.MX_64[11].MiddleAngle - hipErr1 + Joint_getHipTarAng(Omega - Vx, 1.0f, temp));
//	
}


//绳子
static void Joint_RopeModelH(int16_t Vx, int16_t Vy, int16_t Omega,float T)
{	
//	static float temp=0;
//	static uint16_t flag=0,flag1=0;
//	static int16_t  thighErr=-300,crusErr=-600,hipErr=-50,hipErr1 =-50,crusErr1=-500,crusErr2=-500,thighErr1=0;
//  float thigh_anglecoe = 1,crus_anglecoe = 10;
////	static int16_t  thighErr=-800,crusErr=1700,hipErr=50;

//	if(Vx == 0 && Vy == 0 && Omega == 0)
//		temp=0;
//	else
//	  temp = (temp>2*PI)?0:(temp+T);


//	DXL1_setSyncMsg(USART_6,POSITION,12,
//	/*LH*/	
//	0x01, DigitalServo.MX_64[0].MiddleAngle +thighErr - 0*Joint.Vz +thighErr1 
//	+ Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), 
//	/*RH*/
//	0x03,DigitalServo.MX_64[2].MiddleAngle +thighErr-0* Joint.Vz+thighErr1 
//	+ Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), 
//	/*RF*/
//	0x05,DigitalServo.MX_64[4].MiddleAngle +thighErr + 0*Joint.Vz 
//	+ Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), 
//	/*LF*/
//	0x07,DigitalServo.MX_64[6].MiddleAngle +thighErr + 0*Joint.Vz 
//	+ Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), 	
//	
//	/*LH*/	
//	0x02,DigitalServo.MX_64[1].MiddleAngle  + Joint.Vz + Joint.Vp + crusErr1 
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,0.5f),	
//	/*RH*/ 
//	0x04,DigitalServo.MX_64[3].MiddleAngle + Joint.Vz - Joint.Vp +crusErr1
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,0.5f),
//	/*RF*/	
//	0x06,DigitalServo.MX_64[5].MiddleAngle  - Joint.Vz - Joint.Vp+crusErr2
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,crus_anglecoe),
//	/*LF*/	
//	0x08,DigitalServo.MX_64[7].MiddleAngle - Joint.Vz + Joint.Vp+crusErr2
//	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,crus_anglecoe),
//	
//	0x09,DigitalServo.MX_64[8].MiddleAngle + hipErr + Joint_getHipTarAng(Omega + Vx, 0, temp),
//	0x0A,DigitalServo.MX_64[9].MiddleAngle - hipErr + Joint_getHipTarAng(Omega + Vx, 1.0f, temp),
//	0x0B,DigitalServo.MX_64[10].MiddleAngle + hipErr1 + Joint_getHipTarAng(Omega - Vx, 0, temp),
//	0x0C,DigitalServo.MX_64[11].MiddleAngle - hipErr1 + Joint_getHipTarAng(Omega - Vx, 1.0f, temp));
//	
}
//平地走
static void Joint_walkModel(int16_t Vx, int16_t Vy, int16_t Omega,float T)
{	
	static float temp=0;
	static int16_t  thighErr=-400,crusErr=-700,hipErr=-50,crusErr1=0,crusErr2=0,thighErr1=0;
  float thigh_anglecoe = 5,crus_anglecoe = 5;

//	stati5c int16_t  thighErr=-800,crusErr=1700,hipErr=50;
	//T = 0.15;
	if(Vx == 0 && Vy == 0 && Omega == 0)
		temp=0;
	else
	  temp = (temp>2*PI)?0:(temp+T);

	DXL1_setSyncMsg(USART_6,POSITION,12,
	/*LH*/	
	0x01, DigitalServo.MX_64[0].MiddleAngle +thighErr - 0*Joint.Vz +thighErr1 
	+ Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), 
	/*RH*/
	0x03,DigitalServo.MX_64[2].MiddleAngle +thighErr-0* Joint.Vz+thighErr1 
	+ Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), 
	/*RF*/
	0x05,DigitalServo.MX_64[4].MiddleAngle +thighErr + 0*Joint.Vz 
	+ Joint_getThighTarAng(Vy,0,temp,thigh_anglecoe), 
	/*LF*/
	0x07,DigitalServo.MX_64[6].MiddleAngle +thighErr + 0*Joint.Vz 
	+ Joint_getThighTarAng(Vy,1.0f,temp,thigh_anglecoe), 	
	
	/*LH*/	
	0x02,DigitalServo.MX_64[1].MiddleAngle +crusErr + Joint.Vz + Joint.Vp + crusErr1 
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,crus_anglecoe),	
	/*RH*/ 
	0x04,DigitalServo.MX_64[3].MiddleAngle +crusErr+ Joint.Vz - Joint.Vp +crusErr1
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,crus_anglecoe),
	/*RF*/	
	0x06,DigitalServo.MX_64[5].MiddleAngle +crusErr - Joint.Vz - Joint.Vp+crusErr2
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,crus_anglecoe),
	/*LF*/	
	0x08,DigitalServo.MX_64[7].MiddleAngle +crusErr- Joint.Vz + Joint.Vp+crusErr2
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,crus_anglecoe),
	
	0x09,DigitalServo.MX_64[8].MiddleAngle + hipErr + Joint_getHipTarAng(Omega + Vx, 0, temp),
	0x0A,DigitalServo.MX_64[9].MiddleAngle - hipErr + Joint_getHipTarAng(Omega + Vx, 1.0f, temp),
	0x0B,DigitalServo.MX_64[10].MiddleAngle + hipErr + Joint_getHipTarAng(Omega - Vx, 0, temp),
	0x0C,DigitalServo.MX_64[11].MiddleAngle - hipErr + Joint_getHipTarAng(Omega - Vx, 1.0f, temp));
	
}


//上坡
static void Joint_UpslopeModel(int16_t Vx, int16_t Vy, int16_t Omega,float T)
{	
	static float temp=0;
	static int16_t  thighErr=-400,crusErr=-700,hipErr=-50,crusErr1=-0,crusErr2=0,thighErr1=0;
  float up_thigh_anglecoe = 5,up_crus_anglecoe = 8;
//	static int16_t  thighErr=-800,crusErr=1700,hipErr=50;
	if(Vx == 0 && Vy == 0 && Omega == 0)
		temp=0;
	else
	  temp = (temp>2*PI)?0:(temp+T);

	Joint.Vz = PID_Calc(&Joint.PID_Z,GY955.Roll,GY955.targetRoll);
	
	DXL1_setSyncMsg(USART_6,POSITION,12,
	/*LH*/	
	0x01, DigitalServo.MX_64[0].MiddleAngle +thighErr - 0*Joint.Vz +thighErr1 
	+ Joint_getThighTarAng(Vy,0,temp,up_thigh_anglecoe), 
	/*RH*/
	0x03,DigitalServo.MX_64[2].MiddleAngle +thighErr-0* Joint.Vz+thighErr1 
	+ Joint_getThighTarAng(Vy,1.0f,temp,up_thigh_anglecoe), 
	/*RF*/
	0x05,DigitalServo.MX_64[4].MiddleAngle +thighErr + 0*Joint.Vz 
	+ Joint_getThighTarAng(Vy,0,temp,up_thigh_anglecoe), 
	/*LF*/
	0x07,DigitalServo.MX_64[6].MiddleAngle +thighErr + 0*Joint.Vz 
	+ Joint_getThighTarAng(Vy,1.0f,temp,up_thigh_anglecoe), 	
	
	/*LH*/	
	0x02,DigitalServo.MX_64[1].MiddleAngle +crusErr + Joint.Vz + Joint.Vp + crusErr1 
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,up_crus_anglecoe),	
	/*RH*/ 
	0x04,DigitalServo.MX_64[3].MiddleAngle +crusErr+ Joint.Vz - Joint.Vp +crusErr1
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,up_crus_anglecoe),
	/*RF*/	
	0x06,DigitalServo.MX_64[5].MiddleAngle +crusErr - 0*Joint.Vz - Joint.Vp+crusErr2
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 0.25f, temp,up_crus_anglecoe),
	/*LF*/	
	0x08,DigitalServo.MX_64[7].MiddleAngle +crusErr- 0*Joint.Vz + Joint.Vp+crusErr2
	+ Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega), 1.25f, temp,up_crus_anglecoe),
	
	0x09,DigitalServo.MX_64[8].MiddleAngle + hipErr + Joint_getHipTarAng(Omega + Vx, 0, temp),
	0x0A,DigitalServo.MX_64[9].MiddleAngle - hipErr + Joint_getHipTarAng(Omega + Vx, 1.0f, temp),
	0x0B,DigitalServo.MX_64[10].MiddleAngle + hipErr + Joint_getHipTarAng(Omega - Vx, 0, temp),
	0x0C,DigitalServo.MX_64[11].MiddleAngle - hipErr + Joint_getHipTarAng(Omega - Vx, 1.0f, temp));
	
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-09 11:33
	* @brief   三角步态关节运动模型
	* @param   void
	* @retval  void
	*/
void Joint_NewWalkMotionModel(int16_t Vx, int16_t Vy, int16_t Omega)
{	
	static float temp=0;
  static int16_t  thighErr=-350,crusErr=-700,hipErr=-50;
	float up_thigh_anglecoe = 5,up_crus_anglecoe = 5;
	if(Vx == 0 && Vy == 0 && Omega == 0)
		temp=0;
	else
	  temp=temp>(2*PI)?0:(temp+0.2f);
	
	DXL1_setSyncMsg(USART_6,POSITION,12,
	                0x01,DigitalServo.MX_64[0].MiddleAngle +thighErr - 0*Joint.Vz
                  + Joint_getThighTarAng(Vy,-0.5f,temp,up_thigh_anglecoe), /*LF左前*/               
						      0x02,DigitalServo.MX_64[1].MiddleAngle +crusErr + Joint.Vz + Joint.Vp
                  + Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega),-0.25f,temp,up_crus_anglecoe),
									
									0x03,DigitalServo.MX_64[2].MiddleAngle +thighErr - 0*Joint.Vz
                  + Joint_getThighTarAng(Vy,-1.5f,temp,up_thigh_anglecoe), /*RH*/ 
									0x04,DigitalServo.MX_64[3].MiddleAngle +crusErr + Joint.Vz + Joint.Vp
                  + Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega),-1.25f,temp,up_crus_anglecoe),
								
                  0x05,DigitalServo.MX_64[4].MiddleAngle +thighErr - 0*Joint.Vz
                  + Joint_getThighTarAng(Vy,-1.0,temp,up_thigh_anglecoe), /*RH*/ 
									0x06,DigitalServo.MX_64[5].MiddleAngle +crusErr + Joint.Vz + Joint.Vp
                  + Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega),-0.75f,temp,up_crus_anglecoe),
								
									0x07,DigitalServo.MX_64[6].MiddleAngle +thighErr - 0*Joint.Vz
                  + Joint_getThighTarAng(Vy,0.0f,temp,up_thigh_anglecoe), /*LF*/ 
									0x08,DigitalServo.MX_64[7].MiddleAngle +crusErr + Joint.Vz + Joint.Vp
                  + Joint_getCrusTarAng(Filter.p_ABS(Vy) + Filter.p_ABS(Vx) + Filter.p_ABS(Omega),0.25f,temp,up_crus_anglecoe),
									
                  0x09,DigitalServo.MX_64[8].MiddleAngle + hipErr 
                  + Joint_getHipTarAng(Omega + Vx,0,temp),
									0x0A,DigitalServo.MX_64[9].MiddleAngle - hipErr
                  + Joint_getHipTarAng(Omega + Vx,1.0f,temp),
									0x0B,DigitalServo.MX_64[10].MiddleAngle + hipErr
                  + Joint_getHipTarAng(Omega - Vx,0,temp),
									0x0C,DigitalServo.MX_64[11].MiddleAngle - hipErr
                  + Joint_getHipTarAng(Omega - Vx,1.0f,temp));
}

	/**
	* @Data    2019-01-09 11:33
	* @brief   过沙丘
	* @param   void
	* @retval  void
	*/
void Joint_TextPassSandDune(uint8_t Temp)
{
//	static uint16_t  Tex=500;
//	static int16_t  thighErr=-300,crusErr=-800,hipErr=-50;
//	for(uint16_t i=0;i<Tex;i+=20)
//	{
//		DXL1_setSyncMsg(USART_6,POSITION,2,
//								0x01,DigitalServo.MX_64[0].MiddleAngle +thighErr - i , /*LH*/               
//								0x02,DigitalServo.MX_64[1].MiddleAngle +crusErr - i );
//		vTaskDelay(50);
//	}
//	for(uint16_t i=0;i<Tex;i+=20)
//	{
//		DXL1_setSyncMsg(USART_6,POSITION,1,              
//								0x02,DigitalServo.MX_64[1].MiddleAngle + crusErr - Tex + 2*i);
//		vTaskDelay(50);
//	}
//	for(uint16_t i=0;i<Tex;i+=20)
//	{
//		DXL1_setSyncMsg(USART_6,POSITION,1,
//								0x01,DigitalServo.MX_64[0].MiddleAngle + thighErr - Tex + i );
//		vTaskDelay(50);
//	}
//	
//	for(uint16_t i=0;i<Tex;i+=20)
//	{
//		DXL1_setSyncMsg(USART_6,POSITION,2,
//								0x03,DigitalServo.MX_64[2].MiddleAngle +thighErr - i , /*LH*/               
//								0x04,DigitalServo.MX_64[3].MiddleAngle +crusErr - i );
//		vTaskDelay(10);
//	}
//	for(uint16_t i=0;i<Tex;i+=20)
//	{
//		DXL1_setSyncMsg(USART_6,POSITION,2,
//								0x03,DigitalServo.MX_64[2].MiddleAngle + thighErr - Tex + i  , /*LH*/               
//								0x04,DigitalServo.MX_64[3].MiddleAngle + crusErr - Tex + i);
//		vTaskDelay(10);
//	}

//	vTaskDelay(50);
}
	/**
	* @Data    2019-01-09 11:33
	* @brief   过沙丘
	* @param   void
	* @retval  void
	*/



/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-03-06 15:31
	* @brief   关节控制状态机
	* @param   void
	* @retval  void
	*/
	void Joint_StateMachine(void)
	{
		static uint16_t timFlg=0,upslope_flag;
		static uint8_t ropeFlg=0;
		PhotoelectricScan();/*光电开关扫描*/
		KeyScan();
		GY955.targetYaw=GY955.targetYaw>360?GY955.targetYaw-360:GY955.targetYaw;
		GY955.targetYaw=GY955.targetYaw<0?GY955.targetYaw+360:GY955.targetYaw;
		
		Joint.Vspin =0;// PID_Calc(&Joint.PID_Spin,GY955.Yaw,GY955.targetYaw);
//		Joint.Vspin=PID_Calc(&Joint.PID_Spin,Camera.Offset,45);//摄像头偏移量伺服
//		if(GY955.Yaw> GY955.targetYaw-45 && turnFlg==1)
//		{
//			Joint.Vspin=50;
//		}
//		else
//    {
//			turnFlg=0;
//		  Joint.Vspin=0;
//		}
		if(PhoFlg.RF)
		{
			if(!KeyStatus)
			{
				vTaskDelay(300);
				RunFlag++;
				
			}
			else if(!PhoFlg.RH)
			{
				vTaskDelay(300);
			  RunFlag=7;
			}
			else if(!PhoFlg.LH)
			{
				vTaskDelay(300);
			  RunFlag=4;
			}
		}
		else
		{
		  switch (RunFlag)
			{
				case 0:
				//	Joint_NewTrotMotionModel(-DR16.ch3/2,DR16.ch2/2,DR16.ch1/2,0.1f);
					if(!PhoFlg.LF)//轻触开关
					{
						RunFlag=1;
						vTaskDelay(1000);
						Joint_RobotArmCtrl(3180,3050);//机械臂收起令牌
						vTaskDelay(500);
					}
					GY955.targetYaw = GY955.Yaw;
					Joint_Init();
					Joint_walkModel(0,0,0,0.25f);

				break;
					
				case 1:/*状态1 对角步态往沙丘*/
					//  if(Camera.Flag==4)//检测到转弯
//						if(++timFlg>500)//不用摄像头延时测试
//						{
//							timFlg=0;
//							GY955.targetYaw=GY955.Yaw-45;
//							GY955.targetYaw=GY955.targetYaw>360?GY955.targetYaw-360:GY955.targetYaw;
//	          	GY955.targetYaw=GY955.targetYaw<0?GY955.targetYaw+360:GY955.targetYaw;
//							RunFlag=2;
//						}

				    Joint.Vspin = PID_Calc(&Joint.PID_Spin, GY955.Yaw, GY955.targetYaw);					
						
						if (!PhoFlg.CLAMP && !PhoFlg.SLOPE)//到达沙丘
						{
							Joint_walkModel(0,0,0,0.3f);
							if(++timFlg>100)//延迟一会换步态
							{
								timFlg = 0;
								RunFlag = 2;
							}
							
						}
						else
						{
							timFlg = 0;
							Joint_walkModel(0,20,Joint.Vspin,0.3f);
						}
						
					break;

				case 2:/*状态2 态过沙丘*/
					
//						if(++timFlg>500)
//						{
//							timFlg=0;
//							GY955.targetYaw=GY955.Yaw+45;
//							GY955.targetYaw=GY955.targetYaw>360?GY955.targetYaw-360:GY955.targetYaw;
//		          GY955.targetYaw=GY955.targetYaw<0?GY955.targetYaw+360:GY955.targetYaw;
//							RunFlag=3;
//						}
					//Joint.Vspin = PID_Calc(&Joint.PID_Spin,GY955.Yaw,GY955.targetYaw);

        Joint_SandDuneModel(0, 40, 0, 0.3f);
				
				if(++timFlg>200)
				{
					if (PhoFlg.CLAMP && PhoFlg.SLOPE && (ABS(GY955.Roll) < 8))
					{
						timFlg = 0;
						RunFlag = 3;
					}
				}
	
					break;
				case 3://过绳子
//					  if(++timFlg>800)
//						{
//							timFlg=0;
//							RunFlag=0;
//						}
			    	timFlg++;
				    Joint.Vspin = PID_Calc(&Joint.PID_Spin,GY955.Yaw,GY955.targetYaw);
				
				    if(ropeFlg<4)
						{
						  if(timFlg<200)
							{
								Joint_RopeModelF(0,120,Joint.Vspin,0.1f);
							}
							else if(timFlg>=200&&timFlg<300)
							{
								Joint_RopeModel(-DR16.ch3/2, 120, Joint.Vspin, 0.15f);
							}
							else if(timFlg>=300&&timFlg<500)
							{
								Joint_RopeModelH(0,120,Joint.Vspin,0.1f);
							}
							else if(timFlg>=500&&timFlg<550)
							{
								Joint_RopeModel(-DR16.ch3/2, 120, Joint.Vspin, 0.15f);
							}
							else
							{
							  timFlg=0;
								ropeFlg++;
							}
						}
						else
						{
						  Joint_RopeModel(0,0,0,0.1f);
						}
				
#if 0				
            if(timFlg<200)
						{
							Joint_RopeModelF(0,150,Joint.Vspin,0.15f);
						}
						else if(timFlg>200&&timFlg<400)
						{
						  Joint_RopeModelH(0,150,Joint.Vspin,0.15f);
						}
						else if(timFlg>400&&timFlg<600)
						{
						  Joint_RopeModelF(0,150,Joint.Vspin,0.15f);
						}
						else if(timFlg>600&&timFlg<800)
						{
						  Joint_RopeModelH(0,150,Joint.Vspin,0.15f);
						}
						else if(timFlg>800&&timFlg<1000)
						{
						  Joint_RopeModelF(0,150,Joint.Vspin,0.15f);
						}
						else if(timFlg>1000&&timFlg<1200)
						{
						  Joint_RopeModelH(0,150,Joint.Vspin,0.15f);
						}
						else
						{
							Joint_RopeModel(0,0,0,0.1f);
						}
#endif
#if 0
						
					  if(timFlg<400)
						{
				      Joint_NewTrotMotionModel(-DR16.ch3/2, 35, Joint.Vspin, 0.25f);
						}
					  else if(timFlg<600)
						{
							Joint_RopeModelF(0,120,Joint.Vspin,0.1f);
						}
						else if(timFlg>600&&timFlg<800)
						{
						  Joint_RopeModelH(0,120,Joint.Vspin,0.1f);
						}
						else if(timFlg>800&&timFlg<1000)
						{
						  Joint_RopeModelF(0,120,Joint.Vspin,0.1f);
						}
						else if(timFlg>1000&&timFlg<1400)
						{
						  Joint_RopeModelH(0,120,Joint.Vspin,0.1f);
						}
						else
						{
							Joint_RopeModel(0,0,0,0.1f);
						}
#endif
				break;
				
				case 4://爬坡
//					 	if(++timFlg>300)
//						{
//							timFlg=0;
//							RunFlag=5;
//							
//						}
					 //Joint.Vspin = PID_Calc(&Joint.PID_Spin,GY955.Yaw,GY955.targetYaw);
					 if(!PhoFlg.SDOP)
					 {
						 RunFlag = 5;
					 }
					 else
					 {
						 Joint_Init();
						 Joint_NewTrotMotionModel(0,0,0,0.3f);
						 RunFlag = 4;
					 }
						upslope_flag = 0;
//					else
//					{
//						if(++timFlg>100)
//						{					
//							RunFlag	= 5;						
//							Joint_NewTrotMotionModel(0,0,0,0.3f);
//							Joint_RobotArmCtrl(1900,1002);//机械臂收起令牌
//						}
//						else
//						{
//							Joint_NewTrotMotionModel(-DR16.ch3/2, 50, DR16.ch1/2, 0.25f);
//						}
//					}
					
//					if (!PhoFlg.CLAMP && !PhoFlg.SLOPE)//到达终点
//					{
//						Joint_walkModel(0,0,0,0.3f);
//						if(++timFlg>100)
//						{							
//							Joint_RobotArmCtrl(1900,1002);//机械臂收起令牌
//						}
//					}
//					else
//					{
//						Joint_UpslopeModel(0,0,0,0.2f);
//					}
					
				break;
				case 5:		
					Joint.Vspin = PID_Calc(&Joint.PID_Spin,GY955.Yaw,GY955.targetYaw);
					if(upslope_flag)
					{
						if((Filter.p_ABS(GY955.Roll - GY955.targetRoll)< 3)&&(++timFlg>50))
						{
							RunFlag = 6;
						}
						else
						{
							Joint_NewTrotMotionModel(-DR16.ch3/2, 50, Joint.Vspin, 0.25f);
						}
					}
					else
					{
						if(Filter.p_ABS(GY955.Roll - GY955.targetRoll)> 7)
						{
							timFlg = 0;
							upslope_flag = 1;
						}
						else
						{
							Joint_NewTrotMotionModel(-DR16.ch3/2, 50, Joint.Vspin, 0.25f);
						}
					}
					 
				break;
				case 6:
						upslope_flag = 0;
						Joint_NewTrotMotionModel(0,0,0,0.3f);
					  Joint_RobotArmCtrl(1900,1002);//机械臂收起令牌
					break;
				case 7:
					  Joint.Vspin = PID_Calc(&Joint.PID_Spin,GY955.Yaw,GY955.targetYaw);
				   	timFlg++;
						if(timFlg<150)
						{
				      Joint_RopeModel(-DR16.ch3/2, 120, Joint.Vspin, 0.25f);
						}
						else
						{
							timFlg=0;
							RunFlag=3;
						}
				break;
				
				default:

					break;
			}
		}


	}


/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-03-06 15:31
	* @brief   机械臂控制
	* @param   UpperarmTarang:上臂目标值、ForearmTarang ：前臂目标值
	* @retval  void
	*/
void Joint_RobotArmCtrl(uint16_t UpperarmTarang,uint16_t ForearmTarang)
{
	DXL1_setSyncMsg(USART_6,POSITION,2,0x0D,UpperarmTarang,0x0E,ForearmTarang);
}
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-03-06 15:31
	* @brief   单条腿控制 
	* @param   void
	* @retval  void
	*/
void Joint_Legs(int16_t Vx, int16_t Vy, uint16_t Delay)
{
	static float temp=0;

	temp=temp>2*PI?0:temp+0.01f;
	DXL1_setSyncMsg(USART_6,POSITION,3,
									0x01,2648 + Joint_getThighTarAng(Vy,0,temp,2), /*LH*/               
									0x02,2648 + Joint_getCrusTarAng(Vy + Vx ,-0.1f,temp,2),
									0x09,2648 + Joint_getThighTarAng(Vx,0,temp,2));
	vTaskDelay(Delay);
}
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-09 11:33
	* @brief   关节运动测试
	* @param   void
	* @retval  void
	*/
void Joint_MotionTest(void)
{
	DXL1_setSyncMsg(USART_6,POSITION,4,0x01,2648,0x03,2648,0x05,2648,0x07,2648);
	vTaskDelay(80);
	DXL1_setSyncMsg(USART_6,POSITION,4,0x02,2648,0x04,2648,0x06,2648,0x08,2648);
	vTaskDelay(220);

	DXL1_setSyncMsg(USART_6,POSITION,4,0x01,2750,0x03,2648,0x05,2648,0x07,2648);
	vTaskDelay(5);
	DXL1_setSyncMsg(USART_6,POSITION,4,0x02,2750,0x04,2648,0x06,2648,0x08,2648);  
	vTaskDelay(220);

}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-09 12:34
	* @brief   
	* @param   void
	* @retval  void
	*/
void Thigh_Forward(void)
{
	Motor.M6020[0].targetAngle=M6020_MEDIAN(0)+1024;
	Motor.M6020[1].targetAngle=M6020_MEDIAN(1)-1024;
	Motor.M6020[2].targetAngle=M6020_MEDIAN(2)+1024;
	Motor.M6020[3].targetAngle=M6020_MEDIAN(3)-1024;
	Motor.p_M6020setCur(CAN_1);

}


/*---------------------L O C A L - F U N C T I O N S--------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-09 11:33
	* @brief   髋关节控制
	* @param   TarAng 目标角度， Phase相位 ，Temp 倍率
	* @retval  滤波后的角度
	*/

static int16_t Joint_getHipTarAng(int16_t TarAng ,float Phase,float Temp)
{
	  TarAng=TarAng>100?100:TarAng;
		TarAng=TarAng<-100?-100:TarAng;
	  return ((Curve_Sin(TarAng,1,Phase,0,Temp)));
}


/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-09 11:33
	* @brief   大腿控制
	* @param   TarAng 目标角度， Phase相位 ，Temp 倍率
	* @retval  滤波后的角度
	*/

static int16_t Joint_getThighTarAng(int16_t TarAng ,float Phase,float Temp,float coe)
{
  	TarAng*=coe;
	  TarAng=TarAng>400?400:TarAng;
		TarAng=TarAng<-400?-400:TarAng;
//		temp=((Curve_Sin(TarAng,1,Phase,0,Temp)));
//		if(temp<0)
//			return temp;
//		else
//			return 0;
	  return ((Curve_Sin(TarAng,1,Phase,0,Temp)));
}

//static int16_t Joint_getThighTarAng(int16_t TarAng ,float Phase,float Temp)
//{
//  	TarAng*=2;
//	  TarAng=TarAng>400?400:TarAng;
//		TarAng=TarAng<-400?-400:TarAng;
//	  if(Temp>PI/2&&Temp<2*PI)
//	    return ((Curve_SinWalk(TarAng,2.0f/3.0f,Phase,0,Temp)));
//		else
//			return ((Curve_SinWalk(TarAng,1,Phase,0,Temp)));
//}
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-09 11:33
	* @brief   小腿控制
	* @param   TarAng 目标角度， Phase相位 ，Temp 倍率
	* @retval  滤波后的角度
	*/
static int16_t Joint_getCrusTarAng(int16_t TarAng ,float Phase,float Temp,float coe)
{
	static int16_t temp=0;
	TarAng*=coe;
	TarAng=TarAng>1000?1000:TarAng;
	TarAng=TarAng<-1000?-1000:TarAng;
	temp=Curve_Sin(Filter.p_ABS(TarAng),1,Phase,0,Temp);
  if(temp<0)
	  return temp;
	else
		return 0;
//	return Curve_Sin(Filter.p_ABS(TarAng),2,Phase,0,Temp);
//		  if(Temp>PI/2&&Temp<2*PI)
//	    return ((Curve_SinWalk(TarAng,2.0f/3.0f,Phase,0,Temp)));
//		else
//			return ((Curve_SinWalk(TarAng,1,Phase,0,Temp)));
	//return Curve_Sin(Filter.p_ABS(TarAng), 1, Phase, 0, Temp);
}


	/**
	* @Data    2019-01-09 11:33
	* @brief   小腿控制
	* @param   TarAng 目标角度， Phase相位 ，Temp 倍率
	* @retval  滤波后的角度
	*/
//static int16_t Joint_getRopeCrusTarAng(int16_t TarAng ,float Phase,float Temp,float coe)
//{
//	static int16_t temp=0;
//	TarAng*=coe;
//	TarAng=TarAng>1200?1200:TarAng;
//	TarAng=TarAng<-1200?-1200:TarAng;
//	temp=Curve_Sin(Filter.p_ABS(TarAng),1,Phase,0,Temp);
//  if(temp<0)
//	  return temp;
//	else
//		return 0;
////	return Curve_Sin(Filter.p_ABS(TarAng),2,Phase,0,Temp);
////		  if(Temp>PI/2&&Temp<2*PI)
////	    return ((Curve_SinWalk(TarAng,2.0f/3.0f,Phase,0,Temp)));
////		else
////			return ((Curve_SinWalk(TarAng,1,Phase,0,Temp)));
//	//return Curve_Sin(Filter.p_ABS(TarAng), 1, Phase, 0, Temp);
//}
/*-----------------------------------FILE OF END------------------------------*/






