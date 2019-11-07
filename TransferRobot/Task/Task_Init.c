/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Task_Init.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-12               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Task_Init.h"
#include "Task_Usart.h"
#include "Task_Can.h"
#include "Key.h"
#include "OLED.h"
#include "Motor.h"
#include "BSP_USART.h"
#include "BSP_TIM.h"
#include "Task_Ctrl.h"
#include "BSP_NVIC.h"
#include "DR16.h"
#include "Gyro.h"
#include "Servo.h"
#include "Filter.h" 
#include "Ultrasonic.h" 
#include "Camera.h" 
#include "Photoelectric.h" 
#include "Chassis.h"
#include "math.h"
#include "Encoder.h" 
#include "Joint.h"
#include "delay.h"
#include "Curve.h"
#include "UsartHmi.h"
#include "IM1253B.h"
#include "Gimbal.h"
/*-------------------------- D E F I N E S -----------------------------------*/

TaskHandle_t StartTaskHandler=NULL;

static TaskHandle_t TaskLEDHandler=NULL;
static TaskHandle_t TaskTestHandler=NULL;
static TaskHandle_t TaskMonitorHandler=NULL;

volatile uint32_t ulHighFrequencyTimerTicks = 0UL;
/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

static void vTaskLED(void *pvParameters);
static void vTaskTest(void *pvParameters);
static void vTaskMonitor(void *pvParameters);

static void TestTaskCreate(void);

/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-12 17:22
	* @brief   
	* @param   void
	* @retval  void
	*/
void vTaskStart(void *pvParameters)
{		
	taskENTER_CRITICAL();/*�����ٽ���*/
	/*------------------BSP��ʼ��------------------*/

  BSP_USART1_Init(100000); /*DR16���ջ�*/
  BSP_USART2_Init(115200);  /*SMS���*/
	BSP_USART3_Init(9600);  /*GY955������*/
	BSP_USART6_Init(1000000);  /*DXL���*/
	BSP_UART7_Init(9600);   /*����ͷ*/
	BSP_UART8_Init(57600);   /*������*/
	
//	BSP_USART2_Init(1000000);  /*DXL���*/
	
	DMA_USART1RxConfig((uint32_t)DR16.buff,20);
	DMA_USART2RxConfig((uint32_t)UsartHmi.buff,7);
	DMA_USART3RxConfig((uint32_t)IM1253B.buff,29);
	DMA_USART6RxConfig((uint32_t)DigitalServo.DxlBuff,20);
	DMA_UART8RxConfig((uint32_t)Ultrasonic.buff,20);
	DMA_UART7RxConfig((uint32_t)Camera.buff,13);

	BSP_CAN1_Init();
	//BSP_I2C2_Init();
	BSP_NVIC_Init();
	
	/*---------------Devices��ʼ��----------------*/
  delay_init();
  LED_Init();
	KEY_Init();
	DR16_Init();
	Gyro_Init();
	Motor_Init();
	OLED_Init();
	Photoelectric_Init();
  /*---------------Apps��ʼ��----------------*/

	Filter_Init();
	Joint_Init();
	Chassis_Init();
	/*-----------------Task����-------------------*/
	
	CanTaskCreate();   /* ����CAN������� */
	UsartTaskCreate(); /* ����USART������� */
  TestTaskCreate();  /* ��������������� */

	vTaskDelay(1000);  /*��ʱ�ȴ�ģ���ȶ�*/
	ControlTaskCreate();/* ������������ */
	/*-------------------------------------------*/
	
	vTaskDelete(StartTaskHandler);/*ɾ����ʼ����*/ 
  taskEXIT_CRITICAL(); /*�˳��ٽ���*/
}


/*---------------------L O C A L - F U N C T I O N S--------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-12 17:22
	* @brief   
	* @param   void
	* @retval  void
	*/
static void TestTaskCreate(void)
{
	xTaskCreate(vTaskLED,         /* ������  */   
						"vTaskLED",         /* ������    */ 
						32,       			     /* ����ջ��С*/
						NULL,                /* �������  */
						1,       			   		 /* �������ȼ�*/
						&TaskLEDHandler);   /* ������  */ 
#if 1
	xTaskCreate(vTaskTest,            
						"vTaskTest",          
						256,       			   
						NULL,                 
						1,       			   
						&TaskTestHandler); 
#endif

#if 0
	xTaskCreate(vTaskMonitor,            
						"vTaskMonitor",          
						256,       			   
						NULL,                 
						1,       			   
						&TaskMonitorHandler); 
#endif
}
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-12 17:22
	* @brief   
	* @param   void
	* @retval  void
	*/
static void vTaskLED(void *pvParameters)
{
	while(1)
	{
	//	LED_ON(LED0);
		for(uint8_t i=0;i<7;i++)
		{
				LED_ON(i);
				vTaskDelay(30); 
		}
		for(uint8_t i=0; i<7;i++)
		{
				LED_OFF(i);
				vTaskDelay(30);
		}
	
	}
}
/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-12 17:22
	* @brief   
	* @param   void
	* @retval  void
	*/
static void vTaskTest(void *pvParameters)
{
	static uint8_t flag=0,flag1=1;

	GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_9,Bit_SET);
	
	while(1)
	{
		
		//flag=flag>250?0:flag+1;
		
		IM1253B_readData();
//		vTaskDelay(10);
    UsartHmi_setMessage();
		
		if(Camera.shoot)
		{
			if(flag1)
			{
			  GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
		  	vTaskDelay(500);
		  	GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
				
				flag1=0;
			}

			
			if(UsartHmi.num==2||UsartHmi.num==3)
			{
				if(ABS(Camera.err)<5)
				{
				 GPIO_WriteBit(GPIOA,GPIO_Pin_9,Bit_RESET);
				  vTaskDelay(200);
				  Camera.shoot=0;
					flag1=1;
				}

				GPIO_WriteBit(GPIOA,GPIO_Pin_9,Bit_SET);
			}
			else if(IM1253B.Voltage_data>1480000)
			{
				 flag=1;
			}
			
			if((IM1253B.Voltage_data<=1450000)&&flag)
			{
				GPIO_WriteBit(GPIOA,GPIO_Pin_9,Bit_RESET);
				vTaskDelay(200);
				GPIO_WriteBit(GPIOA,GPIO_Pin_9,Bit_SET);
				flag=0;
				Camera.shoot=0;
				flag1=1;
			}
			else
			{
				
			}
			
			
		}
		else
    {
	  	
			
		}

		
		
//		switch(UsartHmi.d)
//		{
//			case 0:
//				break;
//			case 1:
//			break;
//			
//			
//			
//			default:
//				break;
//		
//		}
		
		
		vTaskDelay(10);
	}
}

/**
	* @Data    2019-03-18 15:31
	* @brief   
	* @param   void
	* @retval  void
	*/
static void vTaskMonitor(void *pvParameters)
{
	uint8_t pcWriteBuffer[500];
	BSP_USART2_Init(115200);
	BSP_TIM6Init(0,20000);
	while(1)
	{
		printf("=================================================\r\n");
		printf("������\t\t����״̬\t���ȼ�\tʣ��ջ\t�������\r\n");
		vTaskList((char *)&pcWriteBuffer);
		printf("%s\r\n", pcWriteBuffer);
	
		printf("\r\n������\t\t���м���\t\tʹ����\r\n");
		vTaskGetRunTimeStats((char *)&pcWriteBuffer);
		printf("%s\r\n", pcWriteBuffer);

		vTaskDelay(100);
	}
}
/*-----------------------------------FILE OF END------------------------------*/