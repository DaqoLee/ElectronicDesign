/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Task_Usart.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-02-20               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Task_Usart.h"
#include "BSP_USART.h"
#include "DR16.h"
#include "LED.h"
#include "Encoder.h"
#include "Servo.h"
#include "OpticFlow.h" 
#include "Gyro.h"
#include "Camera.h" 
#include "UsartHmi.h"
#include "IM1253B.h"

/*-------------------------- D E F I N E S -----------------------------------*/

QueueHandle_t xUsart1RxQueue = NULL;
QueueHandle_t xUsart2RxQueue = NULL;
QueueHandle_t xUsart3RxQueue = NULL;
QueueHandle_t xUsart6RxQueue = NULL;
QueueHandle_t xUart7RxQueue = NULL;
QueueHandle_t xusartTxQueue = NULL;

static TaskHandle_t xHandleUsart1Receive = NULL;
static TaskHandle_t xHandleUsart2Receive = NULL;
static TaskHandle_t xHandleUsart3Receive = NULL;
static TaskHandle_t xHandleUsart6Receive = NULL;
static TaskHandle_t xHandleUart7Receive = NULL;
static TaskHandle_t xHandleUsartsend     = NULL;

/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

static void vTaskUsart1Receive(void *pvParameters);
static void vTaskUsart2Receive(void *pvParameters);
static void vTaskUsart3Receive(void *pvParameters);
static void vTaskUsart6Receive(void *pvParameters);
static void vTaskUart7Receive(void *pvParameters);
static void vTaskUsartSend(void *pvParameters);

/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:59
	* @brief   USART任务创建函数
	* @param   void
	* @retval  void
	*/
void UsartTaskCreate(void)
{
	xUsart1RxQueue = xQueueCreate(20,20*sizeof(uint8_t));
	xUsart2RxQueue = xQueueCreate(20,28*sizeof(uint8_t));
	xUsart3RxQueue = xQueueCreate(20,20*sizeof(uint8_t));
	xUsart6RxQueue = xQueueCreate(20,20*sizeof(uint8_t));
	xUart7RxQueue  = xQueueCreate(20,20*sizeof(uint8_t));
	xusartTxQueue  = xQueueCreate(20,sizeof(USARTSend_t));

	
	xTaskCreate(vTaskUsart1Receive,             /* 任务函数  */
						  "vTaskUsart1Receive",           /* 任务名    */
						  128,       			                /* 任务栈大小*/
					  	NULL,                           /* 任务参数  */
					  	3,       			                  /* 任务优先级*/
					  	&xHandleUsart1Receive);         /* 任务句柄  */ 
	
	xTaskCreate(vTaskUsart2Receive,            
					  	"vTaskUsart2Receive",          
						  128,       			   
						  NULL,                 
						  3,       			   
					  	&xHandleUsart2Receive);	
	
	xTaskCreate(vTaskUsart3Receive,            
					  	"vTaskUsart3Receive",          
						  128,       			   
						  NULL,                 
						  3,       			   
					  	&xHandleUsart3Receive);
							
	xTaskCreate(vTaskUsart6Receive,            
					  	"vTaskUsart6Receive",          
						  128,       			   
						  NULL,                 
						  3,       			   
					  	&xHandleUsart6Receive);
							
	xTaskCreate(vTaskUart7Receive,            
					  	"vTaskUsart7Receive",          
						  128,       			   
						  NULL,                 
						  3,       			   
					  	&xHandleUart7Receive);
							
	xTaskCreate(vTaskUsartSend,            
					  	"vTaskUsartSend",          
						  128,       			   
						  NULL,                 
						  4,       			   
					  	&xHandleUsartsend);
}


/*---------------------L O C A L - F U N C T I O N S--------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:59
	* @brief   USART1解析任务
	* @param   void
	* @retval  void
	*/
static void vTaskUsart1Receive(void *pvParameters)
{
  uint8_t usart1RxBuffer[20];
  while(1)
	{
	  xQueueReceive(xUsart1RxQueue, &usart1RxBuffer,portMAX_DELAY);
		DR16.pDR16getMsg(usart1RxBuffer);
		LED_TOGGLE(LED_G);
	}

}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:59
	* @brief   USART2解析任务
	* @param   void
	* @retval  void
	*/
static void vTaskUsart2Receive(void *pvParameters)
{
  uint8_t usart2RxBuffer[7];
  while(1)
	{
	  xQueueReceive(xUsart2RxQueue, &usart2RxBuffer,portMAX_DELAY);
	//	Laser_getMessage(usart2RxBuffer);
	//	LED_TOGGLE(LED_R);
		UsartHmi_getMessage(usart2RxBuffer);
		//OpticFlow_getMassage(usart2RxBuffer);
	}

}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:59
	* @brief   USART3解析任务
	* @param   void
	* @retval  void
	*/
static void vTaskUsart3Receive(void *pvParameters)
{
  uint8_t usart3RxBuffer[29];
  while(1)
	{
	  xQueueReceive(xUsart3RxQueue, &usart3RxBuffer,portMAX_DELAY);
		IM1253B_getMessage(usart3RxBuffer,29);
		
	}

}

static void vTaskUsart6Receive(void *pvParameters)
{
  uint8_t usart6RxBuffer[26];
  while(1)
	{
	  xQueueReceive(xUsart6RxQueue, &usart6RxBuffer,portMAX_DELAY);
	//	Posture_getMessage(usart6RxBuffer);
	//	LED_TOGGLE(LED_R);
		DXL1_getMassage(usart6RxBuffer);
		//OpticFlow_getMassage(usart2RxBuffer);
	}

}

static void vTaskUart7Receive(void *pvParameters)
{
  uint8_t usart7RxBuffer[13];
  while(1)
	{
	  xQueueReceive(xUart7RxQueue, &usart7RxBuffer,portMAX_DELAY);
	//	Posture_getMessage(usart6RxBuffer);
	//	LED_TOGGLE(LED_R);
		Camera_getMassage(usart7RxBuffer);
	//	RFID_getMassage(usart7RxBuffer);
		//OpticFlow_getMassage(usart2RxBuffer);
	}

}
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:59
	* @brief   USART发送任务
	* @param   void
	* @retval  void
	*/
static void vTaskUsartSend(void *pvParameters)
{
	USARTSend_t usartSend;
  while(1)
	{
	  xQueueReceive(xusartTxQueue, &usartSend,portMAX_DELAY);
		switch(usartSend.USART_x)
		{
			case USART_1:
				break;
			case USART_2:
					 for(uint8_t i=0;i<3;i++)
					 {
						 USART_sendChar(USART2,usartSend.pUSARTSendBuff[i]);
					 }	
				break;
			case USART_3:
					 for(uint8_t i=0;i<8;i++)
					 {
						 USART_sendChar(USART3,usartSend.pUSARTSendBuff[i]);
					 }	
				break;
			case UART_4:
					 for(uint8_t i=0;i<usartSend.pUSARTSendBuff[3]+4;i++)
					 {
						 USART_sendChar(UART4,usartSend.pUSARTSendBuff[i]);
					 }	
				break;
			case UART_5:
					 for(uint8_t i=0;i<usartSend.pUSARTSendBuff[3]+4;i++)
					 {
						 USART_sendChar(UART5,usartSend.pUSARTSendBuff[i]);
					 }	
				break;
			case USART_6:
					 for(uint8_t i=0;i<usartSend.pUSARTSendBuff[3]+4;i++)
					 {
						 USART_sendChar(USART6,usartSend.pUSARTSendBuff[i]);
					 }	
				break;
			case UART_7:
					 for(uint8_t i=0;i<usartSend.pUSARTSendBuff[3]+4;i++)
					 {
						 USART_sendChar(UART7,usartSend.pUSARTSendBuff[i]);
					 }	
				break;
			case UART_8:
					 for(uint8_t i=0;i<usartSend.pUSARTSendBuff[3]+4;i++)
					 {
						 USART_sendChar(UART8,usartSend.pUSARTSendBuff[i]);
					 }	
				break;
			default:
				break;
		}
	}

}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:59
	* @brief   print优化
	* @param   void
	* @retval  void
	*/
void vPrintString(const char *pcString)
{
    taskENTER_CRITICAL();
    {
        printf("%s", pcString);
        fflush(stdout);
    }
    taskEXIT_CRITICAL();
}
/*-----------------------------------FILE OF END------------------------------*/



