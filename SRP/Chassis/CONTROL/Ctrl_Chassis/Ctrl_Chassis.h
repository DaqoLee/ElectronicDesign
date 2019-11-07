#ifndef __CTRL_CHASSIS_H
#define __CTRL_CHASSIS_H

#include "stm32f4xx_hal.h"
void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed);
void M3508_PID_Set(void);
void Chassis_Param_Set(uint16_t Max_X,uint16_t Max_Y);
void Power_Limit(float Cur_limit);
void Chassis(void);
void Dog_Text(void);
#endif

