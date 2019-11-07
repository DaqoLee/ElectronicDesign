/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|--FileName    : Ultrasonic.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                         
|--Date        : 2019-03-17               
|--Libsupports : 
|--Description : STM32F4xx_DFP ( 2.9.0)                                                      
|-----------------------------declaration of end-----------------------------|
 **/
#ifndef __ULTRASONIC_H 
#define __ULTRASONIC_H 


/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"

/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef struct 
{
  uint8_t buff[20];

  uint16_t LH;
  uint16_t RH;
  uint16_t LF;
  uint16_t RF;
}Ultrasonic_t;


/*------------------------G L O B A L - M A C R O S --------------------------*/



/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern Ultrasonic_t Ultrasonic;

/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/



#endif	// __ULTRASONIC_H
/*-----------------------------------file of end------------------------------*/


