/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : BSP_USART.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-18               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "BSP_USART.h" 
	
/*-------------------------- D E F I N E S -----------------------------------*/


/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 16:32
	* @brief   USART1初始化
	* @param   BaudRate 波特率
	* @retval  void
	*/
//void BSP_USART_Init(uint8_t USART_X, uint32_t BaudRate)
//{
//	USART_InitTypeDef   USART_InitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//	if(USART_X == 1)\
//  {
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);\
//  }
//  else
//  {
//   // RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART##X##, ENABLE);
//  }
//	
////	USART_INIT(USART1,BaudRate);
//}

/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 16:32
	* @brief   USART1初始化
	* @param   BaudRate 波特率
	* @retval  void
	*/
void BSP_USART1_Init(uint32_t BaudRate)
{
	USART_InitTypeDef   USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_AHB1PeriphClockCmd(USART1_TX_GPIO_CLK | USART1_RX_GPIO_CLK,ENABLE);
			/* GPIO初始化 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		/* 配置Tx引脚  */
	GPIO_InitStructure.GPIO_Pin = USART1_TX_Pin;  
	GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置Rx引脚 */
	GPIO_InitStructure.GPIO_Pin = USART1_RX_Pin;
	GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(USART1_TX_GPIO_PORT, USART1_TX_PINSOURCE, GPIO_AF_USART1);

	/*  连接 PXx 到 USARTx_Rx*/
	GPIO_PinAFConfig(USART1_RX_GPIO_PORT, USART1_RX_PINSOURCE, GPIO_AF_USART1);

	USART_InitStructure.USART_BaudRate           = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode               = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity             = USART_Parity_No;
	USART_InitStructure.USART_StopBits           = USART_StopBits_1;
	USART_InitStructure.USART_WordLength         = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	/* 使能串口空闲中断 */
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 16:32
	* @brief   USART2初始化
	* @param   BaudRate 波特率
	* @retval  void
	*/
void BSP_USART2_Init(uint32_t BaudRate)
{
	USART_InitTypeDef   USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	RCC_AHB1PeriphClockCmd(USART2_TX_GPIO_CLK | USART2_RX_GPIO_CLK,ENABLE);
			/* GPIO初始化 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		/* 配置Tx引脚  */
	GPIO_InitStructure.GPIO_Pin = USART2_TX_Pin;  
	GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);
	
	/* 配置Rx引脚 */
	GPIO_InitStructure.GPIO_Pin = USART2_RX_Pin;
	GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(USART2_TX_GPIO_PORT, USART2_TX_PINSOURCE, GPIO_AF_USART2);
	/*  连接 PXx 到 USARTx_Rx*/
	GPIO_PinAFConfig(USART2_RX_GPIO_PORT, USART2_RX_PINSOURCE, GPIO_AF_USART2);

	USART_InitStructure.USART_BaudRate           = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode               = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity             = USART_Parity_No;
	USART_InitStructure.USART_StopBits           = USART_StopBits_1;
	USART_InitStructure.USART_WordLength         = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);

	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	/* 使能串口空闲中断 */
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART2, ENABLE);
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 16:32
	* @brief   USART2初始化
	* @param   BaudRate 波特率
	* @retval  void
	*/
void BSP_USART2_Half_Init(uint32_t BaudRate)
{
	USART_InitTypeDef   USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	RCC_AHB1PeriphClockCmd(USART2_TX_GPIO_CLK | USART2_RX_GPIO_CLK,ENABLE);
			/* GPIO初始化 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		/* 配置Tx引脚  */
	GPIO_InitStructure.GPIO_Pin = USART2_TX_Pin;  
	GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(USART2_TX_GPIO_PORT, USART2_TX_PINSOURCE, GPIO_AF_USART2);

	USART_InitStructure.USART_BaudRate           = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode               = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity             = USART_Parity_No;
	USART_InitStructure.USART_StopBits           = USART_StopBits_1;
	USART_InitStructure.USART_WordLength         = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);

//	USART2->CR3&=0<<5;//清SCEN
//	USART2->CR3&=0<<1; //清IREN
//	USART2->CR2&=0<<11;//清CLKEN
//	USART2->CR2&=0<<14; //清LINEN
  

	//DMA_USART2TxConfig((uint32_t)Usart2SendBuffer,9);
	
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	/* 使能串口空闲中断 */
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_HalfDuplexCmd( USART2, ENABLE);
  USART_ClearFlag(USART2, USART_FLAG_TC);  
}
void USART_Half_Configuration(void)  
{  
	GPIO_InitTypeDef GPIO_InitStructure;  
	USART_InitTypeDef USART_InitStructure;   

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1);  

	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);  


	USART_InitStructure.USART_BaudRate = 115200;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  
	USART_InitStructure.USART_Parity = USART_Parity_No;  
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
	USART_Init(USART1,&USART_InitStructure);  

	USART_HalfDuplexCmd(USART1, ENABLE);  

	USART_Cmd(USART1,ENABLE);  
	USART_ClearFlag(USART1, USART_FLAG_TC);  
}
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 16:32
	* @brief   USART3初始化
	* @param   BaudRate 波特率
	* @retval  void
	*/
void BSP_USART3_Init(uint32_t BaudRate)
{
	USART_InitTypeDef   USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(USART3_TX_GPIO_CLK | USART3_RX_GPIO_CLK,ENABLE);
			/* GPIO初始化 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		/* 配置Tx引脚  */
	GPIO_InitStructure.GPIO_Pin = USART3_TX_Pin;  
	GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置Rx引脚 */
	GPIO_InitStructure.GPIO_Pin = USART3_RX_Pin;
	GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(USART3_TX_GPIO_PORT, USART3_TX_PINSOURCE, GPIO_AF_USART3);

	/*  连接 PXx 到 USARTx_Rx*/
	GPIO_PinAFConfig(USART3_RX_GPIO_PORT, USART3_RX_PINSOURCE, GPIO_AF_USART3);
	
	USART_InitStructure.USART_BaudRate           = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode               = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity             = USART_Parity_No;
	USART_InitStructure.USART_StopBits           = USART_StopBits_1;
	USART_InitStructure.USART_WordLength         = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);

	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	/* 使能串口空闲中断 */
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART3, ENABLE);
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 16:32
	* @brief   USART3初始化
	* @param   BaudRate 波特率
	* @retval  void
	*/
void BSP_USART6_Init(uint32_t BaudRate)
{
	USART_InitTypeDef   USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(USART6_TX_GPIO_CLK | USART6_RX_GPIO_CLK,ENABLE);
	/* GPIO初始化 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	/* 配置Tx引脚  */
	GPIO_InitStructure.GPIO_Pin = USART6_TX_Pin;  
	GPIO_Init(USART6_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置Rx引脚 */
	GPIO_InitStructure.GPIO_Pin = USART6_RX_Pin;
	GPIO_Init(USART6_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(USART6_TX_GPIO_PORT, USART6_TX_PINSOURCE, GPIO_AF_USART6);
	/*  连接 PXx 到 USARTx_Rx*/
	GPIO_PinAFConfig(USART6_RX_GPIO_PORT, USART6_RX_PINSOURCE, GPIO_AF_USART6);
	
	USART_InitStructure.USART_BaudRate           = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode               = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity             = USART_Parity_No;
	USART_InitStructure.USART_StopBits           = USART_StopBits_1;
	USART_InitStructure.USART_WordLength         = USART_WordLength_8b;
	USART_Init(USART6, &USART_InitStructure);
	
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
	/* 使能串口空闲中断 */
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART6, ENABLE);
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 16:32
	* @brief   USART3初始化
	* @param   BaudRate 波特率
	* @retval  void
	*/
void BSP_UART7_Init(uint32_t BaudRate)
{
	USART_InitTypeDef   USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(UART7_TX_GPIO_CLK | UART7_RX_GPIO_CLK,ENABLE);
			/* GPIO初始化 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		/* 配置Tx引脚  */
	GPIO_InitStructure.GPIO_Pin = UART7_TX_Pin;  
	GPIO_Init(UART7_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置Rx引脚 */
	GPIO_InitStructure.GPIO_Pin = UART7_RX_Pin;
	GPIO_Init(UART7_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(UART7_TX_GPIO_PORT, UART7_TX_PINSOURCE, GPIO_AF_UART7);

	/*  连接 PXx 到 USARTx_Rx*/
	GPIO_PinAFConfig(UART7_RX_GPIO_PORT, UART7_RX_PINSOURCE, GPIO_AF_UART7);
	
	USART_InitStructure.USART_BaudRate           = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode               = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity             = USART_Parity_No;
	USART_InitStructure.USART_StopBits           = USART_StopBits_1;
	USART_InitStructure.USART_WordLength         = USART_WordLength_8b;
	USART_Init(UART7, &USART_InitStructure);

	USART_DMACmd(UART7, USART_DMAReq_Tx, ENABLE);
	/* 使能串口空闲中断 */
	USART_ITConfig(UART7, USART_IT_IDLE, ENABLE);
	USART_Cmd(UART7, ENABLE);
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 16:32
	* @brief   USART3初始化
	* @param   BaudRate 波特率
	* @retval  void
	*/
void BSP_UART8_Init(uint32_t BaudRate)
{
	USART_InitTypeDef   USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(UART8_TX_GPIO_CLK | UART8_RX_GPIO_CLK,ENABLE);
			/* GPIO初始化 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		/* 配置Tx引脚  */
	GPIO_InitStructure.GPIO_Pin = UART8_TX_Pin;  
	GPIO_Init(UART8_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置Rx引脚 */
	GPIO_InitStructure.GPIO_Pin = UART8_RX_Pin;
	GPIO_Init(UART8_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(UART8_TX_GPIO_PORT, UART8_TX_PINSOURCE, GPIO_AF_UART8);

	/*  连接 PXx 到 USARTx_Rx*/
	GPIO_PinAFConfig(UART8_RX_GPIO_PORT, UART8_RX_PINSOURCE, GPIO_AF_UART8);
	
	USART_InitStructure.USART_BaudRate           = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode               = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity             = USART_Parity_No;
	USART_InitStructure.USART_StopBits           = USART_StopBits_1;
	USART_InitStructure.USART_WordLength         = USART_WordLength_8b;
	USART_Init(UART8, &USART_InitStructure);

	USART_DMACmd(UART8, USART_DMAReq_Tx, ENABLE);
	/* 使能串口空闲中断 */
	USART_ITConfig(UART8, USART_IT_IDLE, ENABLE);
	USART_Cmd(UART8, ENABLE);
}


/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 16:32
	* @brief   串口发送一个字符
	* @param   USARTx 串口号，ch:要发送的字符
	* @retval  void
	*/
void USART_sendChar(USART_TypeDef* USARTx, char ch)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(USARTx, (uint8_t) ch);
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);	
}



#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
	USART2->DR = (u8) ch;      
	return ch;
}
#endif 

/*---------------------L O C A L - F U N C T I O N S--------------------------*/



/*-----------------------------------FILE OF END------------------------------*/





