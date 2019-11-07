/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Task_Ctrl.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-02-20               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Task_Ctrl.h" 
#include "FreeRTOS.h"
#include "task.h"
#include "DR16.h"
#include "Servo.h"
#include "LED.h"
#include "Camera.h"
#include "Gimbal.h"
/*-------------------------- D E F I N E S -----------------------------------*/

static TaskHandle_t xHandleCtrlGimbal = NULL;

/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

static void vTaskCtrlGimbal(void *pvParameters);

/*------------------G L O B A L - F U N C T I O N S --------------------------*/
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:18
	* @brief   �������񴴽�����
	* @param   void
	* @retval  void
	*/
void ControlTaskCreate(void)
{
	xTaskCreate(vTaskCtrlGimbal,            /* ������  */   
						  "vTaskCtrlGimbal",          /* ������    */ 
						  128,       			             /* ����ջ��С*/
						  NULL,                        /* �������  */
					    2,       			               /* �������ȼ�*/
						  &xHandleCtrlGimbal);        /* ������  */ 
}

/*---------------------L O C A L - F U N C T I O N S--------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-02-20 15:18
	* @brief   ��̨��������
	* @param   void
	* @retval  void
	*/
static void vTaskCtrlGimbal(void *pvParameters)
{
	portTickType CurrentControlTick = 0;

	
	DXL1_setSyncMsg(USART_2,SPEED,2,0x01,20,0x02,20);/*���ö���ٶ�*/
	
	PID_StructInit(&Laser.PIDx,POSITION_PID,100,60,0.1,0.01,0);
	PID_StructInit(&Laser.PIDy,POSITION_PID,100,60,0.1,0.01,0);/*PID������ʼ��*/
  while(1)
	{
		
		Gimbal_Ctrl();
	  vTaskDelayUntil(&CurrentControlTick, 20 / portTICK_RATE_MS);/*20ms��ʱ*/
	}
}

/*-----------------------------------FILE OF END------------------------------*/

