/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : OpticFlow.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-03-14               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __OPTICFLOW_H 
#define __OPTICFLOW_H 

  
/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"

/*-------------------------G L O B A L - T Y P E S----------------------------*/



/*------------------------G L O B A L - M A C R O S --------------------------*/

typedef struct 
{
  int16_t DeltaX;
  int16_t DeltaY;
  /* data */
}OpticFlow_t;


/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern OpticFlow_t OpticFlow;

/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/

void OpticFlow_getMassage(uint8_t *OFbuffer);


#endif	// __OPTICFLOW_H
/*----------------------------------FILE OF END-------------------------------*/


