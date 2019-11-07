/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : BSP_USART.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-01-18               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __BSP_USART_H 
#define __BSP_USART_H 
 
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "BSP_DMA.h"
#include "stdio.h"	

/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef enum
{
  USART_1=1,
  USART_2=2,
  USART_3=3,
  UART_4 =4,
  UART_5 =5,
  USART_6=6,
  UART_7 =7,
  UART_8 =8,
  
}USARTx_e;
typedef struct
{
  USARTx_e USART_x;
  uint16_t crc;
  uint8_t *pUSARTSendBuff;
}USARTSend_t;

/*------------------------G L O B A L - M A C R O S --------------------------*/

/********USART1_Pin_define*************/
#define USART1_TX_GPIO_CLK      RCC_AHB1Periph_GPIOB
#define USART1_TX_GPIO_PORT     GPIOB
#define USART1_TX_Pin          	GPIO_Pin_6
#define USART1_TX_PINSOURCE			GPIO_PinSource6

#define USART1_RX_GPIO_CLK      RCC_AHB1Periph_GPIOB
#define USART1_RX_GPIO_PORT     GPIOB
#define USART1_RX_Pin           GPIO_Pin_7
#define USART1_RX_PINSOURCE			GPIO_PinSource7
/********USART1_Pin_define_END********/

/********USART2_Pin_define************/
#define USART2_TX_GPIO_CLK       RCC_AHB1Periph_GPIOD
#define USART2_TX_GPIO_PORT      GPIOD
#define USART2_TX_Pin          	 GPIO_Pin_5
#define USART2_TX_PINSOURCE			 GPIO_PinSource5

#define USART2_RX_GPIO_CLK       RCC_AHB1Periph_GPIOD
#define USART2_RX_GPIO_PORT      GPIOD
#define USART2_RX_Pin            GPIO_Pin_6
#define USART2_RX_PINSOURCE			 GPIO_PinSource6
/********USART2_Pin_define_END********/

/********USART3_Pin_define************/
#define USART3_TX_GPIO_CLK       RCC_AHB1Periph_GPIOD
#define USART3_TX_GPIO_PORT      GPIOD
#define USART3_TX_Pin          	 GPIO_Pin_8
#define USART3_TX_PINSOURCE			 GPIO_PinSource8

#define USART3_RX_GPIO_CLK       RCC_AHB1Periph_GPIOD
#define USART3_RX_GPIO_PORT      GPIOD
#define USART3_RX_Pin            GPIO_Pin_9
#define USART3_RX_PINSOURCE			 GPIO_PinSource9
/********USART3_Pin_define_END********/

/********UART4_Pin_define*************/
#define UART4_TX_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define UART4_TX_GPIO_PORT        GPIOA
#define UART4_TX_Pin          	 	GPIO_Pin_0
#define UART4_TX_PINSOURCE			  GPIO_PinSource0

#define UART4_RX_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define UART4_RX_GPIO_PORT        GPIOA
#define UART4_RX_Pin           		GPIO_Pin_1
#define UART4_RX_PINSOURCE			  GPIO_PinSource1
/********UART4_Pin_define_END********/
/********USART6_Pin_define***********/
#define USART6_TX_GPIO_CLK        RCC_AHB1Periph_GPIOC
#define USART6_TX_GPIO_PORT       GPIOC
#define USART6_TX_Pin          	 	GPIO_Pin_6
#define USART6_TX_PINSOURCE			  GPIO_PinSource6

#define USART6_RX_GPIO_CLK        RCC_AHB1Periph_GPIOC
#define USART6_RX_GPIO_PORT       GPIOC
#define USART6_RX_Pin           	GPIO_Pin_7
#define USART6_RX_PINSOURCE			  GPIO_PinSource7
/********USART6_Pin_define_END********/
/********USART6_Pin_define***********/
#define UART7_TX_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define UART7_TX_GPIO_PORT        GPIOE
#define UART7_TX_Pin          	 	GPIO_Pin_8
#define UART7_TX_PINSOURCE			  GPIO_PinSource8

#define UART7_RX_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define UART7_RX_GPIO_PORT        GPIOE
#define UART7_RX_Pin            	GPIO_Pin_7
#define UART7_RX_PINSOURCE			  GPIO_PinSource7
/********USART6_Pin_define_END********/
/********USART6_Pin_define***********/
#define UART8_TX_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define UART8_TX_GPIO_PORT        GPIOE
#define UART8_TX_Pin          	 	GPIO_Pin_1
#define UART8_TX_PINSOURCE			  GPIO_PinSource1

#define UART8_RX_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define UART8_RX_GPIO_PORT        GPIOE
#define UART8_RX_Pin            	GPIO_Pin_0
#define UART8_RX_PINSOURCE			  GPIO_PinSource0
/********USART6_Pin_define_END********/


#define USART_INIT(X, Baud) {\
	USART_InitTypeDef   USART_InitStructure;\
  GPIO_InitTypeDef GPIO_InitStructure;\
  if(X == 1)\
  {\
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);\
  }\
  else\
  {\
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART##X##, ENABLE);\
  }\
	RCC_AHB1PeriphClockCmd(USART##X##_TX_GPIO_CLK | USART##X##_RX_GPIO_CLK,ENABLE);\
			/* GPIO初始化 */\
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;\
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; \
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;\
		/* 配置Tx引脚  */\
	GPIO_InitStructure.GPIO_Pin = USART##X##_TX_Pin;\
	GPIO_Init(USART##X##_TX_GPIO_PORT, &GPIO_InitStructure);\
	/* 配置Rx引脚 */\
	GPIO_InitStructure.GPIO_Pin = USART##X##_RX_Pin;\
	GPIO_Init(USART##X##_RX_GPIO_PORT, &GPIO_InitStructure);\
	/* 连接 PXx 到 USARTx_Tx*/\
	GPIO_PinAFConfig(USART##X##_TX_GPIO_PORT, USART##X##_TX_PINSOURCE, GPIO_AF_USART##X##);\
	/*  连接 PXx 到 USARTx_Rx*/ \
	GPIO_PinAFConfig(USART##X##_RX_GPIO_PORT, USART##X##_RX_PINSOURCE, GPIO_AF_USART##X##);\
	USART_InitStructure.USART_BaudRate           = Baud; \
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;\
	USART_InitStructure.USART_Mode               = USART_Mode_Rx | USART_Mode_Tx;\
	USART_InitStructure.USART_Parity             = USART_Parity_No;\
	USART_InitStructure.USART_StopBits           = USART_StopBits_1;\
	USART_InitStructure.USART_WordLength         = USART_WordLength_8b;\
	USART_Init(USART##X##, &USART_InitStructure);\
	USART_DMACmd(USART##X##, USART_DMAReq_Tx, ENABLE);\
	/* 使能串口空闲中断 */\
	USART_ITConfig(USART##X##, USART_IT_IDLE, ENABLE);\
	USART_Cmd(USART##X##, ENABLE);}

/*----------------------G L O B A L - D E F I N E S---------------------------*/


/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/

void BSP_USART1_Init(uint32_t BaudRate);
void BSP_USART2_Init(uint32_t BaudRate);
void BSP_USART3_Init(uint32_t BaudRate);
void BSP_USART6_Init(uint32_t BaudRate);
void BSP_UART7_Init(uint32_t BaudRate);
void BSP_UART8_Init(uint32_t BaudRate);
void BSP_USART2_Half_Init(uint32_t BaudRate);
void USART_sendChar(USART_TypeDef* USARTx, char ch);

#endif	// __BSP_USART_H
/*----------------------------------FILE OF END-------------------------------*/





 

