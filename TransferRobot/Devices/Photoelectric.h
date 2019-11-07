/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|--FileName    : Photoelectric.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                         
|--Date        : 2019-03-18               
|--Libsupports : 
|--Description : STM32F4xx_DFP ( 2.9.0)                                                      
|-----------------------------declaration of end-------------------------------|
 **/
#ifndef __PHOTOELECTRIC_H 
#define __PHOTOELECTRIC_H 


/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"

/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef struct 
{
  uint8_t LF : 1;
  uint8_t LH : 1;
  uint8_t RF : 1;
  uint8_t RH : 1;
	uint8_t CLAMP :1;
	uint8_t SLOPE :1;
	uint8_t SDOP :1;
}PhoFlg_t;


/*------------------------G L O B A L - M A C R O S --------------------------*/

#define LF_GPIO_CLK             	RCC_AHB1Periph_GPIOA
#define LF_PIN                  	GPIO_Pin_0
#define LF_GPIO_PORT           	  GPIOA

#define LH_GPIO_CLK             	RCC_AHB1Periph_GPIOA
#define LH_PIN                  	GPIO_Pin_2
#define LH_GPIO_PORT           	  GPIOA

#define RF_GPIO_CLK             	RCC_AHB1Periph_GPIOA
#define RF_PIN                  	GPIO_Pin_1
#define RF_GPIO_PORT           	  GPIOA

#define RH_GPIO_CLK             	RCC_AHB1Periph_GPIOA
#define RH_PIN                  	GPIO_Pin_3
#define RH_GPIO_PORT           	  GPIOA

#define SDOP_GPIO_CLK             	RCC_AHB1Periph_GPIOA
#define SDOP_PIN                  	GPIO_Pin_10
#define SDOP_GPIO_PORT           	GPIOA

#define CLAMP_GPIO_CLK             	RCC_AHB1Periph_GPIOA
#define CLAMP_PIN                  	GPIO_Pin_9
#define CLAMP_GPIO_PORT           	GPIOA

#define SLOPE_GPIO_CLK             	RCC_AHB1Periph_GPIOA
#define SLOPE_PIN                  	GPIO_Pin_8
#define SLOPE_GPIO_PORT           	GPIOA

/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern PhoFlg_t PhoFlg;

/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/
void Photoelectric_Init(void);
void PhotoelectricScan(void);

#endif	// __PHOTOELECTRIC_H
/*-----------------------------------file of end------------------------------*/


