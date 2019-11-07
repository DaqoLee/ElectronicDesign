#include "stm32f4xx_it.h"
#include <stdio.h>
#include "Task_Usart.h"
#include "BSP_USART.h"
#include "BSP_CAN.h"
#include "DR16.h"
#include "Servo.h"
#include "Task_Init.h"
#include "Camera.h"
/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  USART1�жϷ�������DRң����
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		/*�ر�DMA*/
		DMA_Cmd(USART1_RX_DMA_STREAM, DISABLE);
		/*��ȡDMAbuffʣ���С���Ƿ�ƥ��*/
		if (DMA_GetCurrDataCounter(USART1_RX_DMA_STREAM) == 2)
		{
			xQueueSendFromISR(xUsart1RxQueue,&DR16.buff,&xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
		
		/*��DMA*/
		DMA_Cmd(USART1_RX_DMA_STREAM, ENABLE);
		/*��������жϱ�־λ*/
		(void)USART1->DR;
		(void)USART1->SR;

	}
}

void USART3_IRQHandler(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		/*�ر�DMA*/
		DMA_Cmd(USART3_RX_DMA_STREAM, DISABLE);
		/*��ȡDMAbuffʣ���С���Ƿ�ƥ��*/
		if (DMA_GetCurrDataCounter(USART3_RX_DMA_STREAM) == 2)
		{
			xQueueSendFromISR(xUsart3RxQueue,&Laser.buff,&xHigherPriorityTaskWoken);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}
		
		/*��DMA*/
		DMA_Cmd(USART3_RX_DMA_STREAM, ENABLE);
		/*��������жϱ�־λ*/
		(void)USART3->DR;
		(void)USART3->SR;

	}
}





