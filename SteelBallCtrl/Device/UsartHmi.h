#ifndef   _USART_HMI_
#define   _USART_HMI_

#include "stm32f1xx_hal.h"

typedef struct
{
	uint8_t HmiBuf[3];
	uint8_t Flag;

}Hmi_t;


extern	Hmi_t Hmi;
void Hmi_getMassage(uint8_t *pHmiData);

#endif

