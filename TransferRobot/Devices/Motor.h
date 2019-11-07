/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : Motor.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-01-08               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __MOTOR_H 
#define __MOTOR_H 

/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "pid.h"


/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef enum
{
	CAN_1   =1,
	CAN_2   =2
}CANx_e;

typedef enum
{
  LF =1,
	RF,
	RB,
	LB
}M6020x_e;

typedef struct
{
	#define M3508_ID_START	0x201
  #define M3508_ID_END	  0x204
	
	uint16_t realAngle;			/*实际角度*/
	 int16_t realSpeed;			/*实际速度*/
	 int16_t realCurrent;		/*实际电流*/
	 uint8_t temperture;		/*实际温度*/
	
	 uint16_t lastRealAngle;
	 int16_t turnsNum;
	 int32_t totalAngle;
	
   int16_t targetCurrent;	/*目标电流*/
	 int16_t targetSpeed;		/*目标速度*/
	 int32_t targetAngle;		/*目标角度*/
	
	 pid_t OutPID;				/*外环PID*/
	 pid_t InPID;					/*内环PID*/

}M3508_t;


typedef struct
{
	#define M2006_ID_START	0x201
  #define M2006_ID_END	  0x204
	
	uint16_t realAngle;			/*实际角度*/
	int16_t  realSpeed;			/*实际速度*/
	int16_t  realCurrent;		/*实际电流*/
	uint8_t  temperture;		/*实际温度*/
	
   int16_t targetCurrent;	/*目标电流*/
	 int16_t targetSpeed;		/*目标速度*/
	uint16_t targetAngle;		/*目标角度*/

}M2006_t;


typedef struct
{
	#define M6623_ID_START	0x205
  #define M6623_ID_END	  0x206

	uint16_t realAngle;			
	 int16_t realCurrent;		 
	 uint8_t temperture;		
	
   int16_t targetCurrent;	
	uint16_t targetAngle;		

}M6623_t;

typedef struct
{
	
	#define M6020_ID_START	0x205
  #define M6020_ID_END	  0x208
	
  /*定义太长了，换个短一点的*/
	#define M6020_MEDIAN(i)   (Motor.M6020[i].medianAngle)

  #define RATIO          (0.1f) 
	#define M6020_RANGE    4096

	uint16_t realAngle;			
	 int16_t realSpeed;			
	 int16_t realCurrent;	
	 uint8_t temperture;		
	
   int16_t targetCurrent;	
	uint16_t targetSpeed;		
	uint16_t targetAngle;		
	uint16_t medianAngle;

     pid_t OutPID;				/*外环PID*/
	   pid_t InPID;					/*内环PID*/
	
	
}M6020_t;

typedef struct
{
	M3508_t M3508[4];
  M6623_t M6623[2];
  M6020_t M6020[4];
  M2006_t M2006[4];
	
  void (*p_M6020setCur)	(CANx_e CANx);
	void (*p_M3508setCur)	(CANx_e CANx);
	void (*p_M2006setCur)	(CANx_e CANx);
	
	void (*p_M6020setTarAngle)(uint8_t Motor_ID, int16_t DR16_chx);
	
	void (*p_M6020getMsg) (CanRxMsg RxMessage);
	void (*p_M3508getMsg) (CanRxMsg RxMessage);
	void (*p_M2006getMsg) (CanRxMsg RxMessage);
	void (*p_M6623getMsg) (CanRxMsg RxMessage);
	
}Motor_t;

/*------------------------G L O B A L - M A C R O S --------------------------*/
#define MOTOR_MAX_ANGLE 8191

/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern Motor_t Motor;


/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/

void Motor_Init(void);


#endif	// __MOTOR_H
/*----------------------------------FILE OF END-------------------------------*/







 

