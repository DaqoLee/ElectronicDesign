/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|--FileName    : Photoelectric.c                                              
|--Version     : v1.0                                                          
|--Author      : Daqo Lee                                                     
|--Date        : 2019-03-18             
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                     
|-----------------------------declaration of end-------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Photoelectric.h" 
  
/*-------------------------- D E F I N E S -----------------------------------*/

PhoFlg_t PhoFlg;/*光电开关标志位*/
/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/



/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/**
  * @Data    2019-03-18 10:40
  * @brief   
  * @param   void
  * @retval  void
  */
void Photoelectric_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(LF_GPIO_CLK, ENABLE); 		

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Pin   = LF_PIN|LH_PIN|RF_PIN|RH_PIN|CLAMP_PIN|SLOPE_PIN|SDOP_PIN;	 

	GPIO_Init(LF_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @Data    2019-03-18 10:58
  * @brief   
  * @param   void
  * @retval  void
  */
  void PhotoelectricScan(void)
  {
    PhoFlg.LF=GPIO_ReadInputDataBit(LF_GPIO_PORT,LF_PIN);
    PhoFlg.LH=GPIO_ReadInputDataBit(LH_GPIO_PORT,LH_PIN);
    PhoFlg.RF=GPIO_ReadInputDataBit(RF_GPIO_PORT,RF_PIN);
    PhoFlg.RH=GPIO_ReadInputDataBit(RH_GPIO_PORT,RH_PIN);
		PhoFlg.CLAMP=GPIO_ReadInputDataBit(CLAMP_GPIO_PORT,CLAMP_PIN);
		PhoFlg.SLOPE=GPIO_ReadInputDataBit(SLOPE_GPIO_PORT,SLOPE_PIN);
		PhoFlg.SDOP=GPIO_ReadInputDataBit(SDOP_GPIO_PORT,SDOP_PIN);
  }
/*---------------------L O C A L - F U N C T I O N S--------------------------*/



/*-----------------------------------file of end------------------------------*/


