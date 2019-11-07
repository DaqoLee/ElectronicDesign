#include "User_USART.h"
#include "usart.h"
#include "Driver_Niming.h"
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 {
 
 }
 
 void UART_Init()
 {
  __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE );//串口1空闲中断使能
  __HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE );//串口2空闲中断使能 
  __HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE );//串口3空闲中断使能
  __HAL_UART_ENABLE_IT(&huart6,UART_IT_IDLE );//串口6空闲中断使能
  __HAL_UART_ENABLE_IT(&huart7,UART_IT_IDLE );//串口7空闲中断使能
  __HAL_UART_ENABLE_IT(&huart8,UART_IT_IDLE );//串口8空闲中断使能
	 
	 Init_Ninming(&huart2);//匿名上位机初始化
 }
 
