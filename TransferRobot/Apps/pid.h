/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : pid.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-01-14               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __PID_H 
#define __PID_H 

/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"

/*-------------------------G L O B A L - T Y P E S----------------------------*/

enum{
    LLAST	= 0,
    LAST 	= 1,
    NOW 	= 2,
    
    POSITION_PID,
    DELTA_PID,
};
typedef struct __pid_t
{
	float P;
	float I;
	float D;
	
	float set[3];				
	float get[3];				
	float err[3];			
	float erc[3];
	
	float Pout;						
	float Iout;						
	float Dout;						
	
	float posOut;						
	float lastPosOut;				  
	float deltaU;						
	float deltaOut;					
	float lastDeltaOut;
    
	float maxErr;
	float deadband;				

	uint16_t maxInput;
	uint16_t errRange;
	uint32_t pidMode;
	uint32_t maxOutput;				
	uint32_t integralLimit;		
    
	void (*f_ParamInit)(struct __pid_t *pid,  
									uint32_t pid_mode,
									uint32_t maxOutput,
									uint32_t integralLimit,
									float p,
									float i,
									float d);
	void (*f_PID_Reset)(struct __pid_t *pid, float p, float i, float d);

}pid_t;

/*------------------------G L O B A L - M A C R O S --------------------------*/

#define ABS(x)		((x>0)? (x): (-x)) 

#define NB -60
#define NM -40
#define NS -20
#define ZO 0
#define PS 20
#define PM 40
#define PB 60

/*----------------------G L O B A L - D E F I N E S---------------------------*/



/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/
void PID_StructInit(
    pid_t* pid,
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,
    
    float 	kp, 
    float 	ki, 
    float 	kd);
    
float PID_Calc(pid_t* pid, float fdb, float ref);



#endif	// __PID_H
/*----------------------------------FILE OF END-------------------------------*/










	



 

