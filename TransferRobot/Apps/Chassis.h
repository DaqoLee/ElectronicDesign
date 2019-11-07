/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : Chassis.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-02-20               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __CHASSIS_H 
#define __CHASSIS_H 

  
/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"
#include "pid.h"
/*-------------------------G L O B A L - T Y P E S----------------------------*/


typedef struct
{
	int16_t Vx;			    //Vx速度
	int16_t Vy;				//Vy速度
	int16_t Vspin;			//自旋速度

	pid_t   PID_X;
	pid_t   PID_Y;
	pid_t   PID_Spin;

}Chassis_t;
/*------------------------G L O B A L - M A C R O S --------------------------*/

#define MECANUM   1  /*麦克纳姆轮*/
#define THROMNI		0  /*三轮全向*/
#define FOROMNI		0  /*四轮全向*/

#define PI        3.141592653589

/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern Chassis_t Chassis;
extern int16_t Speed;
/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/
void Chassis_MotionModel(float Vx, float Vy, float Omega, int16_t *Speed);
void Chassis_Init(void);
void Chassis_Ctrl(void);


#endif	// __CHASSIS_H
/*----------------------------------FILE OF END-------------------------------*/


 

