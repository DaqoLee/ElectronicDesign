/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Encoder.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-08               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Encoder.h" 
#include "string.h" 
#include "DR16.h"
/*-------------------------- D E F I N E S -----------------------------------*/

Posture_t Posture;
/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

  /**
  * @Data    2019-01-08 14:28
  * @brief   
  * @param   *EncoderBuf 全场定位数据
  * @retval  void 
  */

void Posture_getMessage(uint8_t *EncoderBuf)
{
	if(EncoderBuf[0]==0x0D&&EncoderBuf[1]==0x0A&&\
	   EncoderBuf[26]==0x0A&&EncoderBuf[27]==0x0D)
	{
		memcpy(Posture.DataBuff,&EncoderBuf[2],sizeof(uint8_t[24]));
	}
}



/*------------------G L O B A L - F U N C T I O N S --------------------------*/



/*---------------------L O C A L - F U N C T I O N S--------------------------*/



/*-----------------------------------FILE OF END------------------------------*/






/*------------------------------80 Chars Limit--------------------------------*/

