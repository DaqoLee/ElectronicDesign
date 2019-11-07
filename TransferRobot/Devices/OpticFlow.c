/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : OpticFlow.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-03-14               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "OpticFlow.h" 
#include "BSP_USART.h"  
/*-------------------------- D E F I N E S -----------------------------------*/

OpticFlow_t OpticFlow;

/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/



/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-03-14 10:16
  * @brief   
  * @param   void
  * @retval  void
  */
  void OpticFlow_getMassage(uint8_t *OFbuffer)
  {
    for(uint8_t i=0;i<10;i++)
    {
      if(OFbuffer[i] == 0xFE && OFbuffer[i+8] == 0xAA)
      {
//        if(((OFbuffer[i+2]+OFbuffer[i+3]+OFbuffer[i+4]+OFbuffer[i+5]) & 0xFF) 
//             == OFbuffer[i+6])
          {
            OpticFlow.DeltaX = OFbuffer[i+2] | (OFbuffer[i+3]<<8);
            OpticFlow.DeltaY = OFbuffer[i+4] | (OFbuffer[i+5]<<6);  
          }
				break;
      }
    }
  }

/*---------------------L O C A L - F U N C T I O N S--------------------------*/



/*-----------------------------------FILE OF END------------------------------*/


