/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|--FileName    : LED.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                         
|--Date        : 2019-03-17               
|--Libsupports : 
|--Description : STM32F4xx_DFP ( 2.9.0)                                                      
|-----------------------------declaration of end-----------------------------|
 **/
#ifndef __LED_H 
#define __LED_H 


/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "stm32f4xx.h"


/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef enum
{
	LED0=0,
	LED1,
	LED2,
	LED3,
	LED4,
	LED5,
	LED6,
	LED_R, 
	LED_G		 
}LED_State_e;

/*------------------------G L O B A L - M A C R O S --------------------------*/

#define LED_R_GPIO_CLK            RCC_AHB1Periph_GPIOB
#define LED_R_PIN                 GPIO_Pin_4
#define LED_R_GPIO_PORT           GPIOB

#define LED_G_GPIO_CLK            RCC_AHB1Periph_GPIOB
#define LED_G_PIN                 GPIO_Pin_5
#define LED_G_GPIO_PORT           GPIOB

#define LED0_GPIO_CLK             RCC_AHB1Periph_GPIOE
#define LED0_PIN                  GPIO_Pin_9
#define LED0_GPIO_PORT            GPIOE

#define LED1_GPIO_CLK             RCC_AHB1Periph_GPIOE
#define LED1_PIN                  GPIO_Pin_10
#define LED1_GPIO_PORT            GPIOE

#define LED2_GPIO_CLK             RCC_AHB1Periph_GPIOE
#define LED2_PIN                  GPIO_Pin_11
#define LED2_GPIO_PORT            GPIOE

#define LED3_GPIO_CLK             RCC_AHB1Periph_GPIOE
#define LED3_PIN                  GPIO_Pin_12
#define LED3_GPIO_PORT            GPIOE

#define LED4_GPIO_CLK             RCC_AHB1Periph_GPIOE
#define LED4_PIN                  GPIO_Pin_13
#define LED4_GPIO_PORT            GPIOE

#define LED5_GPIO_CLK             RCC_AHB1Periph_GPIOE
#define LED5_PIN                  GPIO_Pin_14
#define LED5_GPIO_PORT            GPIOE

#define LED6_GPIO_CLK             RCC_AHB1Periph_GPIOE
#define LED6_PIN                  GPIO_Pin_15
#define LED6_GPIO_PORT            GPIOE

#define LASER_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define LASER_PIN                  GPIO_Pin_12
#define LASER_GPIO_PORT            GPIOA




#define LED_ON(x)			if((LED_State_e)x==LED_R)     {GPIOB->BSRRH|=LED_R_PIN;}\
											else if((LED_State_e)x==LED_G){GPIOB->BSRRH|=LED_G_PIN;}\
											else {GPIOE->BSRRH|=1<<(9+(LED_State_e)x);}
													
#define LED_OFF(x)		if((LED_State_e)x==LED_R)     {GPIOB->BSRRL|=LED_R_PIN;}\
											else if((LED_State_e)x==LED_G){GPIOB->BSRRL|=LED_G_PIN;}\
											else {GPIOE->BSRRL|=1<<(9+(LED_State_e)x);}
													
#define LED_TOGGLE(x)	if((LED_State_e)x==LED_R)			 {GPIOB->ODR ^=LED_R_PIN;}\
											else if((LED_State_e)x==LED_G) {GPIOB->ODR ^=LED_G_PIN;}\
											else {GPIOE->ODR ^= 1<<(9+(LED_State_e)x);}
/*----------------------G L O B A L - D E F I N E S---------------------------*/



/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/

void LED_Init(void);

/*-----------------------------------file of end------------------------------*/
#endif	// __LED_H


