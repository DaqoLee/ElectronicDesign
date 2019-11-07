/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : BSP_TIM.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-01-08               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __BSP_TIM_H 
#define __BSP_TIM_H 

  
/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"

/*----------------------G L O B A L - D E F I N E S---------------------------*/



/*-------------------------G L O B A L - T Y P E S----------------------------*/



/*------------------------G L O B A L - M A C R O S --------------------------*/

#define TIM1_CH1_GPIO_CLK          	RCC_AHB1Periph_GPIOA
#define TIM1_CH1_GPIO_PORT         	GPIOA
#define TIM1_CH1_Pin          	 	GPIO_Pin_8
#define TIM1_CH1_PINSOURCE			GPIO_PinSource8

#define TIM1_CH2_GPIO_CLK          	RCC_AHB1Periph_GPIOA
#define TIM1_CH2_GPIO_PORT         	GPIOA
#define TIM1_CH2_Pin          	 	GPIO_Pin_9
#define TIM1_CH2_PINSOURCE			GPIO_PinSource9

#define TIM1_CH3_GPIO_CLK          	RCC_AHB1Periph_GPIOE
#define TIM1_CH3_GPIO_PORT         	GPIOE
#define TIM1_CH3_Pin          	 	GPIO_Pin_13
#define TIM1_CH3_PINSOURCE			GPIO_PinSource13

#define TIM1_CH4_GPIO_CLK          	RCC_AHB1Periph_GPIOE
#define TIM1_CH4_GPIO_PORT         	GPIOE
#define TIM1_CH4_Pin          	 	GPIO_Pin_14
#define TIM1_CH4_PINSOURCE			GPIO_PinSource14

/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/


void BSP_TIM6Init(uint16_t prescaler,uint16_t period);

#endif	// __BSP_TIM_H
/*----------------------------------FILE OF END-------------------------------*/





 

