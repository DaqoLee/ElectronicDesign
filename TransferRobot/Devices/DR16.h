/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : DR16.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-01-08               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __DR16_H 
#define __DR16_H 
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "stm32f4xx.h"

/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef struct 
{
	uint8_t buff[20];
	int16_t ch1;	/*each ch value from -660 -- +660*/
	int16_t ch2;
	int16_t ch3;
	int16_t ch4;
	int16_t ch5;
	
	uint8_t switch_left;	
	uint8_t switch_right;
	
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
	
		uint8_t press_left;
		uint8_t press_right;
        
    uint8_t jumppress_left;
    uint8_t jumppress_right;
	}mouse;
	
	struct 
	{
/*******************************************************************************
   * Í¨µÀ:15 14   13   12   11   10   9   8   7   6     5     4   3   2   1
   *      V    C    X	   Z    G    F    R   E   Q  CTRL  SHIFT  D   A   S   W
*******************************************************************************/
		uint16_t key_code;              
    uint16_t jumpkey_code;         
	}keyBoard;
	
	void (*pDR16getMsg)(uint8_t *pDR16Buffer);
	
}DR16_t;

/*------------------------G L O B A L - M A C R O S --------------------------*/


/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern DR16_t DR16;

/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/


void DR16_Init(void);


#endif	// __DR16_H
/*----------------------------------FILE OF END-------------------------------*/



 

