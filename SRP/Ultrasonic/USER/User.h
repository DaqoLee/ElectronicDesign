/**
  ************************************* Copyright ******************************   
  * (C) Copyright 2018,Daqo Lee,China, GCU.
  *		All Rights Reserved
  *
  * By(ZHE WOLF TEAM OF GCU ROBOT)
  * https://github.com/GCUWildwolfteam
  *
  * FileName   : User.h   
  * Version    : v1.0		
  * Author     : Daqo Lee			
  * Date       : 2018-11-04         
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



#ifndef __USER_H_
#define __USER_H_
#include "usart.h"
#include "Ultrasonic.h"
#include "RFID_RC522.h"

void Ultrasonic_Transmit(uint8_t Length,uint16_t Data1,uint16_t Data2,uint16_t Data3,uint16_t Data4,\
	               uint16_t Data5,uint16_t Data6,uint16_t Data7,uint16_t Data8);
void RFID_RC522_Transmit(uint8_t Length,char *str);
#endif


 

