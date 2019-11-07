/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : BSP_CAN.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-20               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "BSP_CAN.h" 
	
/*-------------------------- D E F I N E S -----------------------------------*/



/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/



/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 13:33
	* @brief   CAN2初始化
	* @param   void
	* @retval  void
	*/
void BSP_CAN1_Init(void)
{

	CAN_InitTypeDef     CAN_InitStructure;
	CAN_FilterInitTypeDef   CAN_FilterInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(CAN1_RX_GPIO_CLK | CAN1_TX_GPIO_CLK, ENABLE);
	/* GPIO初始化 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  /* 配置Tx引脚  */
	GPIO_InitStructure.GPIO_Pin = CAN1_TX_Pin;  
	GPIO_Init(CAN1_TX_GPIO_PORT, &GPIO_InitStructure);

	/* 配置Rx引脚*/
	GPIO_InitStructure.GPIO_Pin = CAN1_RX_Pin;
	GPIO_Init(CAN1_RX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_PinAFConfig(CAN1_TX_GPIO_PORT, CAN1_TX_PINSOURCE, GPIO_AF_CAN1);
	GPIO_PinAFConfig(CAN1_RX_GPIO_PORT, CAN1_RX_PINSOURCE, GPIO_AF_CAN1);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  /* -------- CAN初始化配置  --------- */
	CAN_InitStructure.CAN_ABOM      =   ENABLE;         
	CAN_InitStructure.CAN_AWUM      =   DISABLE;        
	CAN_InitStructure.CAN_NART      =   DISABLE;         
	CAN_InitStructure.CAN_TTCM      =   DISABLE;				 
	CAN_InitStructure.CAN_TXFP      =   DISABLE;				 
	CAN_InitStructure.CAN_RFLM      =   DISABLE;			  
	CAN_InitStructure.CAN_Mode      =   CAN_Mode_Normal;
	
 
	CAN_InitStructure.CAN_SJW       =   CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1       =   CAN_BS1_3tq;     
	CAN_InitStructure.CAN_BS2       =   CAN_BS2_5tq;   
	
 
	CAN_InitStructure.CAN_Prescaler =   5;
	CAN_Init(CAN1, &CAN_InitStructure);


	
/*********************CAN筛选器初始化******************************************/
	CAN_FilterInitStructure.CAN_FilterNumber        =   0;
	CAN_FilterInitStructure.CAN_FilterMode          =   CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale         =   CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=   CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation    =   ENABLE; 
	
	CAN_FilterInitStructure.CAN_FilterIdHigh        =   0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow         =   0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh    =   0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow     =   0x0000;
	
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* FIFO0 message pending interrupt */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-18 13:33
	* @brief   CAN2初始化
	* @param   void
	* @retval  void
	*/
void BSP_CAN2_Init(void)
{
	CAN_InitTypeDef     CAN_InitStructure;
	CAN_FilterInitTypeDef   CAN_FilterInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(CAN2_RX_GPIO_CLK | CAN2_TX_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	
	GPIO_InitStructure.GPIO_Pin = CAN2_TX_Pin;  
	GPIO_Init(CAN2_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = CAN2_RX_Pin;
	GPIO_Init(CAN2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(CAN2_TX_GPIO_PORT, CAN2_TX_PINSOURCE, GPIO_AF_CAN2);
	GPIO_PinAFConfig(CAN2_RX_GPIO_PORT, CAN2_RX_PINSOURCE, GPIO_AF_CAN2);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	
	CAN_InitStructure.CAN_ABOM      =   ENABLE;         
	CAN_InitStructure.CAN_AWUM      =   DISABLE;        
	CAN_InitStructure.CAN_NART      =   DISABLE;        
	CAN_InitStructure.CAN_TTCM      =   DISABLE;				 
	CAN_InitStructure.CAN_TXFP      =   DISABLE;				 
	CAN_InitStructure.CAN_RFLM      =   DISABLE;			  
	CAN_InitStructure.CAN_Mode      =   CAN_Mode_Normal; 

	CAN_InitStructure.CAN_SJW       =   CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1       =   CAN_BS1_3tq;     
	CAN_InitStructure.CAN_BS2       =   CAN_BS2_5tq;   
	
	CAN_InitStructure.CAN_Prescaler =   5;
	CAN_Init(CAN2, &CAN_InitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber        =   14;
	CAN_FilterInitStructure.CAN_FilterMode          =   CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale         =   CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=   CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation    =   ENABLE; 

	CAN_FilterInitStructure.CAN_FilterIdHigh        =   0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow         =   0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh    =   0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow     =   0x0000;
	
	CAN_FilterInit(&CAN_FilterInitStructure);
  /* FIFO0 message pending interrupt */
	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);

}

/*---------------------L O C A L - F U N C T I O N S--------------------------*/



/*-----------------------------------FILE OF END------------------------------*/






