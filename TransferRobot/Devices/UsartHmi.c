#include "UsartHmi.h"
#include "BSP_USART.h"
#include "Task_Usart.h"
#include "IM1253B.h"
#include "Task_Ctrl.h" 
#include "Camera.h"

UsartHmi_t UsartHmi;

void UsartHmi_getMessage(uint8_t *pUsartHmiBuf)
{
	if(pUsartHmiBuf[0]==0xFF&&pUsartHmiBuf[1]==0xFF
		
	  &&pUsartHmiBuf[5]==0xAA&&pUsartHmiBuf[6]==0xAA)
	{
		
	  UsartHmi.ID=pUsartHmiBuf[2];
	
		switch(UsartHmi.ID)
		{
		  case 0:
				UsartHmi.d=pUsartHmiBuf[3]|pUsartHmiBuf[4]<<8;
				break;
		  case 1:
				UsartHmi.a=pUsartHmiBuf[3]|pUsartHmiBuf[4]<<8;
				break;
		  case 2:
				UsartHmi.shoot=pUsartHmiBuf[3]|pUsartHmiBuf[4]<<8;
				break;
		  case 3:
				UsartHmi.flag=pUsartHmiBuf[3]|pUsartHmiBuf[4]<<8;
				break;
		  case 4:
				UsartHmi.num=pUsartHmiBuf[3]|pUsartHmiBuf[4]<<8;
				break;
		  case 5:
				UsartHmi.reset=pUsartHmiBuf[3]|pUsartHmiBuf[4]<<8;
				break;
			default:
				break;
		}
	}

}



void UsartHmi_setMessage(void)
{
  static uint8_t PrintBuff[3]={0xff,0xff,0xff};
	USARTSend_t usartSend;
	usartSend.USART_x=USART_2;
	usartSend.pUSARTSendBuff=PrintBuff;
	
	printf("n2.val=%d",UsartHmi.d);
	vTaskDelay(1);
	xQueueSend(xusartTxQueue, &usartSend, 20);
	vTaskDelay(1);
	printf("n3.val=%d",UsartHmi.a);
	vTaskDelay(1);
	xQueueSend(xusartTxQueue, &usartSend, 20);
	vTaskDelay(1);
	printf("x2.val=%d",(int)RcPitch*3600/4096);
	vTaskDelay(1);
	xQueueSend(xusartTxQueue, &usartSend, 20);
	
	vTaskDelay(1);
	printf("x1.val=%d",(int)RcYaw*3600/4096);
	vTaskDelay(1);
	xQueueSend(xusartTxQueue, &usartSend, 20);
	
	printf("x0.val=%d",IM1253B.Voltage_data/1000);
	vTaskDelay(1);
	xQueueSend(xusartTxQueue, &usartSend, 20);
	
	printf("page0.n0.val=%d",Camera.d);
	vTaskDelay(1);
	xQueueSend(xusartTxQueue, &usartSend, 20);//UsartHmi.num
	
	printf("page0.n1.val=%d",UsartHmi.num);
	vTaskDelay(1);
	xQueueSend(xusartTxQueue, &usartSend, 20);//

}