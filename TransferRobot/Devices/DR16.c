/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : DR16.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-08               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "DR16.h" 
	
/*-------------------------- D E F I N E S -----------------------------------*/

DR16_t DR16;

/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

static void DR16_getMessage(uint8_t *pDR16Buffer);

/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 17:08
	* @brief   
	* @param   void
	* @retval  void
	*/
void DR16_Init(void)
{
	DR16.ch1=0;
	DR16.ch2=0;
	DR16.ch3=0;
	DR16.ch4=0;
	DR16.keyBoard.key_code=0;
	
	DR16.switch_left = 0;
	DR16.switch_right =0 ;
	
	DR16.mouse.x = 0;	
	DR16.mouse.y = 0;
	DR16.mouse.z =0;
	
	DR16.mouse.press_left 	= 0;	
	DR16.mouse.press_right 	=0;
	
	DR16.pDR16getMsg=DR16_getMessage;
}

/*---------------------L O C A L - F U N C T I O N S--------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 17:08
	* @brief   
	* @param   *pDR16Buffer
	* @retval  void
	*/
static void DR16_getMessage(uint8_t *pDR16Buffer)
{
	DR16.ch1 = (pDR16Buffer[0] | pDR16Buffer[1]<<8) & 0x07FF;
	DR16.ch1 -= 1024;
	DR16.ch2 = (pDR16Buffer[1]>>3 | pDR16Buffer[2]<<5 ) & 0x07FF;
	DR16.ch2 -= 1024;
	DR16.ch3 = (pDR16Buffer[2]>>6 | pDR16Buffer[3]<<2 | pDR16Buffer[4]<<10) & 0x07FF;
	DR16.ch3 -= 1024;
	DR16.ch4 = (pDR16Buffer[4]>>1 | pDR16Buffer[5]<<7) & 0x07FF;		
	DR16.ch4 -= 1024;
	
	DR16.switch_left = ( (pDR16Buffer[5] >> 4)& 0x000C ) >> 2;
	DR16.switch_right =  (pDR16Buffer[5] >> 4)& 0x0003 ;
	
	DR16.mouse.x = pDR16Buffer[6] | (pDR16Buffer[7] << 8);	
	DR16.mouse.y = pDR16Buffer[8] | (pDR16Buffer[9] << 8);
	DR16.mouse.z = pDR16Buffer[10]| (pDR16Buffer[11] << 8);
	
	DR16.mouse.press_left 	= pDR16Buffer[12];	
	DR16.mouse.press_right 	= pDR16Buffer[13];
	
	DR16.keyBoard.key_code 	= pDR16Buffer[14] | pDR16Buffer[15] << 8; 
	DR16.ch5 = (pDR16Buffer[16] | pDR16Buffer[17]<<8) & 0x07FF;
	DR16.ch5 -= 1024;
}


/*-----------------------------------FILE OF END------------------------------*/



