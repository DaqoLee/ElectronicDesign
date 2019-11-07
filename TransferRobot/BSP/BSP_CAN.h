/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : BSP_CAN.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-01-18               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __BSP_CAN_H 
#define __BSP_CAN_H 
/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef struct
{
  uint8_t     CANx;               /*1：CAN1  2：CAN2*/
  CanTxMsg    SendCanTxMsg;       /*要发送的数据缓存*/
}CanSend_t;

/*------------------------G L O B A L - M A C R O S --------------------------*/

/********CAN1_Pin_define**********/
#define CAN1_RX_GPIO_CLK          	RCC_AHB1Periph_GPIOD
#define CAN1_RX_GPIO_PORT           GPIOD
#define CAN1_RX_Pin           		  GPIO_Pin_0
#define CAN1_RX_PINSOURCE		      	GPIO_PinSource0

#define CAN1_TX_GPIO_CLK          	RCC_AHB1Periph_GPIOD
#define CAN1_TX_GPIO_PORT           GPIOD
#define CAN1_TX_Pin          	     	GPIO_Pin_1
#define CAN1_TX_PINSOURCE		      	GPIO_PinSource1

/********CAN1_Pin_define*********/
#define CAN2_RX_GPIO_CLK          	RCC_AHB1Periph_GPIOB
#define CAN2_RX_GPIO_PORT           GPIOB
#define CAN2_RX_Pin           	  	GPIO_Pin_12
#define CAN2_RX_PINSOURCE			      GPIO_PinSource12

#define CAN2_TX_GPIO_CLK          	RCC_AHB1Periph_GPIOB
#define CAN2_TX_GPIO_PORT           GPIOB
#define CAN2_TX_Pin          	 	    GPIO_Pin_13
#define CAN2_TX_PINSOURCE		      	GPIO_PinSource13

/*----------------------G L O B A L - D E F I N E S---------------------------*/



/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/

void BSP_CAN1_Init(void);
void BSP_CAN2_Init(void);

#endif	// __BSP_CAN_H
/*----------------------------------FILE OF END-------------------------------*/





 

