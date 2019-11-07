/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : LED.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-08               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
 
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "LED.h" 
  
/*-------------------------- D E F I N E S -----------------------------------*/



/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-01-08 17:03
  * @brief   LED≥ı ºªØ
  * @param   void
  * @retval  void
  */
void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_AHB1PeriphClockCmd(LED_R_GPIO_CLK|LED0_GPIO_CLK|LASER_GPIO_CLK, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;
  GPIO_InitStructure.GPIO_Pin =LED0_PIN|LED1_PIN|LED2_PIN|LED3_PIN\
                              |LED4_PIN|LED5_PIN|LED6_PIN;
  GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;
  GPIO_InitStructure.GPIO_Pin =LED_R_PIN|LED_G_PIN;
  GPIO_Init(LED_G_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;
  GPIO_InitStructure.GPIO_Pin =LASER_PIN;
  GPIO_Init(LASER_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_WriteBit(LASER_GPIO_PORT,LASER_PIN,Bit_RESET);

	GPIO_WriteBit(LED0_GPIO_PORT,LED0_PIN|LED1_PIN|LED2_PIN|LED3_PIN\
                              |LED4_PIN|LED5_PIN|LED6_PIN,Bit_SET);

	GPIO_WriteBit(LED_R_GPIO_PORT,LED_R_PIN|LED_G_PIN,Bit_SET);
															 
}

/*------------------G L O B A L - F U N C T I O N S --------------------------*/



/*---------------------L O C A L - F U N C T I O N S--------------------------*/



/*-----------------------------------FILE OF END------------------------------*/







