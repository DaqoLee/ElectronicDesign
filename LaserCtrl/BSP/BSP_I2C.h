/**
  ************************************* Copyright ******************************   
  * (C) Copyright 2018,Daqo Lee,China, GCU.
  *		All Rights Reserved
  *
  * By(ZHE WOLF TEAM OF GCU ROBOT)
  * https://github.com/GCUWildwolfteam
  *
  * FileName   : BSP_I2C.h   
  * Version    : v1.0		
  * Author     : Daqo Lee			
  * Date       : 2018-12-16         
  * Description:    
  * Function List:  
  	1. ....
  	   <version>: 		
  <modify staff>:
  		  <data>:
   <description>:  
  	2. ...
  ******************************************************************************
 */



#ifndef __BSP__I2C_H_
#define __BSP__I2C_H_
#include "stm32f4xx.h"

/******************************************************************************/
#define I2C2_SCL_GPIO_CLK           RCC_AHB1Periph_GPIOF
#define I2C2_SCL_GPIO_PORT          GPIOF
#define I2C2_SCL_Pin           	  	GPIO_Pin_1
#define I2C2_SCL_PINSOURCE			    GPIO_PinSource1

#define I2C2_SDA_GPIO_CLK           RCC_AHB1Periph_GPIOF
#define I2C2_SDA_GPIO_PORT          GPIOF
#define I2C2_SDA_Pin           	  	GPIO_Pin_0
#define I2C2_SDA_PINSOURCE			    GPIO_PinSource0

void BSP_I2C2_Init(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);

#endif


 

