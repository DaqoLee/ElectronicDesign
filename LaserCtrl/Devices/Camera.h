/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|--FileName    : Camera.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                         
|--Date        : 2019-03-17               
|--Libsupports : 
|--Description : STM32F4xx_DFP ( 2.9.0)                                                      
|-----------------------------declaration of end-----------------------------|
 **/
#ifndef __CAMERA_H 
#define __CAMERA_H 


/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"
#include "pid.h"

/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef struct 
{
  uint8_t buff[12];

  int16_t Angle;
  int16_t Offset;
	uint8_t Flag;
}Camera_t;


typedef struct
{
	  uint8_t buff[28];
    int16_t X;
	  int16_t Y;
	  uint8_t Flag;
	  uint8_t Num;
	  uint8_t ON;
	
	  int16_t TarX;
	  int16_t TarY;
	  
	  pid_t PIDx;
	  pid_t PIDy;
	
	
}Laser_t;

/*------------------------G L O B A L - M A C R O S --------------------------*/



/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern Camera_t Camera;
extern Laser_t Laser;
extern uint8_t RFIDBuf[9];
/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/
void Camera_getMassage(uint8_t *CameraBuff);
void RFID_getMassage(uint8_t *CameraBuff);
void Laser_getMessage(uint8_t *LaserBuf);
#endif	// __CAMERA_H
/*-----------------------------------file of end------------------------------*/


