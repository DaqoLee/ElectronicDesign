#ifndef _USER_ADC_H
#define _USER_ADC_H

#include "adc.h"
#define N 20 //ÿͨ����20��
#define M 4 //Ϊ8��ͨ��
extern uint32_t ADC_Value[N][M];
extern uint32_t WDD35D4[M];
uint16_t Get_Adc(void);
void Get_Adc_Average(void);
void ADC1_Init(void);
#endif

