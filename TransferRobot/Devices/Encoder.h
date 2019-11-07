/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : Encoder.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-01-10               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __ENCODER_H 
#define __ENCODER_H 
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "stm32f4xx.h"
#include "pid.h"
#pragma anon_unions

/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef struct
{						
  uint8_t buff[28];	
	union
	{
		uint8_t DataBuff[24];
		struct
		{
			float realZ_Angle;																		
			float realX_Angle;
			float realY_Angle;
			float realX_Coords;
			float realY_Coords;
			float realOmega;
		};
	}; 
	float targetZ_Angle;																		
	float targetX_Angle;
	float targetY_Angle;
	float targetX_Coords;
	float targetY_Coords;
	float targetOmega;

}Posture_t;	



/*------------------------G L O B A L - M A C R O S --------------------------*/



/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern Posture_t Posture;

/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/


void Posture_getMessage(uint8_t *EncoderBuf);

#endif	// __ENCODER_H
/*----------------------------------FILE OF END-------------------------------*/


