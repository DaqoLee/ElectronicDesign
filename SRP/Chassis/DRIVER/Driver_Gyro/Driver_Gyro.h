#ifndef	__DRIVER_GYRO_H
#define __DRIVER_GYRO_H
#include "user_can.h"

void CAN_IMU_Reset(CAN_HandleTypeDef* hcan,uint16_t ID);
void CAN_IMU_Calib(CAN_HandleTypeDef* hcan,uint16_t ID);
void CAN_IMU_SET_ANGLE(CAN_HandleTypeDef* hcan,uint16_t ID,float radian);
void Gyro_Init(CAN_HandleTypeDef* hcan,uint16_t ID,float radian);
void Analysis_Cloud_Gyro(void);
void Analysis_Chassis_Gyro(void);
void Get_Target_Angle(void);
extern uint8_t Cloud_Gyro_Data[20],Chassis_Gyro_Data[20]; 
extern uint8_t Gyro_Init_flag;
extern  int16_t ERror;
#endif
