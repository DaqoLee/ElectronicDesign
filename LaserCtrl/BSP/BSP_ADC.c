#include "BSP_ADC.h"

void BSP_ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  //ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC1, ENABLE );
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	
}

