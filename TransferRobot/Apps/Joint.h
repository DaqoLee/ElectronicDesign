/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : Joint.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-02-20               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __JOINT_H 
#define __JOINT_H 

  
/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"
#include "pid.h"
/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef struct
{
	int16_t Vx;			    //Vx速度
	int16_t Vy;				//Vy速度
	int16_t Vz;	
	int16_t Vp;	
	int16_t Vspin;			//自旋速度

	pid_t   PID_X;
	pid_t   PID_Z;
	pid_t   PID_Y;
	pid_t   PID_P;
	pid_t   PID_Spin;

}Joint_t;

/*------------------------G L O B A L - M A C R O S --------------------------*/

#define ABS(x)		 ((x>0)? (x): (-x)) 
#define LIMIT(x,y) ((x>y)? (y): (x))

/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern Joint_t Joint;
extern uint8_t RunFlag;
/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/
void Joint_Init(void);
void Thigh_M6020Ctrl(void);
void Joint_MotionTest(void);
void Joint_StateMachine(void);
void Joint_TextPassSandDune(uint8_t Temp);
void Joint_RobotArmCtrl(uint16_t UpperarmTarang,uint16_t ForearmTarang);
void Joint_TrotMotionModel(int16_t Vx, int16_t Vy, int16_t Omega);
void Joint_TrotMotionModelx(int16_t Vx, int16_t Vy, int16_t Omega);
void Joint_WalkMotionModel(int16_t Vx, int16_t Vy, int16_t Omega);
void Joint_NewTrotMotionModel(int16_t Vx, int16_t Vy, int16_t Omega,float T);
void Joint_NewWalkMotionModel(int16_t Vx, int16_t Vy, int16_t Omega);
void Joint_RobotArmCtrl(uint16_t UpperarmTarang,uint16_t ForearmTarang);
#endif	// __JOINT_H
/*----------------------------------FILE OF END-------------------------------*/



 

