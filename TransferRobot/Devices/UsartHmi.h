#ifndef _USART_HMI_H
#define _USART_HMI_H
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct
{
	uint8_t buff[7];
	uint8_t ID;
	uint16_t d;
	int16_t a;
	uint16_t flag;
	uint16_t shoot;
	uint16_t num;
	uint8_t reset;

}UsartHmi_t;


extern UsartHmi_t UsartHmi;
void UsartHmi_getMessage(uint8_t *pUsartHmiBuf);
void UsartHmi_setMessage(void);
#endif