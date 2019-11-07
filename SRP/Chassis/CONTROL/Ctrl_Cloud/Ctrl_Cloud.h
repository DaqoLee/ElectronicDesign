#ifndef __CTRL_CLOUD_H
#define __CTRL_CLOUD_H
#include "control.h"

void M6623_PID_Set(pid_t* PID, float Yaw_Real,float Yaw_Target,pid_t* PID_Pitch, float Pitch_Real,float Pitch_Target);
void Cloud_Param_Set(void);
void Cloud_Target_Angle(int16_t Pitch,int16_t Yaw);
void Read_M6623_First_Angle(void);

extern const	    float	Pitch_Min,Pitch_Max;
extern const      float   Yaw_Max,Yaw_Min;
extern const      float	Gyro_Yaw_Min,Gyro_Yaw_Max;

#endif


