#include "Ctrl_Rammer.h"
#include "Ctrl_Frict.h"

const 	  int32_t 	Rammer_Max_Angle=((8192*REDUCTION_RATIO)-RAMMER_ERROR);//拨盘转一圈编码器累计的最大值
const     int32_t	St_Serve=(Rammer_Max_Angle/RAMMER_NUM+1);//转一格的线数

uint8_t Hierogram_Shoot=0;
Shoot_Struct Shoot;
 /*
  * @brief 拨弹电机参数设定，单击单发，长按连发。
  * @param None
  * @retval None
  */
void Rammer_Param_Set()//拨弹
{
	if(M2006.Real_Current>4000)//电流大于4000反转一格
	{
		M2006.Target_Angle=M2006.Target_Angle<=St_Serve?(Rammer_Max_Angle-2*St_Serve):M2006.Target_Angle-2*St_Serve;
	}
	else if(DBUS_ReceiveData.mouse.press_left||DBUS_ReceiveData.switch_left==2||Hierogram_Shoot==1)
	{
		if(DBUS_ReceiveData.mouse.press_right)//低速高频模式
		{
				switch(Judge_RobotState.robotLevel)//机器人当前等级
			{
				case 0:
				case 1://一级上限 90，每秒冷却18
					FRICT_SPEED=650;//射速
					Shoot.Freq=8;//射频；
					Shoot.Num=(120-Judge_RobotPowerHeatData.shooterHeat0)/14;
					break;
				case 2://二级上限180，每秒冷却36
					FRICT_SPEED=750;//射速
					Shoot.Freq=10;//射频；
					Shoot.Num=(240-Judge_RobotPowerHeatData.shooterHeat0)/18;
					break;
				case 3://三级上限360，每秒冷却72
					FRICT_SPEED=800;//射速
					Shoot.Freq=10;//射频；
					Shoot.Num=(480-Judge_RobotPowerHeatData.shooterHeat0)/20;
					break;
			}
		}
		else
		{
			switch(Judge_RobotState.robotLevel)//机器人当前等级
			{
				case 0:
				case 1://一级上限 90，每秒冷却18
					FRICT_SPEED=1000;//射速
					if(Judge_RobotRfidDetect.cardType&0x0100)//冷却值加成
						Shoot.Freq=8;
					else
						Shoot.Freq=5;//射频；
					Shoot.Num=(120-Judge_RobotPowerHeatData.shooterHeat0)/22;
					break;
				case 2://二级上限180，每秒冷却36
					FRICT_SPEED=1000;//射速
					if(Judge_RobotRfidDetect.cardType&0x0100)
						Shoot.Freq=10;
					else
						Shoot.Freq=6;//射频；
					Shoot.Num=(240-Judge_RobotPowerHeatData.shooterHeat0)/22;
					break;
				case 3://三级上限360，每秒冷却72
					FRICT_SPEED=1000;
					if(Judge_RobotRfidDetect.cardType&0x0100)
						Shoot.Freq=10;
					else
						Shoot.Freq=8;//射频；
					  Shoot.Num=(480-Judge_RobotPowerHeatData.shooterHeat0)/22;
					break;
			}
		} 
		if(Shoot.Num>2&&Shoot.Flag==1&&Mode!=SUPPLY)
		{
			Shoot.Flag=0;
			M2006.Target_Angle=M2006.Target_Angle>Rammer_Max_Angle?0:M2006.Target_Angle+St_Serve;
		}
		
	}
	else
	{
		Shoot.Time=0;
		Shoot.Flag=1;
	}
	
	M2006.Target_Angle=M2006.Target_Angle>Rammer_Max_Angle?0:M2006.Target_Angle;
	M2006_PID_Set();

}

 /*
  * @brief 拨弹电机减速比36：1，编码器转换函数
  * @param None
  * @retval None
  */
void Rammer_Angle()
{
	static int16_t error=0;
	
	M2006.Now_Angle=M2006.Real_Angle;
	M2006.Error_Angle=M2006.Now_Angle-M2006.Last_Angle;//获得两次编码器数据差值
	
	if(M2006.Real_Speed>0)
	{
		if(M2006.Error_Angle<0)//如果速度大于0，差值为负数表示过零点，加上8191
		{
			error=M2006.Error_Angle+8191;
			
		}
		else if(M2006.Error_Angle>=0)
		{
			error=M2006.Error_Angle;
			
		}
		if(M2006.Error_Angle>=0)//把差值累加
		M2006.All_error_Angle=M2006.All_error_Angle>Rammer_Max_Angle?M2006.All_error_Angle-Rammer_Max_Angle:M2006.All_error_Angle+error;
	}
	else if(M2006.Real_Speed<0)
	{
		if(M2006.Error_Angle>0)
		{
			error=M2006.Error_Angle-8191;
		
		}
		else if(M2006.Error_Angle<=0)
		{
			error=M2006.Error_Angle;
			
		}
		if(M2006.Error_Angle<=0)
			
		M2006.All_error_Angle=M2006.All_error_Angle<0?M2006.All_error_Angle+Rammer_Max_Angle:M2006.All_error_Angle+error;
	}
	
	M2006.Last_Angle=M2006.Now_Angle;//现在的值赋给上一次的值
}

 /*
  * @brief 拨弹电机PID计算
  * @param None
  * @retval None
  */
void M2006_PID_Set()
{
	float Current1=0,Current2=0;
	
	Current1=pid_calc(&M2006.PID.Out,M2006.All_error_Angle,M2006.Target_Angle);
	Current2=pid_calc(&M2006.PID.In,M2006.Real_Speed,Current1);
	M2006.Target_Current=Current2;
}

