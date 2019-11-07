#ifndef __DRIVER_HIEROGRAM_H
#define __DRIVER_HIEROGRAM_H
#include "control.h"
#include "Ctrl_Cloud.h"

extern uint8_t Hierogram[20],Hierogram_flag,Hierogram_Shoot;
extern uint16_t First_Pitch_Angle,Hierogram_Yaw[3][3],Hierogram_Pitch[3][3];
extern pid_t Auto_Yaw_PID,Auto_Pitch_PID;
extern int16_t Hierogram_Y,Hierogram_X;
extern FrameRate_Struct PC_Data;
extern float Target_Auto_Yaw,Target_Auto_Pitch;
void Hierogram_Get(void);
void PC_Data_Analysis(void);
#endif

