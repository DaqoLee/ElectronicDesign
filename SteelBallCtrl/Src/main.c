/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "Gyro.h"
#include "UsartHmi.h"
#include <stdlib.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void PrintDisplay(void);
double getAlpha(double time);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//uint16_t Capture[0].Buff[3]={0};
//uint8_t Capture[0].Cnt = 0;
//uint16_t PwmCycle,Capture[0].Time;

uint16_t CapTime=0;
Capture_t Capture[2];
uint8_t PrintBuff[3]={0xff,0xff,0xff};
float Angle;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//uint16_t flag=0;
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	__HAL_TIM_ENABLE_IT(&htim2,TIM_IT_UPDATE); 
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE );
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE );
	//HAL_UART_Receive_DMA(&huart2,GY955.buff,20);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */

		PrintDisplay();
  }
	
	

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enables the Clock Security System 
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM2 )
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
		  switch(Capture[0].Cnt)
			{
				case 1:
						Capture[0].Buff[0] = __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_1);
				    TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1); 
						__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);//RISING
						Capture[0].Cnt++;
				    Capture[0].Num++;
				    break;
				case 2:
						Capture[0].Buff[1] = __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_1) + Capture[0].Temp*65535;
				    HAL_TIM_IC_Stop_IT(htim,TIM_CHANNEL_1);
						Capture[0].Cnt++;
				    break;
				default:
					break;
			}
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
		  switch(Capture[1].Cnt)
			{
				case 1:
						Capture[1].Buff[0] = __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_2);
		    		TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2); 
						__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_RISING);//RISING
						Capture[1].Cnt++;
				    Capture[1].Num++;
				    break;
				case 2:
						Capture[1].Buff[1] = __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_2) + Capture[1].Temp*65535;
				    HAL_TIM_IC_Stop_IT(htim,TIM_CHANNEL_2);
						Capture[1].Cnt++;
				    break;
				default:
					break;
			}				
		}

	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//更新中断（溢出）发生时执行
{
	static uint16_t time=0;
  if(htim->Instance == TIM6 )
	{
		if(Capture[0].Num > Capture[1].Num)
		{
			Capture[0].Flag=1;
		  time++;
		}
		else if(Capture[0].Num < Capture[1].Num)
		{
			Capture[0].Flag=0;
			time++;
		}
		else
    {
			if(time!=0)
			{
			  CapTime=time;
				time=0;
			}
			
		}
		
	}
	else if(htim->Instance == TIM7)
	{
		switch(Capture[0].Cnt)
		{
			case 0:
				Capture[0].Cnt++;
			  __HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);
			  HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
			  
			    break;
			case 3:
				
		     Capture[0].Time = Capture[0].Buff[1] - Capture[0].Buff[0];
				 if(Capture[0].Num == Capture[1].Num)
				 {
						Angle=getAlpha(CapTime/100.0f);
			   }
				 __HAL_TIM_DISABLE(&htim2); 
				 
 				 __HAL_TIM_SET_COUNTER(&htim2,0);
				 TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1); 
	   		 Capture[0].Cnt = 0;
				 Capture[0].Temp = 0;
				 
				 __HAL_TIM_ENABLE(&htim2); 
			    break;
			default:
				break;
		
		}
		
		switch(Capture[1].Cnt)
		{
			case 0:
				Capture[1].Cnt++;
		  	__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_FALLING);
		  	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
			  
			    break;
			case 3:
				
		     Capture[1].Time = Capture[1].Buff[1] - Capture[1].Buff[0];
			
				 if(Capture[0].Num == Capture[1].Num)
				 {
						Angle=getAlpha(CapTime/100.0f);
			   }
				 __HAL_TIM_DISABLE(&htim2); 
 				 __HAL_TIM_SET_COUNTER(&htim2,0);
				 TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2); 
	   		 Capture[1].Cnt = 0;
				 Capture[1].Temp=0;
				 __HAL_TIM_ENABLE(&htim2); 
			    break;
			default:
				break;
		
		}
	}

}

void PrintDisplay(void)
{
		switch(Hmi.Flag)
		{
		 case 0:
					printf("t0.PrintBuff=\"%.2f\"",GY955.Yaw);
					HAL_UART_Transmit(&huart1,PrintBuff,3,10);
				
					printf("t1.PrintBuff=\"%.2f\"",GY955.Pitch);
					HAL_UART_Transmit(&huart1,PrintBuff,3,10);
				
					printf("t2.PrintBuff=\"%.2f\"",GY955.Roll);
					HAL_UART_Transmit(&huart1,PrintBuff,3,10);	
		
		      printf("t3.PrintBuff=\"S1:%.2f ms\"",(float)Capture[0].Time/100);
					HAL_UART_Transmit(&huart1,PrintBuff,3,10);
		 
		      printf("t4.PrintBuff=\"S2:%.2f ms\"",(float)Capture[1].Time/100);
					HAL_UART_Transmit(&huart1,PrintBuff,3,10);
		 
		      printf("t5.PrintBuff=\"Tim:%.1f ms\"",CapTime/100.0f);
					HAL_UART_Transmit(&huart1,PrintBuff,3,10);
		 
					printf("t9.PrintBuff=\"Num:%d\"",Capture[1].Num);
		      HAL_UART_Transmit(&huart1,PrintBuff,3,10);

					printf("t10.PrintBuff=\"Angle:%.2f\"",Angle);
		      HAL_UART_Transmit(&huart1,PrintBuff,3,10);
		 
					if(Capture[0].Flag==1)
					{
						printf("t11.PrintBuff=\"Dir:->(10)\"");
					 HAL_UART_Transmit(&huart1,PrintBuff,3,10);
					}
					else
					{
						printf("t11.PrintBuff=\"Dir:<-(01)\"");
					  HAL_UART_Transmit(&huart1,PrintBuff,3,10);
					}
			 break;
		 case 1:
			    printf("t0.PrintBuff=\"T:%d\"",Capture[1].Num/2);
				  HAL_UART_Transmit(&huart1,PrintBuff,3,10);
			 break;
		 case 2:
			    printf("t0.PrintBuff=\"Num:%d\"",Capture[1].Num);
					HAL_UART_Transmit(&huart1,PrintBuff,3,10);   
			 break ;
		 default:
			 break;
	 
	 }
	

}

double getAlpha(double time)
{
 double a, b, c;
 double alpha;
 a = 2.727e+04;
 b = -1.886;
 c = 1.07;

 alpha = a*powl(time, b) + c;

 return alpha;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
