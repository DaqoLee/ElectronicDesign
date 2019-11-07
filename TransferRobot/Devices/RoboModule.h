/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|--FileName    : RoboModule.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                         
|--Date        : 2019-03-19               
|--Libsupports : 
|--Description : STM32F4xx_DFP ( 2.9.0)                                                      
|-----------------------------declaration of end-------------------------------|
 **/
#ifndef __ROBOMODULE_H 
#define __ROBOMODULE_H 


/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"

/*-------------------------G L O B A L - T Y P E S----------------------------*/



/*------------------------G L O B A L - M A C R O S --------------------------*/

#define OpenLoop_Mode                       0x01
#define Current_Mode                        0x02
#define Velocity_Mode                       0x03
#define Position_Mode                       0x04
#define Velocity_Position_Mode              0x05
#define Current_Velocity_Mode               0x06
#define Current_Position_Mode               0x07
#define Current_Velocity_Position_Mode      0x08


/*----------------------G L O B A L - D E F I N E S---------------------------*/



/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/
void RoboModule_Init(void);
void RoboModule_DRV_Reset(uint8_t Group,uint8_t Number);
void RoboModule_DRV_Mode_Choice(uint8_t Group,uint8_t Number,uint8_t Mode);
void RoboModule_DRV_OpenLoop_Mode(uint8_t Group,uint8_t Number,
	                                                  int16_t Temp_PWM);
void RoboModule_DRV_Current_Mode(uint8_t Group,uint8_t Number,
	                                   int16_t Temp_PWM,int16_t Temp_Current);
void RoboModule_DRV_Velocity_Mode(uint8_t Group,uint8_t Number,
	                                    int16_t Temp_PWM,int16_t Temp_Velocity);
void RoboModule_DRV_Position_Mode(uint8_t Group,uint8_t Number,
	                                      int16_t Temp_PWM,uint32_t Temp_Position);
void RoboModule_DRV_Velocity_Position_Mode(uint8_t Group,uint8_t Number,
	                int16_t Temp_PWM,int16_t Temp_Velocity,uint32_t Temp_Position);
void RoboModule_DRV_Current_Velocity_Mode(uint8_t Group,uint8_t Number,
	                                  int16_t Temp_Current,int16_t Temp_Velocity);
void RoboModule_DRV_Current_Position_Mode(uint8_t Group,uint8_t Number,
	                                   int16_t Temp_Current,int32_t Temp_Position);
void RoboModule_DRV_Current_Velocity_Position_Mode(uint8_t Group,uint8_t Number,
	             int16_t Temp_Current,int16_t Temp_Velocity,int32_t Temp_Position);
void RoboModule_DRV_Config(uint8_t Group,uint8_t Number, 
	                         uint8_t Temp_Time,uint8_t Ctl1_Ctl2);
void RoboModule_DRV_Online_Check(uint8_t Group,uint8_t Number);
#endif	// __ROBOMODULE_H
/*-----------------------------------file of end------------------------------*/








 

