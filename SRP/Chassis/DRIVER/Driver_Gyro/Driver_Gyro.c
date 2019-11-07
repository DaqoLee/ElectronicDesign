#include "Driver_Gyro.h"
#include "control.h"
#include "Ctrl_Chassis.h"
#include "Ctrl_Cloud.h"
#include "Ctrl_Rammer.h"
#include "Ctrl_Frict.h"
#include "Driver_Hierogram.h"
uint8_t Gyro_Init_flag=0;//陀螺仪初始化完成标志
uint8_t Cloud_Gyro_Data[20],Chassis_Gyro_Data[20];
int16_t ERror;

 /*
  * @brief 云台陀螺仪报文解析
  * @param None
  * @retval None
  */
void Analysis_Cloud_Gyro()
{
	static uint8_t i=0,sum;
	
		for(sum=0,i=0;i<(Cloud_Gyro_Data[3]+4);i++)
			sum+=Cloud_Gyro_Data[i];
		if(sum==Cloud_Gyro_Data[i])//校验和判断
		{
			
			CloudParam.Cloud_Gyro.Gyr_X=((Cloud_Gyro_Data[4]<<8)|Cloud_Gyro_Data[5]);
			CloudParam.Cloud_Gyro.Gyr_Y=((Cloud_Gyro_Data[6]<<8)|Cloud_Gyro_Data[7]);
			CloudParam.Cloud_Gyro.Gyr_Z=((Cloud_Gyro_Data[8]<<8)|Cloud_Gyro_Data[9]);
	
			CloudParam.Cloud_Gyro.Yaw=(uint16_t)((Cloud_Gyro_Data[10]<<8)|Cloud_Gyro_Data[11])/100.0f;
			CloudParam.Cloud_Gyro.Roll=(int16_t)((Cloud_Gyro_Data[12]<<8)|Cloud_Gyro_Data[13])/100.0f;
			CloudParam.Cloud_Gyro.Pitch=(int16_t)((Cloud_Gyro_Data[14]<<8)|Cloud_Gyro_Data[15])/100.0f;
			
			CloudParam.Cloud_Gyro.FrameRate++;
		}
}

 /*
  * @brief 底盘陀螺仪报文解析
  * @param None
  * @retval None
  */
void Analysis_Chassis_Gyro()
{
	static uint8_t i=0,sum;
	
		for(sum=0,i=0;i<(Chassis_Gyro_Data[3]+4);i++)
			sum+=Chassis_Gyro_Data[i];
		if(sum==Chassis_Gyro_Data[i])//校验和判断
		{
			
			ChassisParam.Chassis_Gyro.Gyr_X=((Chassis_Gyro_Data[4]<<8)|Chassis_Gyro_Data[5]);
			ChassisParam.Chassis_Gyro.Gyr_Y=((Chassis_Gyro_Data[6]<<8)|Chassis_Gyro_Data[7]);
			ChassisParam.Chassis_Gyro.Gyr_Z=((Chassis_Gyro_Data[8]<<8)|Chassis_Gyro_Data[9]);
	
			ChassisParam.Chassis_Gyro.Yaw=(uint16_t)((Chassis_Gyro_Data[10]<<8)|Chassis_Gyro_Data[11])/100.0f;
			ChassisParam.Chassis_Gyro.Roll=(int16_t)((Chassis_Gyro_Data[12]<<8)|Chassis_Gyro_Data[13])/100.0f;
			ChassisParam.Chassis_Gyro.Pitch=(int16_t)((Chassis_Gyro_Data[14]<<8)|Chassis_Gyro_Data[15])/100.0f;
			
			ChassisParam.Chassis_Gyro.FrameRate++;
		}
}

void Get_Target_Angle()
{
	ChassisParam.TargetABSAngle=ChassisParam.Chassis_Gyro.Yaw;
	ChassisParam.Chassis_Gyro.Target_Yaw=ChassisParam.Chassis_Gyro.Yaw;
	CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Yaw;
	CloudParam.Cloud_Gyro.Target_Roll=CloudParam.Cloud_Gyro.Roll;
	ERror=ChassisParam.Chassis_Gyro.Yaw-CloudParam.Cloud_Gyro.Yaw;
	
	Target_Auto_Yaw=CloudParam.Yaw.Real_Angle;
	Target_Auto_Pitch=CloudParam.Pitch.Real_Angle;
	Read_M6623_First_Angle();
	SRP.TargetAngle=UWB.Angle;
}
void CAN_IMU_Reset(CAN_HandleTypeDef* hcan,uint16_t ID)//陀螺仪复位
{
	hcan->pTxMsg->StdId = ID;//0x0012
    hcan->pTxMsg->IDE = CAN_ID_STD;
	hcan->pTxMsg->RTR = CAN_RTR_DATA;
	hcan->pTxMsg->DLC = 0x02;
    hcan->pTxMsg->Data[0] = 0x55;
	hcan->pTxMsg->Data[1] = 0xff;
	
    HAL_CAN_Transmit(hcan, 1000);
}

void CAN_IMU_Calib(CAN_HandleTypeDef* hcan,uint16_t ID)//设置零点
{
	hcan->pTxMsg->StdId = ID;//0x0012
    hcan->pTxMsg->IDE = CAN_ID_STD;
	hcan->pTxMsg->RTR = CAN_RTR_DATA;
	hcan->pTxMsg->DLC = 0x02;
    hcan->pTxMsg->Data[0] = 0xcc;
	hcan->pTxMsg->Data[1] = 0xaa;
	
    HAL_CAN_Transmit(hcan, 1000);
}

void CAN_IMU_SET_ANGLE(CAN_HandleTypeDef* hcan,uint16_t ID,float radian)//设置当前角度
{
	
    hcan->pTxMsg->StdId = ID;//0x0012
    hcan->pTxMsg->IDE = CAN_ID_STD;
	hcan->pTxMsg->RTR = CAN_RTR_DATA;
	hcan->pTxMsg->DLC = 0x04;
	memcpy(&hcan->pTxMsg->Data[0], & radian, 4);
	HAL_CAN_Transmit(hcan, 1000);

}

void Gyro_Init(CAN_HandleTypeDef* hcan,uint16_t ID,float radian)//初始化陀螺仪相关参数
{
	 CAN_IMU_SET_ANGLE(hcan,ID,radian);
	 CAN_IMU_Calib(hcan,ID);
	 Gyro_Init_flag=1;
}
