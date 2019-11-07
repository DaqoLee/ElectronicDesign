/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : BSP_DMA.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-18               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "BSP_DMA.h" 
	
/*-------------------------- D E F I N E S -----------------------------------*/



/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/



/*------------------G L O B A L - F U N C T I O N S --------------------------*/

 /*------------------------------80 Chars Limit--------------------------------*/
	 /**
	 * @Data    2019-01-18 10:26
	 * @brief   USART1 RX DMA 配置，外设到内存(USART1->DR)
	 * @param   DMABuffAddr  DMA目标存储地址
	 * @retval  void
	 */
void DMA_USART1RxConfig(uint32_t DMABuffAddr, uint32_t buffsize)
{
	DMA_InitTypeDef DMA_InitStructure;

	/*开启DMA2时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	/* 复位初始化DMA数据流 */
	DMA_DeInit(USART1_RX_DMA_STREAM);
	/* 确保DMA数据流复位完成 */
	while(DMA_GetCmdStatus(USART1_RX_DMA_STREAM) != DISABLE)
	{

	}

	/*usart1 rx对应dma2，通道4，数据流2*/	
	DMA_InitStructure.DMA_Channel = USART1_RX_DMA_CHANNEL;  
	/*设置DMA源：串口数据寄存器地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_RX_ADDR;	 
	/*内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_Memory0BaseAddr = DMABuffAddr;
	/*方向：从外设到内存*/		
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
	DMA_InitStructure.DMA_BufferSize = buffsize;
	/*外设地址不增*/	    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	/*内存地址自增*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	/*外设数据单位*/	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/*内存数据单位 8bit*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	/*DMA模式：不断循环*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	/*优先级：中*/	
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;      
	/*禁用FIFO*/
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
	/*FIFO阈值*/      
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;    
	/*存储器突发传输 16 个节拍*/
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
	/*外设突发传输 1 个节拍*/
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    
	/*配置DMA2的数据流*/		   
	DMA_Init(USART1_RX_DMA_STREAM, &DMA_InitStructure);
	  
	/*使能DMA*/
	DMA_Cmd(USART1_RX_DMA_STREAM, ENABLE);
  
	/* 等待DMA数据流有效*/
	while(DMA_GetCmdStatus(USART1_RX_DMA_STREAM) != ENABLE)
	{

	}
	/*使能DMA接收*/ 
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE); 
  
}

 /*------------------------------80 Chars Limit--------------------------------*/
	 /**
	 * @Data    2019-01-18 10:26
	 * @brief   USART2 RX DMA 配置，外设到内存(USART1->DR)
	 * @param   DMABuffAddr  DMA目标存储地址
	 * @retval  void
	 */
void DMA_USART2RxConfig(uint32_t DMABuffAddr, uint32_t buffsize)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_DeInit(USART2_RX_DMA_STREAM);
	while(DMA_GetCmdStatus(USART2_RX_DMA_STREAM) != DISABLE)
	{

	}

	DMA_InitStructure.DMA_Channel = USART2_RX_DMA_CHANNEL;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_RX_ADDR;	 
	DMA_InitStructure.DMA_Memory0BaseAddr = DMABuffAddr;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	DMA_InitStructure.DMA_BufferSize = buffsize;    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;      
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;    
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    		   
	DMA_Init(USART2_RX_DMA_STREAM, &DMA_InitStructure);
	  
	DMA_Cmd(USART2_RX_DMA_STREAM, ENABLE);
  
	while(DMA_GetCmdStatus(USART2_RX_DMA_STREAM) != ENABLE)
	{

	} 
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE); 
}

 /*------------------------------80 Chars Limit--------------------------------*/
	 /**
	 * @Data    2019-01-18 10:26
	 * @brief   USART2 RX DMA 配置，外设到内存(USART1->DR)
	 * @param   DMABuffAddr  DMA目标存储地址
	 * @retval  void
	 */
void DMA_USART2TxConfig(uint32_t DMABuffAddr, uint32_t buffsize)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_DeInit(USART2_TX_DMA_STREAM);
	while(DMA_GetCmdStatus(USART2_TX_DMA_STREAM) != DISABLE)
	{

	}

	DMA_InitStructure.DMA_Channel = USART2_TX_DMA_CHANNEL;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_TX_ADDR;	 
	DMA_InitStructure.DMA_Memory0BaseAddr = DMABuffAddr;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;	
	DMA_InitStructure.DMA_BufferSize = buffsize;    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;      
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;    
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    		   
	DMA_Init(USART2_TX_DMA_STREAM, &DMA_InitStructure);
	  
	DMA_Cmd(USART2_TX_DMA_STREAM, ENABLE);
  
	while(DMA_GetCmdStatus(USART2_TX_DMA_STREAM) != ENABLE)
	{

	} 
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE); 
}
 /*------------------------------80 Chars Limit--------------------------------*/
	 /**
	 * @Data    2019-01-18 10:26
	 * @brief   USART3 RX DMA 配置，外设到内存(USART1->DR)
	 * @param   DMABuffAddr  DMA目标存储地址
	 * @retval  void
	 */
void DMA_USART3RxConfig(uint32_t DMABuffAddr, uint32_t buffsize)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_DeInit(USART3_RX_DMA_STREAM);
	while(DMA_GetCmdStatus(USART3_RX_DMA_STREAM) != DISABLE)
	{

	}

	DMA_InitStructure.DMA_Channel = USART3_RX_DMA_CHANNEL;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_RX_ADDR;	 
	DMA_InitStructure.DMA_Memory0BaseAddr = DMABuffAddr;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	DMA_InitStructure.DMA_BufferSize = buffsize;    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;      
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;    
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    	   
	DMA_Init(USART3_RX_DMA_STREAM, &DMA_InitStructure);
	  
	DMA_Cmd(USART3_RX_DMA_STREAM, ENABLE);
  
	while(DMA_GetCmdStatus(USART3_RX_DMA_STREAM) != ENABLE)
	{

	}
  
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE); 
}

 /*------------------------------80 Chars Limit--------------------------------*/
	 /**
	 * @Data    2019-01-18 10:26
	 * @brief   USART6 RX DMA 配置，外设到内存(USART1->DR)
	 * @param   DMABuffAddr  DMA目标存储地址
	 * @retval  void
	 */
void DMA_USART6RxConfig(uint32_t DMABuffAddr, uint32_t buffsize)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(USART6_RX_DMA_STREAM);
	while(DMA_GetCmdStatus(USART6_RX_DMA_STREAM) != DISABLE)
	{

	}

	DMA_InitStructure.DMA_Channel = USART6_RX_DMA_CHANNEL;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART6_RX_ADDR;	 
	DMA_InitStructure.DMA_Memory0BaseAddr = DMABuffAddr;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	DMA_InitStructure.DMA_BufferSize = buffsize;    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;      
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;    
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    	   
	DMA_Init(USART6_RX_DMA_STREAM, &DMA_InitStructure);
	  
	DMA_Cmd(USART6_RX_DMA_STREAM, ENABLE);
  
	while(DMA_GetCmdStatus(USART6_RX_DMA_STREAM) != ENABLE)
	{

	}
  
	USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE); 
}

 /*------------------------------80 Chars Limit--------------------------------*/
	 /**
	 * @Data    2019-01-18 10:26
	 * @brief   USART6 RX DMA 配置，外设到内存(USART1->DR)
	 * @param   DMABuffAddr  DMA目标存储地址
	 * @retval  void
	 */
void DMA_UART7RxConfig(uint32_t DMABuffAddr, uint32_t buffsize)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_DeInit(UART7_RX_DMA_STREAM);
	while(DMA_GetCmdStatus(UART7_RX_DMA_STREAM) != DISABLE)
	{

	}

	DMA_InitStructure.DMA_Channel = UART7_RX_DMA_CHANNEL;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = UART7_RX_ADDR;	 
	DMA_InitStructure.DMA_Memory0BaseAddr = DMABuffAddr;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	DMA_InitStructure.DMA_BufferSize = buffsize;    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;      
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;    
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    	   
	DMA_Init(UART7_RX_DMA_STREAM, &DMA_InitStructure);
	  
	DMA_Cmd(UART7_RX_DMA_STREAM, ENABLE);
  
	while(DMA_GetCmdStatus(UART7_RX_DMA_STREAM) != ENABLE)
	{

	}
  
	USART_DMACmd(UART7,USART_DMAReq_Rx,ENABLE); 
}

 /*------------------------------80 Chars Limit--------------------------------*/
	 /**
	 * @Data    2019-01-18 10:26
	 * @brief   USART6 RX DMA 配置，外设到内存(USART1->DR)
	 * @param   DMABuffAddr  DMA目标存储地址
	 * @retval  void
	 */
void DMA_UART8RxConfig(uint32_t DMABuffAddr, uint32_t buffsize)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_DeInit(UART8_RX_DMA_STREAM);
	while(DMA_GetCmdStatus(UART8_RX_DMA_STREAM) != DISABLE)
	{

	}

	DMA_InitStructure.DMA_Channel = UART8_RX_DMA_CHANNEL;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = UART8_RX_ADDR;	 
	DMA_InitStructure.DMA_Memory0BaseAddr = DMABuffAddr;	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	DMA_InitStructure.DMA_BufferSize = buffsize;    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;      
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;    
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;    	   
	DMA_Init(UART8_RX_DMA_STREAM, &DMA_InitStructure);
	  
	DMA_Cmd(UART8_RX_DMA_STREAM, ENABLE);
  
	while(DMA_GetCmdStatus(UART8_RX_DMA_STREAM) != ENABLE)
	{

	}
  
	USART_DMACmd(UART8,USART_DMAReq_Rx,ENABLE); 
}
/*---------------------L O C A L - F U N C T I O N S--------------------------*/



/*-----------------------------------FILE OF END------------------------------*/





