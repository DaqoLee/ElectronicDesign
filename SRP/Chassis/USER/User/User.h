#ifndef	__USER_H
#define __USER_H
#include "stm32f4xx_hal.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "Driver_DBUS.h"
#include "user_can.h"
#include "control.h"
#include "Ctrl_Cloud.h"
#include "Driver_Gyro.h"
#include "Driver_Hierogram.h"
#include "User_ADC.h"

typedef struct
{
	uint32_t Distance;
	uint16_t Distance_Str[5];
	uint16_t Mean_Distance;
	uint8_t  Stop_Flag;
	
}Ultrasonic_Struct;
extern pid_t TextPID;	
extern uint8_t Ultrasonic_Message[20];
extern char RFID_RC522[8];
extern uint8_t Esp8266[14];
extern uint8_t Esp8266_Flag;
extern Ultrasonic_Struct Ultrasonic[8];
void Motor_Param_Init(void);
void Bell_Init(void);
void text(void);
void Analysis(void);
void Esp8266_init(void);
void Esp8266_Analysis(void);
#endif

