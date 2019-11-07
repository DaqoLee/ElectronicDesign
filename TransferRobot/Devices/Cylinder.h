/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : Cylinder.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-01-08               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __CYLINDER_H 
#define __CYLINDER_H 

#pragma anon_unions
/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"

/*----------------------G L O B A L - D E F I N E S---------------------------*/
#define GAS8_SEND_ID    0x500

#define GAS_POS_1      0x01
#define GAS_POS_2      0x02
#define GAS_POS_3      0x04
#define GAS_POS_4      0x08
#define GAS_POS_5      0x10
#define GAS_POS_6      0x20
#define GAS_POS_7      0x40
#define GAS_POS_8      0x80
#define GAS_POS_ALL    0xFF




/*-------------------------G L O B A L - T Y P E S----------------------------*/


typedef struct{
  union {
		uint8_t dataPack;
		struct{
			uint8_t GAS_1:1;
			uint8_t GAS_2:1;
			uint8_t GAS_3:1;
			uint8_t GAS_4:1;
			uint8_t GAS_5:1;
			uint8_t GAS_6:1;
			uint8_t GAS_7:1;
			uint8_t GAS_8:1;
		};
	}data;
  
}GAS8_t;

/*------------------------G L O B A L - M A C R O S --------------------------*/

extern GAS8_t GAS8s[8];

/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/


void GAS8_setBits(uint8_t gasID, uint8_t pos);
void GAS8_resetBits(uint8_t gasID, uint8_t pos);
void GAS8_toggleBits(uint8_t gasID, uint8_t pos);
void GAS8_writeBits(uint8_t gasID, uint8_t pos, uint8_t status);

void GAS8_writeData(uint8_t gasID, uint8_t data);

#endif	// __CYLINDER_H
/*----------------------------------FILE OF END-------------------------------*/


