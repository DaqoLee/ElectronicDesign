/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : Gyro.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-02-23               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __GYRO_H 
#define __GYRO_H 

  
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "stm32f4xx.h"


/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef struct 
{
	uint8_t buff[15];
	
  float  AccX;
  float  AccY;
  float  AccZ;

  float  Mag_X;
  float  Mag_Y;
  float  Mag_Z;

  int16_t  Gyr_X;
  int16_t  Gyr_Y;
  int16_t  Gyr_Z;

  float  Yaw;
  float  Roll;
  float  Pitch;

  float	 Q[4];

  float  targetYaw;
  float  targetRoll;
  float  targetPitch;

  void (*pGY955getMsg) (uint8_t *pGyroData);

}GY955_t;

/*------------------------G L O B A L - M A C R O S --------------------------*/



/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern GY955_t GY955;

/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/

void Gyro_Init(void);


#endif	// __GYRO_H
/*----------------------------------FILE OF END-------------------------------*/





 

