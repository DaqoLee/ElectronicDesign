/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : BSP_TIM.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-18               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "BSP_TIM.h" 
	
/*-------------------------- D E F I N E S -----------------------------------*/



/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/



/*------------------G L O B A L - F U N C T I O N S --------------------------*/

void BSP_TIM1PWMOutput(u16 prescaler, u16 period, u16 Pulse)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  /*使能GPIO时钟*/
	RCC_AHB1PeriphClockCmd(TIM1_CH1_GPIO_CLK | TIM1_CH2_GPIO_CLK \
	  | TIM1_CH3_GPIO_CLK | TIM1_CH4_GPIO_CLK, ENABLE);

	/*使能定时器时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_InitStructure.GPIO_Pin = TIM1_CH1_Pin;
	GPIO_Init(TIM1_CH1_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(TIM1_CH1_GPIO_PORT,TIM1_CH1_PINSOURCE, GPIO_AF_TIM1);

	GPIO_InitStructure.GPIO_Pin = TIM1_CH2_Pin;
	GPIO_Init(TIM1_CH2_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(TIM1_CH2_GPIO_PORT,TIM1_CH2_PINSOURCE, GPIO_AF_TIM1);

	GPIO_InitStructure.GPIO_Pin = TIM1_CH3_Pin;
	GPIO_Init(TIM1_CH3_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(TIM1_CH3_GPIO_PORT,TIM1_CH3_PINSOURCE, GPIO_AF_TIM1);

	GPIO_InitStructure.GPIO_Pin = TIM1_CH4_Pin;
	GPIO_Init(TIM1_CH4_GPIO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(TIM1_CH4_GPIO_PORT,TIM1_CH4_PINSOURCE, GPIO_AF_TIM1);


	/*配置定时器时钟分频*/
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
	/*配置自动重装载寄存器的值*/
	TIM_TimeBaseStructure.TIM_Period = period;
	/*配置死区时间与采样滤波分频，不分频*/
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	/*配置计数模式，向上计数*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	/*配置输出比较结构体*/
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/*配置输出模式，PWM1模式*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	/*使能输出*/
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*不使能互补输出*/
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	/*设置初始占空比*/
	TIM_OCInitStructure.TIM_Pulse = Pulse;
	/*设置PWM极性，初始电平为高*/
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/*设置空闲时输出电平*/
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	
	/*设置互补输出PWM极性，初始电平为高(不用也要配置，否则输出可能出错)*/
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	/*设置互补输出空闲时输出低电平(不用也要配置，否则输出可能出错)*/
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);

	/*使能定时器*/
	TIM_Cmd(TIM1, ENABLE);
	/* 高级定时器需要使能PWM输出 */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
}
void BSP_TIM6Init(uint16_t prescaler,uint16_t period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);

	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_Prescaler =prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE );
	TIM_Cmd(TIM6, ENABLE);
}

/*---------------------L O C A L - F U N C T I O N S--------------------------*/

/*-----------------------------------FILE OF END------------------------------*/





