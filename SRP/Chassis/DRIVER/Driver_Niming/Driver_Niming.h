#ifndef __DRIVER_NIMING_H
#define __DRIVER_NIMING_H
#include "stm32f4xx_hal.h"

#define user_TI_Succeed 				((uint16_t)0x0040)//发送完成标志
#define user_Timeout 						((uint32_t)0xFF)

void usart1_send_char(uint8_t c);
void usart1_niming_report(uint8_t fun,uint8_t *data,uint8_t len);
void Send_PC_Data(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz);
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);
void Init_Ninming(UART_HandleTypeDef* _huart_);
uint16_t user_UART_WaitOnFlagUntilTimeout(void);

extern uint8_t send_buf[32];
    


#endif



