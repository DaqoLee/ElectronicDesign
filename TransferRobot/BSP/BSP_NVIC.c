/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : BSP_NVIC.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-18               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
#include "BSP_NVIC.h"

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 13:47
	* @brief   中断优先级配置
	* @param   void
	* @retval  void
	*/
void BSP_NVIC_Init(void)
{
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	/*CAN1*/
	NVIC_InitStructure.NVIC_IRQChannel					        	=	CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd				        	=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		    	=	0;
	NVIC_Init(&NVIC_InitStructure);
	
	/*CAN1*/
	NVIC_InitStructure.NVIC_IRQChannel					        	=	CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd				        	=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	9;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			    =	0;
	NVIC_Init(&NVIC_InitStructure);
	
  /*USART1*/
	NVIC_InitStructure.NVIC_IRQChannel						        =	USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd				        	=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		     	=	0;
	NVIC_Init(&NVIC_InitStructure);
	
  /*USART2*/
	NVIC_InitStructure.NVIC_IRQChannel						        =	USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd				        	=	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		     	=	0;
	NVIC_Init(&NVIC_InitStructure);	
	
  /*USART3*/
	NVIC_InitStructure.NVIC_IRQChannel					        	=	USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					        =	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			    =	0;
	NVIC_Init(&NVIC_InitStructure);
	
  /*UART4*/
	NVIC_InitStructure.NVIC_IRQChannel					        	=	UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					        =	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			    =	0;
	NVIC_Init(&NVIC_InitStructure);
	
  /*UART5*/
	NVIC_InitStructure.NVIC_IRQChannel					        	=	UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					        =	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		    	=	0;
	NVIC_Init(&NVIC_InitStructure);

  /*USART6*/
	NVIC_InitStructure.NVIC_IRQChannel					        	=	USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					        =	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	11;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			    =	0;
	NVIC_Init(&NVIC_InitStructure);
	
	  /*UART7*/
	NVIC_InitStructure.NVIC_IRQChannel					        	=	UART7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					        =	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			    =	0;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel					        	=	TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd					        =	ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		    	=	0;
	NVIC_Init(&NVIC_InitStructure);

}
/*-----------------------------------FILE OF END------------------------------*/
