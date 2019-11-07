/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : Task_Can.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-02-20               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __TASK_CAN_H 
#define __TASK_CAN_H 

  
/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "BSP_CAN.h"

/*-------------------------G L O B A L - T Y P E S----------------------------*/

extern QueueHandle_t xCanSendQueue;
extern QueueHandle_t xCan1RxQueue ;
extern QueueHandle_t xCan2RxQueue ;

/*------------------------G L O B A L - M A C R O S --------------------------*/



/*----------------------G L O B A L - D E F I N E S---------------------------*/



/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/

void CanTaskCreate(void);


#endif	// __TASK_CAN_H
/*----------------------------------FILE OF END-------------------------------*/

 

 

