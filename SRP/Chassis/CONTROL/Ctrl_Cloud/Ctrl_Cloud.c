#include "Ctrl_Cloud.h"
#include "Driver_Hierogram.h"
const	    float	Pitch_Min=((8192.0f*(PITCH_RANGE/360.0f))/2+0.5f),Pitch_Max=(8192.0f-Pitch_Min+0.5f);//根据角度限制范围确定Pitch最小和最大机械角度
const       float   Yaw_Min=((8192.0f*(YAW_RANGE/360.0f))/2+0.5f),Yaw_Max=(8192.0f-Yaw_Min+0.5f);
const       float	Gyro_Yaw_Min=YAW_GYRO_RANGE/2,Gyro_Yaw_Max=360-Gyro_Yaw_Min;

 /*
  * @brief 云台电机PID计算
  * @param Yaw轴与Pitch轴的PID结构体，实际值与目标值。
  * @retval None
  */
void M6623_PID_Set(pid_t* PID_Yaw, float Yaw_Real,float Yaw_Target,pid_t* PID_Pitch, float Pitch_Real,float Pitch_Target)
{
	float Current=0,Current1=0;
	//云台Pitch轴
	Current=pid_calc(PID_Pitch,Pitch_Real,Pitch_Target);//外环PID计算
	if(PID_Pitch==&CloudParam.Pitch.PID.Out)//判断外环类型
	{
		Current1=pid_calc(&CloudParam.Pitch.PID.In,CloudParam.Cloud_Gyro.Gyr_Y,Current);//内环PID计算
		CloudParam.Pitch.Target_Current=-Current1;
	}
	else if(PID_Pitch==&CloudParam.Cloud_Gyro.Pitch_PID.Out)
	{
		Current1=pid_calc(&CloudParam.Cloud_Gyro.Pitch_PID.In,CloudParam.Cloud_Gyro.Gyr_Y,Current);
		CloudParam.Pitch.Target_Current=Current;
	}
	else if(PID_Pitch==&Auto_Pitch_PID)
	{
		CloudParam.Pitch.Target_Current=-Current;
	}
	
	//云台Yaw轴
	Current=pid_calc(PID_Yaw,Yaw_Real,Yaw_Target);
	if(PID_Yaw==&CloudParam.Yaw.PID.Out)
	{
		Current1=pid_calc(&CloudParam.Yaw.PID.In,CloudParam.Cloud_Gyro.Gyr_Z,Current);
		CloudParam.Yaw.Target_Current=-Current1;
		
	}   
		else if(PID_Yaw==&CloudParam.Cloud_Gyro.Yaw_PID.Out)
	{
		Current1=pid_calc(&CloudParam.Cloud_Gyro.Yaw_PID.In,CloudParam.Cloud_Gyro.Gyr_Z,-Current);
		CloudParam.Yaw.Target_Current=-Current1;
	}
	else if(PID_Yaw==&Auto_Yaw_PID)
	{
 		CloudParam.Yaw.Target_Current=-Current;
	}

}

 /*
  * @brief 云台电机和陀螺仪相关参数的处理，云台电机的绝对是编码器（0~8191），陀螺仪安装位置影响，Pitch轴真实参数用Roll替换
  * @param None
  * @retval None
  */

void Cloud_Param_Set()//云台
{	

/**************************************把目标值限制在0~8191**************************************/	
	CloudParam.Pitch.Target_Angle=CloudParam.Pitch.Target_Angle>8191?CloudParam.Pitch.Target_Angle-8191:CloudParam.Pitch.Target_Angle;
	CloudParam.Pitch.Target_Angle=CloudParam.Pitch.Target_Angle<0?CloudParam.Pitch.Target_Angle+8191:CloudParam.Pitch.Target_Angle;
	
	CloudParam.Yaw.Target_Angle=CloudParam.Yaw.Target_Angle>8191?CloudParam.Yaw.Target_Angle-8191:CloudParam.Yaw.Target_Angle;
	CloudParam.Yaw.Target_Angle=CloudParam.Yaw.Target_Angle<0?CloudParam.Yaw.Target_Angle+8191:CloudParam.Yaw.Target_Angle;
	
	CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw>360?CloudParam.Cloud_Gyro.Target_Yaw-360:CloudParam.Cloud_Gyro.Target_Yaw;
	CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw<0?CloudParam.Cloud_Gyro.Target_Yaw+360:CloudParam.Cloud_Gyro.Target_Yaw;
	
	CloudParam.Cloud_Gyro.Target_Roll=CloudParam.Cloud_Gyro.Target_Roll>10?10:CloudParam.Cloud_Gyro.Target_Roll;
	CloudParam.Cloud_Gyro.Target_Roll=CloudParam.Cloud_Gyro.Target_Roll<-15?-15:CloudParam.Cloud_Gyro.Target_Roll;
/***********************************Yaw轴限制在180°和过零处理************************************/	
	if(MEDIAN_YAW>Yaw_Min&&MEDIAN_YAW<Yaw_Max)//中间值在最大值和最小值之间，不会经过零点
	{
		
	  	CloudParam.Yaw.Target_Angle=CloudParam.Yaw.Target_Angle>MEDIAN_YAW+Yaw_Min?MEDIAN_YAW+Yaw_Min:CloudParam.Yaw.Target_Angle;
	    CloudParam.Yaw.Target_Angle=CloudParam.Yaw.Target_Angle<MEDIAN_YAW-Yaw_Min?MEDIAN_YAW-Yaw_Min:CloudParam.Yaw.Target_Angle;
		
	}
	
	else if(MEDIAN_YAW<Yaw_Min)//中间值小于最小值，过零处理
	{
		CloudParam.Yaw.Target_Angle=(CloudParam.Yaw.Target_Angle<(MEDIAN_YAW+4096))&&\
		(CloudParam.Yaw.Target_Angle>(MEDIAN_YAW+Yaw_Min))?MEDIAN_YAW+Yaw_Min:CloudParam.Yaw.Target_Angle;
		CloudParam.Yaw.Target_Angle=(CloudParam.Yaw.Target_Angle>(MEDIAN_YAW+4096))&&\
		(CloudParam.Yaw.Target_Angle<(MEDIAN_YAW+Yaw_Max))?MEDIAN_YAW+Yaw_Max:CloudParam.Yaw.Target_Angle;
	}
	else
	{
		CloudParam.Yaw.Target_Angle=(CloudParam.Yaw.Target_Angle>(MEDIAN_YAW-4096))&&\
		(CloudParam.Yaw.Target_Angle<(MEDIAN_YAW-Yaw_Min))?MEDIAN_YAW-Yaw_Min:CloudParam.Yaw.Target_Angle;
		CloudParam.Yaw.Target_Angle=(CloudParam.Yaw.Target_Angle<(MEDIAN_YAW-4096))&&\
		(CloudParam.Yaw.Target_Angle>(MEDIAN_YAW-Yaw_Max))?MEDIAN_YAW-Yaw_Max:CloudParam.Yaw.Target_Angle;
	}
	
/***********************************Pitch轴限制在60°和过零处理************************************/	
	if(MEDIAN_PITCH>Pitch_Min&&MEDIAN_PITCH<Pitch_Max)
	{
		if(Mode!=SUPPLY)//补给模式时解除角度限制
		{
			CloudParam.Pitch.Target_Angle=CloudParam.Pitch.Target_Angle>MEDIAN_PITCH+Pitch_Min?MEDIAN_PITCH+(int)Pitch_Min:CloudParam.Pitch.Target_Angle;
	        CloudParam.Pitch.Target_Angle=CloudParam.Pitch.Target_Angle<MEDIAN_PITCH-Pitch_Min?MEDIAN_PITCH-(int)Pitch_Min:CloudParam.Pitch.Target_Angle;
		}
	}
	
	else if(MEDIAN_PITCH<Pitch_Min)
	{
		CloudParam.Pitch.Target_Angle=(CloudParam.Pitch.Target_Angle<(MEDIAN_PITCH+4096))&&\
		(CloudParam.Pitch.Target_Angle>(MEDIAN_PITCH+Pitch_Min))?MEDIAN_PITCH+Pitch_Min:CloudParam.Pitch.Target_Angle;
		CloudParam.Pitch.Target_Angle=(CloudParam.Pitch.Target_Angle>(MEDIAN_PITCH+4096))&&\
		(CloudParam.Pitch.Target_Angle<(MEDIAN_PITCH+Pitch_Max))?MEDIAN_PITCH+Pitch_Max:CloudParam.Pitch.Target_Angle;
	}
	else
	{
		CloudParam.Pitch.Target_Angle=(CloudParam.Pitch.Target_Angle>(MEDIAN_PITCH-4096))&&\
		(CloudParam.Pitch.Target_Angle<(MEDIAN_PITCH-Pitch_Min))?MEDIAN_PITCH-Pitch_Min :CloudParam.Pitch.Target_Angle;
		CloudParam.Pitch.Target_Angle=(CloudParam.Pitch.Target_Angle<(MEDIAN_PITCH-4096))&&\
		(CloudParam.Pitch.Target_Angle>(MEDIAN_PITCH-Pitch_Max))?MEDIAN_PITCH-Pitch_Max:CloudParam.Pitch.Target_Angle;
	}
	
#if  INFANTRY==4	
//	if(ChassisParam.Chassis_Gyro.Yaw-ERror>Gyro_Yaw_Min&&ChassisParam.Chassis_Gyro.Yaw-ERror<Gyro_Yaw_Max)
//	{
//		CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw>359?2:CloudParam.Cloud_Gyro.Target_Yaw;
//		CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw<1?358:CloudParam.Cloud_Gyro.Target_Yaw;
//		CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw-(ChassisParam.Chassis_Gyro.Yaw-ERror)>Gyro_Yaw_Min?\
//		(ChassisParam.Chassis_Gyro.Yaw-ERror)+Gyro_Yaw_Min:CloudParam.Cloud_Gyro.Target_Yaw;
//		CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw-(ChassisParam.Chassis_Gyro.Yaw-ERror)<-Gyro_Yaw_Min?\
//		(ChassisParam.Chassis_Gyro.Yaw-ERror)-Gyro_Yaw_Min:CloudParam.Cloud_Gyro.Target_Yaw;
//	}
//	else if((ChassisParam.Chassis_Gyro.Yaw-ERror)<=Gyro_Yaw_Min)
//	{
//		CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw-(ChassisParam.Chassis_Gyro.Yaw-ERror)>Gyro_Yaw_Min\
//		&&CloudParam.Cloud_Gyro.Target_Yaw<(ChassisParam.Chassis_Gyro.Yaw-ERror)+180?(ChassisParam.Chassis_Gyro.Yaw-ERror)+Gyro_Yaw_Min:CloudParam.Cloud_Gyro.Target_Yaw;
//		CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw<(ChassisParam.Chassis_Gyro.Yaw-ERror)+Gyro_Yaw_Max\
//		&&CloudParam.Cloud_Gyro.Target_Yaw>(ChassisParam.Chassis_Gyro.Yaw-ERror)+180?(ChassisParam.Chassis_Gyro.Yaw-ERror)+Gyro_Yaw_Max:CloudParam.Cloud_Gyro.Target_Yaw;

//	}
//	
//	else if((ChassisParam.Chassis_Gyro.Yaw-ERror)>=Gyro_Yaw_Max)
//	{
//		CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw<(ChassisParam.Chassis_Gyro.Yaw-ERror)-Gyro_Yaw_Min\
//		&&CloudParam.Cloud_Gyro.Target_Yaw>(ChassisParam.Chassis_Gyro.Yaw-ERror)-180?(ChassisParam.Chassis_Gyro.Yaw-ERror)-Gyro_Yaw_Min:CloudParam.Cloud_Gyro.Target_Yaw;
//		CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw>(ChassisParam.Chassis_Gyro.Yaw-ERror)-Gyro_Yaw_Max\
//		&&CloudParam.Cloud_Gyro.Target_Yaw<(ChassisParam.Chassis_Gyro.Yaw-ERror)-180?(ChassisParam.Chassis_Gyro.Yaw-ERror)-Gyro_Yaw_Max:CloudParam.Cloud_Gyro.Target_Yaw;

//	}
#endif	
#if  	1	
if(DBUS_ReceiveData.switch_right==1&&Mode==COMMON)
{
	if(!CloudParam.Cloud_Gyro.Offline)
	{
		if(DBUS_CheckPush(KEY_CTRL))
		{
			CloudParam.Cloud_Gyro.Target_Yaw+=0.01f*DBUS_ReceiveData.mouse.x;
			CloudParam.Cloud_Gyro.Target_Roll+=0.01f*DBUS_ReceiveData.mouse.y;
			CloudParam.Pitch.Target_Angle-=0.2f*DBUS_ReceiveData.mouse.y;
		}
		else 
		{
//			Filter(0.04f*DBUS_ReceiveData.mouse.x,&Mou_x,0.5f);//滤波处理
//	    	Filter(0.8f*DBUS_ReceiveData.mouse.y,&Mou_y,0.5f);//滤波处理
//			CloudParam.Cloud_Gyro.Target_Yaw+=Mou_x;
//			CloudParam.Pitch.Target_Angle-=Mou_y;

			//限制鼠标速度
			DBUS_ReceiveData.mouse.x=DBUS_ReceiveData.mouse.x>200?200:DBUS_ReceiveData.mouse.x;
			DBUS_ReceiveData.mouse.x=DBUS_ReceiveData.mouse.x<-200?-200:DBUS_ReceiveData.mouse.x;
			
			DBUS_ReceiveData.mouse.y=DBUS_ReceiveData.mouse.y>200?200:DBUS_ReceiveData.mouse.y;
			DBUS_ReceiveData.mouse.y=DBUS_ReceiveData.mouse.y<-200?-200:DBUS_ReceiveData.mouse.y;
			
			CloudParam.Cloud_Gyro.Target_Yaw+=0.04f*DBUS_ReceiveData.mouse.x;
			CloudParam.Cloud_Gyro.Target_Roll+=0.02f*DBUS_ReceiveData.mouse.y;
			CloudParam.Pitch.Target_Angle-=0.8f*DBUS_ReceiveData.mouse.y;
			
		}
#if  INFANTRY==4
			if((CloudParam.Yaw.Real_Angle<MEDIAN_YAW-Yaw_Min&&CloudParam.Cloud_Gyro.Yaw_PID.Out.err[NOW]>0)\
				||(CloudParam.Yaw.Real_Angle>MEDIAN_YAW+Yaw_Min&&CloudParam.Cloud_Gyro.Yaw_PID.Out.err[NOW]<0))
		{
				CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Yaw;
		}
#elif INFANTRY==1

			if((CloudParam.Yaw.Real_Angle<MEDIAN_YAW-Yaw_Min&&CloudParam.Cloud_Gyro.Yaw_PID.Out.err[NOW]>0)\
				||(CloudParam.Yaw.Real_Angle>MEDIAN_YAW+Yaw_Min&&CloudParam.Cloud_Gyro.Yaw_PID.Out.err[NOW]<0))
		{
				CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Yaw;
		}
#endif
		
		CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw>360?CloudParam.Cloud_Gyro.Target_Yaw-360:CloudParam.Cloud_Gyro.Target_Yaw;
	    CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw<0?CloudParam.Cloud_Gyro.Target_Yaw+360:CloudParam.Cloud_Gyro.Target_Yaw;
		
		M6623_PID_Set(&CloudParam.Cloud_Gyro.Yaw_PID.Out,CloudParam.Cloud_Gyro.Yaw ,CloudParam.Cloud_Gyro.Target_Yaw,\
		&CloudParam.Pitch.PID.Out,CloudParam.Pitch.Real_Angle,CloudParam.Pitch.Target_Angle);
		CloudParam.Yaw.Target_Angle=CloudParam.Yaw.Real_Angle;
		
//		M6623_PID_Set(&CloudParam.Cloud_Gyro.Yaw_PID.Out,CloudParam.Cloud_Gyro.Yaw ,CloudParam.Cloud_Gyro.Target_Yaw,\
//		&CloudParam.Cloud_Gyro.Pitch_PID.Out,CloudParam.Cloud_Gyro.Roll,CloudParam.Cloud_Gyro.Target_Roll);
//		CloudParam.Yaw.Target_Angle=CloudParam.Yaw.Real_Angle;
//		CloudParam.Pitch.Target_Angle=CloudParam.Pitch.Real_Angle;
	}	
	else
	{
		CloudParam.Pitch.Target_Angle-=0.5f*Mou_y;
		CloudParam.Yaw.Target_Angle-=0.5f*Mou_x;
		//云台PID计算
		M6623_PID_Set(&CloudParam.Yaw.PID.Out,CloudParam.Yaw.Real_Angle ,CloudParam.Yaw.Target_Angle,\
		&CloudParam.Pitch.PID.Out,CloudParam.Pitch.Real_Angle,CloudParam.Pitch.Target_Angle);
		
		CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Yaw;//真实值作为目标值，切换时保证位置不变
		CloudParam.Cloud_Gyro.Target_Roll=CloudParam.Cloud_Gyro.Roll;
	}
}
else
#endif
	
{
	if(!CloudParam.Cloud_Gyro.Offline)
	{
		CloudParam.Cloud_Gyro.Target_Yaw=ABS(DBUS_ReceiveData.ch1)>20?CloudParam.Cloud_Gyro.Target_Yaw+DBUS_ReceiveData.ch1*0.005:CloudParam.Cloud_Gyro.Target_Yaw;
		CloudParam.Cloud_Gyro.Target_Roll=ABS(DBUS_ReceiveData.ch2)>20?CloudParam.Cloud_Gyro.Target_Roll-DBUS_ReceiveData.ch2*0.005:CloudParam.Cloud_Gyro.Target_Roll;
	    CloudParam.Pitch.Target_Angle=ABS(DBUS_ReceiveData.ch2)>20?CloudParam.Pitch.Target_Angle+DBUS_ReceiveData.ch2*0.05f:CloudParam.Pitch.Target_Angle;
     	
//			CloudParam.Cloud_Gyro.Target_Yaw+=DBUS_ReceiveData.ch1*0.002;
//			CloudParam.Cloud_Gyro.Target_Roll+=DBUS_ReceiveData.ch2*0.002;
		
#if  INFANTRY==4
	
			if((CloudParam.Yaw.Real_Angle<MEDIAN_YAW-Yaw_Min&&CloudParam.Cloud_Gyro.Yaw_PID.Out.err[NOW]>0)\
				||(CloudParam.Yaw.Real_Angle>MEDIAN_YAW+Yaw_Min&&CloudParam.Cloud_Gyro.Yaw_PID.Out.err[NOW]<0))
		{
				CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Yaw;
		}

#elif INFANTRY==1


		if((CloudParam.Yaw.Real_Angle<MEDIAN_YAW-Yaw_Min&&CloudParam.Cloud_Gyro.Yaw_PID.Out.err[NOW]>0)\
			||(CloudParam.Yaw.Real_Angle>MEDIAN_YAW+Yaw_Min&&CloudParam.Cloud_Gyro.Yaw_PID.Out.err[NOW]<0))
		{
			CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Yaw;
		}
#endif
		if(Mode==COMMON)
		{
		  	CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw>360?CloudParam.Cloud_Gyro.Target_Yaw-360:CloudParam.Cloud_Gyro.Target_Yaw;
	        CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Target_Yaw<0?CloudParam.Cloud_Gyro.Target_Yaw+360:CloudParam.Cloud_Gyro.Target_Yaw;
			M6623_PID_Set(&CloudParam.Cloud_Gyro.Yaw_PID.Out,CloudParam.Cloud_Gyro.Yaw ,CloudParam.Cloud_Gyro.Target_Yaw,\
			&CloudParam.Pitch.PID.Out,CloudParam.Pitch.Real_Angle,CloudParam.Pitch.Target_Angle);
			
//			M6623_PID_Set(&CloudParam.Cloud_Gyro.Position_Yaw_PID,CloudParam.Cloud_Gyro.Yaw ,CloudParam.Cloud_Gyro.Target_Yaw,\
//			&CloudParam.Cloud_Gyro.Position_Pitch_PID,CloudParam.Cloud_Gyro.Roll,CloudParam.Cloud_Gyro.Target_Roll);
//			CloudParam.Yaw.Target_Angle=CloudParam.Yaw.Real_Angle;
//			CloudParam.Pitch.Target_Angle=CloudParam.Pitch.Real_Angle;
//	    	M6623_PID_Set(&CloudParam.Cloud_Gyro.Yaw_PID.Out,CloudParam.Cloud_Gyro.Yaw ,CloudParam.Cloud_Gyro.Target_Yaw,\
//		    &CloudParam.Cloud_Gyro.Pitch_PID.Out,CloudParam.Cloud_Gyro.Roll,CloudParam.Cloud_Gyro.Target_Roll);
//		    CloudParam.Yaw.Target_Angle=CloudParam.Yaw.Real_Angle;
//		    CloudParam.Pitch.Target_Angle=CloudParam.Pitch.Real_Angle;
		}
		else 
		{
////			if(!PC_Data.Offline)
////			{	
//				//Filter(Hierogram_X,&Target_Auto_Yaw,0.1f);
//				//Filter(Hierogram_Y,&Target_Auto_Pitch,0.1f);
//				M6623_PID_Set(&Auto_Yaw_PID,CloudParam.Yaw.Real_Angle,Target_Auto_Yaw,&Auto_Pitch_PID,CloudParam.Pitch.Real_Angle,Target_Auto_Pitch);
//			//	CloudParam.Yaw.Target_Angle=CloudParam.Yaw.Real_Angle;
//			
//			//	M6623_PID_Set(&Auto_Yaw_PID,Hierogram_X,400,&Auto_Pitch_PID,Hierogram_Y,300);
//			//	CloudParam.Yaw.Target_Angle=CloudParam.Yaw.Real_Angle;
////	  		    CloudParam.Pitch.Target_Angle=CloudParam.Pitch.Real_Angle;
////			}
////			else
			M6623_PID_Set(&CloudParam.Yaw.PID.Out,CloudParam.Yaw.Real_Angle ,CloudParam.Yaw.Target_Angle,\
			&CloudParam.Pitch.PID.Out,CloudParam.Pitch.Real_Angle,CloudParam.Pitch.Target_Angle);
			
			CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Yaw;
			CloudParam.Cloud_Gyro.Target_Roll=CloudParam.Cloud_Gyro.Roll;
		
	}
	}
	else
	{
		CloudParam.Pitch.Target_Angle=ABS(DBUS_ReceiveData.ch2)>20?CloudParam.Pitch.Target_Angle-DBUS_ReceiveData.ch2*0.02f:CloudParam.Pitch.Target_Angle;
		CloudParam.Yaw.Target_Angle=ABS(DBUS_ReceiveData.ch1)>20?CloudParam.Yaw.Target_Angle-DBUS_ReceiveData.ch1*0.02f:CloudParam.Yaw.Target_Angle;
		
		M6623_PID_Set(&CloudParam.Yaw.PID.Out,CloudParam.Yaw.Real_Angle ,CloudParam.Yaw.Target_Angle,\
		&CloudParam.Pitch.PID.Out,CloudParam.Pitch.Real_Angle,CloudParam.Pitch.Target_Angle);
		
		CloudParam.Cloud_Gyro.Target_Yaw=CloudParam.Cloud_Gyro.Yaw;
		CloudParam.Cloud_Gyro.Target_Roll=CloudParam.Cloud_Gyro.Roll;
	}
	
}

}

 /*
  * @brief 云台的Yaw与Pitch轴的目标值设定函数
  * @param Pitch、Yaw 目标值
  * @retval None
  */
void Cloud_Target_Angle(int16_t Pitch,int16_t Yaw)
{
	if(Pitch==DBUS_ReceiveData.ch2&&Yaw==DBUS_ReceiveData.ch1)
	{
		CloudParam.Pitch.Target_Angle+=Pitch/30;
		CloudParam.Yaw.Target_Angle-=Yaw/30;
	}
	else if(Pitch==DBUS_ReceiveData.mouse.y&&Yaw==DBUS_ReceiveData.mouse.x)
	{
		CloudParam.Pitch.Target_Angle-=0.5*Pitch;
		CloudParam.Yaw.Target_Angle-=0.5*Yaw;
	}
	else
	{
		CloudParam.Pitch.Target_Angle=Pitch;
		CloudParam.Yaw.Target_Angle=Yaw;
	}
}

void Read_M6623_First_Angle()//获取Yaw轴初始位置
{
    CloudParam.Yaw.Target_Angle=CloudParam.Yaw.Real_Angle;
	CloudParam.Pitch.Target_Angle=CloudParam.Pitch.Real_Angle;

}


