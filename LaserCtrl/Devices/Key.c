/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|--FileName    : Key.c                                              
|--Version     : v1.0                                                          
|--Author      : Daqo Lee                                                     
|--Date        : 2019-03-18             
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                     
|-----------------------------declaration of end-----------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Key.h" 
	
/*-------------------------- D E F I N E S -----------------------------------*/

uint8_t KeyStatus=0;

/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/



/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/* --------------------------------------------------------------------- */
/*
 * @Name    KEY_Init
 * @brief  
 * @param   None
 * @retval
 * @author  Daqo Lee 
 * @Data    2019-01-08
 */
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(KEY_GPIO_CLK, ENABLE); 		
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;   
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	GPIO_InitStructure.GPIO_Pin   = KEY_PIN;	 
	
	GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
}

void KeyScan(void)
{
	
	KeyStatus=GPIO_ReadInputDataBit (KEY_GPIO_PORT,KEY_PIN);
}

/*---------------------L O C A L - F U N C T I O N S--------------------------*/



/*-----------------------------------file of end------------------------------*/


