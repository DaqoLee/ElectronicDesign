/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|--FileName    : Key.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                         
|--Date        : 2019-03-18               
|--Libsupports : 
|--Description : STM32F4xx_DFP ( 2.9.0)                                                      
|-----------------------------declaration of end-----------------------------|
 **/
#ifndef __KEY_H 
#define __KEY_H 


/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"

/*-------------------------G L O B A L - T Y P E S----------------------------*/



/*------------------------G L O B A L - M A C R O S --------------------------*/

#define KEY_GPIO_CLK             	RCC_AHB1Periph_GPIOD
#define KEY_PIN                  	GPIO_Pin_7
#define KEY_GPIO_PORT           	GPIOD


/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern uint8_t KeyStatus;

/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/

void KEY_Init(void);
void KeyScan(void);

#endif	// __KEY_H
/*-----------------------------------file of end------------------------------*/

