#ifndef __IM1258B_H 
#define __IM1258B_H 

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BSP_USART.h"
#include "Task_Usart.h"

#define Read_ID 0x01

typedef struct
{
	uint8_t buff[29];
  uint32_t Voltage_data;
	uint32_t Current_data;
	uint32_t Power_data;
	uint32_t Energy_data;
	uint32_t Pf_data;
	uint32_t CO2_data;
}IM1253B_t;

extern IM1253B_t IM1253B;

void IM1253B_getMessage(uint8_t *RX_Buffer,uint8_t reveive_number);
void IM1253B_readData(void);
#endif

