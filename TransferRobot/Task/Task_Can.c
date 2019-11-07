/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Task_Can.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-11               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Task_Can.h"
#include "LED.h"
#include "Motor.h"	
/*-------------------------- D E F I N E S -----------------------------------*/
QueueHandle_t xCanSendQueue= NULL;
QueueHandle_t xCan1RxQueue = NULL;
QueueHandle_t xCan2RxQueue = NULL;

static TaskHandle_t xHandleCan1Receive = NULL;
static TaskHandle_t xHandleCan2Receive = NULL;
static TaskHandle_t xHandleCanSend = NULL;


/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

static void vTaskCanSend(void *Parameters);
static void vTaskCan1Receive(void *pvParameters);
static void vTaskCan2Receive(void *pvParameters);
static void CAN1_DataDecoding(CanRxMsg rxMessage);

/*------------------G L O B A L - F U N C T I O N S --------------------------*/
void CanTaskCreate(void)
{
	xCanSendQueue=xQueueCreate(20,sizeof(CanSend_t));
	xCan1RxQueue =xQueueCreate(20,sizeof(CanRxMsg));
	xCan2RxQueue =xQueueCreate(20,sizeof(CanRxMsg));
	
 	xTaskCreate(vTaskCan1Receive,            /* 任务函数  */
						"vTaskCan1Receive",            /* 任务名    */ 
						128,       			               /* 任务栈大小*/
						NULL,                          /* 任务参数  */
						3,       			                 /* 任务优先级*/
						&xHandleCan1Receive);          /* 任务句柄  */ 
	
	xTaskCreate(vTaskCan2Receive,            
						"vTaskCan2Receive",          
						128,       			   
						NULL,                 
						3,       			   
						&xHandleCan2Receive); 
	
 	xTaskCreate(vTaskCanSend,            
						"vTaskCanSend",          
						128,       			   
						NULL,                 
						3,       			   
						&xHandleCanSend); 
}

/*---------------------L O C A L - F U N C T I O N S--------------------------*/
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-11 11:13
	* @brief   
	* @param   void
	* @retval  void
	*/
static void vTaskCanSend(void *Parameters)
{
	CanSend_t canSendData;
	
	while(1)
	{
		xQueueReceive(xCanSendQueue, &canSendData, portMAX_DELAY);
			
		if(canSendData.CANx == 1)
		{     	
			CAN_Transmit(CAN1, &(canSendData.SendCanTxMsg));
		}
		else
		{
			CAN_Transmit(CAN2, &(canSendData.SendCanTxMsg));
		} 
	}
}


/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-11 11:13
	* @brief   
	* @param   void
	* @retval  void
	*/
static void vTaskCan1Receive(void *pvParameters)
{
	CanRxMsg rxMessage;
  while(1)
	{
	  xQueueReceive(xCan1RxQueue, &rxMessage,portMAX_DELAY);
	  Motor.p_M3508getMsg(rxMessage);
		Motor.p_M6020getMsg(rxMessage);
//	  CAN1_DataDecoding(rxMessage);
  //  LED_TOGGLE(LED_R);

	}
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-11 11:13
	* @brief   
	* @param   void
	* @retval  void
	*/
static void vTaskCan2Receive(void *pvParameters)
{
	CanRxMsg rxMessage;
  while(1)
	{
	  xQueueReceive(xCan2RxQueue, &rxMessage,portMAX_DELAY);

  //  LED_TOGGLE(LED_R);

	}
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-11 11:13
	* @brief   
	* @param   void
	* @retval  void
	*/
//static void CAN1_DataDecoding(CanRxMsg rxMessage)
//{
//	switch(rxMessage.StdId)
//	{
//		case 0x201:
//		case 0x202:
//		case 0x203:
//		case 0x204:
////			M3508_DataDecode(rxMessage);
//			break;
//	
//		case 0x205:
//		case 0x206:
//		case 0x207:
//		case 0x208:
//		//	M6020_DataDecode(rxMessage);
//			break;
//		case 0x401:

//			   ZGyroModuleAngle= -0.01f*((int32_t)(rxMessage.Data[0]<<24)|\
//		                               (int32_t)(rxMessage.Data[1]<<16)|\
//	                                 (int32_t)(rxMessage.Data[2]<<8) |\
//	                       	         (int32_t)(rxMessage.Data[3])); 
//			break;
//		
//		default: 
//			break;
//	}
//}



/*-----------------------------------FILE OF END------------------------------*/





