#include "Ctrl_Chassis.h"
#include "control.h"
#include "User_can.h"
#include "Driver_SupCap.h"
#include "Ctrl_Cloud.h"
#include "User.h"
#include "string.h"
float Buffer[4];
uint16_t MaxWheelSpeed=6600;

 /*
  * @brief 麦克纳姆伦运动模型
  * @param Vx X轴方向的速度，Vy Y轴方向的速度，Omega 自旋速度，*Speed轮子转速
  * @retval None
  */
void MecanumCalculate(float Vx, float Vy, float Omega, int16_t *Speed)
{
    float  Param, MaxSpeed;
    uint8_t index;
    
    Buffer[0] = -Vx - Vy + Omega;
    Buffer[1] = -Vx + Vy + Omega;
    Buffer[2] =  Vx + Vy + Omega;
    Buffer[3] =  Vx - Vy + Omega;
    
    //限幅
    for(index = 0, MaxSpeed = 0; index < 4; index++)
    {
        if((Buffer[index] > 0 ? Buffer[index] : -Buffer[index]) > MaxSpeed)
        {
            MaxSpeed = (Buffer[index] > 0 ? Buffer[index] : -Buffer[index]);//挑出最大的速度
        }
    }	
	
	if((MEDIAN_ROLL-CloudParam.Cloud_Gyro.Roll)-((CloudParam.Pitch.Real_Angle-(MEDIAN_PITCH-Pitch_Min-200))/22.75f)>12)
		MaxWheelSpeed=2000;//云台陀螺仪角度变化大于Pitch轴编码器的转换值，被认为是爬坡，把速度降下来
	else
		MaxWheelSpeed=6600;
	
    if(MaxWheelSpeed < MaxSpeed)
    {
        Param = (float)MaxWheelSpeed / MaxSpeed;
        Speed[0] = Buffer[0] * Param;
        Speed[1] = Buffer[1] * Param;
        Speed[2] = Buffer[2] * Param;
        Speed[3] = Buffer[3] * Param; 
    }
    else
    {
        Speed[0] = Buffer[0];
        Speed[1] = Buffer[1];
        Speed[2] = Buffer[2];
        Speed[3] = Buffer[3];
    }
}

 /*
  * @brief 底盘功率限制
  * @param None
  * @retval None
  */
void Power_Limit(float Cur_limit)
{
	static float Current;
    //计算四个电机的总电流
	Current=ABS(ChassisParam.LF.Target_Current)+ABS(ChassisParam.LB.Target_Current)\
		   +ABS(ChassisParam.RB.Target_Current)+ABS(ChassisParam.RF.Target_Current);
	
	if(Current>Cur_limit)
	{//根据目标值比例分配电流
		ChassisParam.LF.Target_Current = ChassisParam.LF.Target_Current/Current*Cur_limit;
		ChassisParam.LB.Target_Current = ChassisParam.LB.Target_Current/Current*Cur_limit;
		ChassisParam.RB.Target_Current = ChassisParam.RB.Target_Current/Current*Cur_limit;
		ChassisParam.RF.Target_Current = ChassisParam.RF.Target_Current/Current*Cur_limit;
	}
}

 /*
  * @brief 底盘电机PID计算
  * @param None
  * @retval None
  */
void M3508_PID_Set()
{
	float Current=0;
	
	//底盘左前轮
	Current=pid_calc(&ChassisParam.LF.PID,ChassisParam.LF.Real_Speed,ChassisParam.LF.Target_Speed);
	ChassisParam.LF.Target_Current=Current;
	
	//底盘左后轮
	Current=pid_calc(&ChassisParam.LB.PID,ChassisParam.LB.Real_Speed,ChassisParam.LB.Target_Speed);
	ChassisParam.LB.Target_Current=Current;
	
	//底盘右前轮
	Current=pid_calc(&ChassisParam.RF.PID,ChassisParam.RF.Real_Speed,ChassisParam.RF.Target_Speed);
	ChassisParam.RF.Target_Current=Current;
	
	//底盘右后轮
	Current=pid_calc(&ChassisParam.RB.PID,ChassisParam.RB.Real_Speed,ChassisParam.RB.Target_Speed);
	ChassisParam.RB.Target_Current=Current;
	
}

 /*
  * @brief 底盘参数设置
  * @param X与Y轴的最大速度
  * @retval None
  */
void Chassis_Param_Set(uint16_t Max_X,uint16_t Max_Y)//底盘
{
    static int16_t speed_x=0,speed_y=0;
	static int16_t Max_speed_x=0,Max_speed_y=0;
	static float Cur_limit;
	static int8_t Spin_Flag=1;
	int16_t WheelSpeed[4]; 
	if(!Remote.Offline)
	{
		if(DBUS_ReceiveData.switch_right==3)//遥控模式
		{
			if(ABS(DBUS_ReceiveData.ch4)>20)//遥控中间时有误差，不一定为零
			{
				ChassisParam.TargetVY=10*DBUS_ReceiveData.ch4;
				if(!CloudParam.Yaw.Offline)
				{   //底盘跟随，Yaw轴编码器的实际值往中间值靠
					Filter(pid_calc(&ChassisParam.Chassis_Gyro.Chassis_PID,CloudParam.Yaw.Real_Angle,MEDIAN_YAW),&ChassisParam.TargetOmega,0.2f);
					if(ABS(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW])<100||CloudParam.Cloud_Gyro.Offline)
						ChassisParam.TargetOmega=0;
					//防止跑动时云台抖动影响车身，允许机械角度有200的抖动范围
				}
				else//如果Yaw轴电机离线直接控制底盘	
					ChassisParam.TargetOmega=ABS(DBUS_ReceiveData.ch1)>20?10*DBUS_ReceiveData.ch1:0;
			}
			else
				ChassisParam.TargetVY=0;
			
			if(ABS(DBUS_ReceiveData.ch3)>20)
			{
				ChassisParam.TargetVX=-10*DBUS_ReceiveData.ch3;
				if(!CloudParam.Yaw.Offline)
				{
					Filter(pid_calc(&ChassisParam.Chassis_Gyro.Chassis_PID,CloudParam.Yaw.Real_Angle,MEDIAN_YAW),&ChassisParam.TargetOmega,0.2f);
					if(ABS(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW])<100||CloudParam.Cloud_Gyro.Offline)
						ChassisParam.TargetOmega=0;
				}
				else
					ChassisParam.TargetOmega=ABS(DBUS_ReceiveData.ch1)>20?10*DBUS_ReceiveData.ch1:0;

			}
			else
				ChassisParam.TargetVX=0;
		
			if(ChassisParam.TargetVX==0&&ChassisParam.TargetVY==0)
			{
				if(!CloudParam.Yaw.Offline)
				{   //静止时，90度范围内不跟随
					Filter(pid_calc(&ChassisParam.Chassis_Gyro.Chassis_PID,CloudParam.Yaw.Real_Angle,MEDIAN_YAW),&ChassisParam.TargetOmega,0.06f);
					if(ABS(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW])<1024||CloudParam.Cloud_Gyro.Offline)
						ChassisParam.TargetOmega=0;
				}
				else
					ChassisParam.TargetOmega=ABS(DBUS_ReceiveData.ch1)>20?10*DBUS_ReceiveData.ch1:0;		
			}
		}
		else if(DBUS_ReceiveData.switch_right==1)//键鼠模式
		{	
			if(Mode!=HIEROGRAM)
			{
				if((DBUS_CheckPush(KEY_D)||DBUS_CheckPush(KEY_A)||DBUS_CheckPush(KEY_S)||DBUS_CheckPush(KEY_W))&&DBUS_CheckPush(KEY_SHIFT))
				{	//按住Shift加速		
					if(CAP_READ_S)//超级电容满电，最大速度提高
					{
						Max_speed_x=6500;
						Max_speed_y=6500; 
					}
					else
					{
						Max_speed_x=5500;
						Max_speed_y=5500; 
					}
				}
				else if((DBUS_CheckPush(KEY_D)||DBUS_CheckPush(KEY_A)||DBUS_CheckPush(KEY_S)||DBUS_CheckPush(KEY_W))&&DBUS_CheckPush(KEY_CTRL))
				{   //按住Ctrl减速
					Max_speed_x=2000;
					Max_speed_y=2000; 
				}
				else
				{
					Max_speed_x=Max_speed_x<=Max_X?Max_X:Max_speed_x-20;
					Max_speed_y=Max_speed_y<=Max_Y?Max_Y:Max_speed_y-20;
				}	
				
				if(DBUS_CheckPush(KEY_W))
				{
					speed_y=speed_y<0?speed_y+100:speed_y;
					speed_y=speed_y>=Max_speed_y?Max_speed_y:speed_y+50;//缓慢加速
					if(!(DBUS_CheckPush(KEY_CTRL))&&!CloudParam.Cloud_Gyro.Offline&&Mode==COMMON)
						{
							if(DBUS_CheckPush(KEY_F))//蛇皮走位
							{		
								pid_calc(&ChassisParam.Chassis_Gyro.Chassis_PID,CloudParam.Yaw.Real_Angle,MEDIAN_YAW);
								
								if(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW]>400)						
									Spin_Flag=1;
								else if(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW]<-400)
									Spin_Flag=-1;
								
								Filter(Spin_Flag*2000,&ChassisParam.TargetOmega,0.04f);
							}
							else//普通跟随
							{
								ChassisParam.TargetOmega=pid_calc(&ChassisParam.Chassis_Gyro.Chassis_PID,CloudParam.Yaw.Real_Angle,MEDIAN_YAW);
								if(ABS(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW])<100)
								ChassisParam.TargetOmega=0;
							}
						}
						else
							ChassisParam.TargetOmega=0;		
				}
				else if(DBUS_CheckPush(KEY_S))
				{
					speed_y=speed_y>0?speed_y-100:speed_y;
					speed_y=speed_y<=-Max_speed_y?-Max_speed_y:speed_y-50;
					if(!(DBUS_CheckPush(KEY_CTRL))&&!CloudParam.Cloud_Gyro.Offline&&Mode==COMMON)
						{
							ChassisParam.TargetOmega=pid_calc(&ChassisParam.Chassis_Gyro.Chassis_PID,CloudParam.Yaw.Real_Angle,MEDIAN_YAW);
							if(ABS(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW])<100)
								ChassisParam.TargetOmega=0;	
						}
					else
						ChassisParam.TargetOmega=0;		
				}
				 else
				{
					speed_y=speed_y<0?speed_y+100:speed_y-100;//缓慢减速
					if(ABS(speed_y)<200)
					{
						speed_y=0;
					}
				}
				
				if(!DBUS_CheckPush(KEY_F))//没有按下F，正常跟随
				{
						if(!(DBUS_CheckPush(KEY_D)||DBUS_CheckPush(KEY_A)||DBUS_CheckPush(KEY_S)||DBUS_CheckPush(KEY_W)))
						{
							if(!(DBUS_CheckPush(KEY_CTRL))&&!CloudParam.Cloud_Gyro.Offline&&Mode==COMMON)
								{
									pid_calc(&ChassisParam.Chassis_Gyro.Chassis_PID,CloudParam.Yaw.Real_Angle,MEDIAN_YAW);
									if(ABS(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW])<1024)
										ChassisParam.TargetOmega=0;	
									else
										Filter(pid_calc(&ChassisParam.Chassis_Gyro.Chassis_PID,CloudParam.Yaw.Real_Angle,MEDIAN_YAW),&ChassisParam.TargetOmega,0.05f);
								}
									else
										ChassisParam.TargetOmega=0;	
						}
				}
				else if(!DBUS_CheckPush(KEY_W))//按下F，且不前进时扭腰
				{		
					pid_calc(&ChassisParam.Chassis_Gyro.Chassis_PID,CloudParam.Yaw.Real_Angle,MEDIAN_YAW);
					
					if(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW]>600)				
						Spin_Flag=1;
					else if(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW]<-600)
						Spin_Flag=-1;
					
					if(Spin_Flag==1)
						ChassisParam.TargetOmega=ChassisParam.TargetOmega>3000?3000:ChassisParam.TargetOmega+200;
					else 
						ChassisParam.TargetOmega=ChassisParam.TargetOmega<-3000?-3000:ChassisParam.TargetOmega-200;
				}
				
				if(DBUS_CheckPush(KEY_D))
				{
					speed_x=speed_x>0?speed_x-100:speed_x;
					speed_x=speed_x<=-Max_speed_x?-Max_speed_x:speed_x-50;
					
					if(!(DBUS_CheckPush(KEY_CTRL))&&!CloudParam.Cloud_Gyro.Offline&&Mode==COMMON)
					{
						ChassisParam.TargetOmega=pid_calc(&ChassisParam.Chassis_Gyro.Chassis_PID,CloudParam.Yaw.Real_Angle,MEDIAN_YAW);
						if(ABS(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW])<100)
							ChassisParam.TargetOmega=0;
					}
				}
				else if(DBUS_CheckPush(KEY_A))
				{
					speed_x=speed_x<0?speed_x+100:speed_x;
					speed_x=speed_x>=Max_speed_x?Max_speed_x:speed_x+50;
					
					if(!(DBUS_CheckPush(KEY_CTRL))&&!CloudParam.Cloud_Gyro.Offline&&Mode==COMMON)
					{
						ChassisParam.TargetOmega=pid_calc(&ChassisParam.Chassis_Gyro.Chassis_PID,CloudParam.Yaw.Real_Angle,MEDIAN_YAW);
						if(ABS(ChassisParam.Chassis_Gyro.Chassis_PID.err[NOW])<100)
							ChassisParam.TargetOmega=0;
					}
				}
				else
				{
					speed_x=speed_x<0?speed_x+100:speed_x-100;
					if(ABS(speed_x)<200)
					{
						speed_x=0;
					}
	
				}
		if(Mode==SUPPLY)//补给模式不跟随，Q，E自旋
		{
				if(DBUS_CheckPush(KEY_Q))
					 ChassisParam.TargetOmega=-500;//补给时低速
				
				else if(DBUS_CheckPush(KEY_E))
				
					ChassisParam.TargetOmega=500;
				
				else
					ChassisParam.TargetOmega=0;
		 }
				Filter(speed_x,&ChassisParam.TargetVX,0.1f);//缓冲平滑处理
				Filter(speed_y,&ChassisParam.TargetVY,0.1f);

		}
		else//手动神符，速度为零
		{
			ChassisParam.TargetVX=0;
			ChassisParam.TargetVY=0;
		}
	 }
	}
   else	//遥控离线，速度全部为零
	{
		ChassisParam.TargetVX=0;
		ChassisParam.TargetVY=0;
		ChassisParam.TargetOmega=0;		
	}
	
	MecanumCalculate(ChassisParam.TargetVX,ChassisParam.TargetVY,ChassisParam.TargetOmega,WheelSpeed);
	//麦轮解算
	ChassisParam.LF.Target_Speed = WheelSpeed[0];
    ChassisParam.LB.Target_Speed = WheelSpeed[1];
    ChassisParam.RB.Target_Speed = WheelSpeed[2];
    ChassisParam.RF.Target_Speed = WheelSpeed[3];
	
	M3508_PID_Set();
	
	if(Judge_Power_Limit==1||Meter_Power_Limit==1)
	{//缓冲功率低于50
		
		if(!Judge.Offline||!Cur_Meter.FrameRate)//电流计或裁判系统数据正常，进行功率限制
			Cur_limit=100000.0f/Judge_RobotPowerHeatData.chassisVolt;
		else
			Cur_limit=3000.0f;
		 Power_Limit(Cur_limit);
	}
	else if(Judge_Power_Limit==2||Meter_Power_Limit==2)
	{//缓冲功率低于30	
		if(!Judge.Offline||!Cur_Meter.FrameRate)
			Cur_limit=80000.0f/Judge_RobotPowerHeatData.chassisVolt;
		else
			Cur_limit=3000.0f;
		 Power_Limit(Cur_limit);
	}
	
}

void Chassis(void)
{
	int16_t WheelSpeed[4];
	switch(Esp8266_Flag)
	{
		case '1':
			SRP.TargetX=800;
			SRP.TargetY=500;
			if(strcmp(RFID_RC522,"B1C5A031")==0)
				SRP.Stop_Flag=1;
			else
				SRP.Stop_Flag=0;	
			break;
		case '8':
			SRP.TargetX=200;
			SRP.TargetY=200;
			if(strcmp(RFID_RC522,"5CFFE029")==0)
				SRP.Stop_Flag=1;
			else
				SRP.Stop_Flag=0;
			break;
		case '9':
			SRP.TargetX=50;
			SRP.TargetY=50;
			if(strcmp(RFID_RC522,"AFACDF89")==0)
				SRP.Stop_Flag=1;
			else
				SRP.Stop_Flag=0;
			break;
		default:
		
			break;
	}
	if(DBUS_ReceiveData.switch_right==1)
	{
		SRP.Speed_X=-pid_calc(&SRP.X_PID,UWB.X,SRP.TargetX);
		SRP.Speed_Y=pid_calc(&SRP.Y_PID,UWB.Y,SRP.TargetY);
		SRP.Omega=pid_calc(&SRP.Om_PID,UWB.Angle,SRP.TargetAngle);
	}
	else
	{
		SRP.Speed_X=-2*DBUS_ReceiveData.ch3;
		SRP.Speed_Y=2*(DBUS_ReceiveData.ch2+DBUS_ReceiveData.ch4);
		SRP.Omega=2*DBUS_ReceiveData.ch1;
		SRP.TargetX=UWB.X;
		SRP.TargetY=UWB.Y;
	}
	if(SRP.Stop_Flag)
	{
		SRP.Speed_X=0;
		SRP.Speed_Y=0;
	}
	if(Ultrasonic[0].Stop_Flag==1||Ultrasonic[7].Stop_Flag==1)
		SRP.Speed_Y=SRP.Speed_Y>0?0:SRP.Speed_Y;
	if(Ultrasonic[3].Stop_Flag==1||Ultrasonic[4].Stop_Flag==1)
		SRP.Speed_Y=SRP.Speed_Y<0?0:SRP.Speed_Y;
	if(Ultrasonic[1].Stop_Flag==1||Ultrasonic[2].Stop_Flag==1)
		SRP.Speed_X=SRP.Speed_X>0?0:SRP.Speed_X;
	if(Ultrasonic[5].Stop_Flag==1||Ultrasonic[6].Stop_Flag==1)
		SRP.Speed_X=SRP.Speed_X<0?0:SRP.Speed_X;
	MecanumCalculate(SRP.Speed_X,SRP.Speed_Y,SRP.Omega,WheelSpeed);
	
//	MecanumCalculate(-2*DBUS_ReceiveData.ch3,2*(DBUS_ReceiveData.ch2+DBUS_ReceiveData.ch4),2*DBUS_ReceiveData.ch1,WheelSpeed);
	CAN_RoboModule_DRV_Velocity_Mode(0,1,2000,1000);
	CAN_RoboModule_DRV_Velocity_Mode(0,2,2000,WheelSpeed[1]);
	CAN_RoboModule_DRV_Velocity_Mode(0,3,2000,WheelSpeed[2]);
	CAN_RoboModule_DRV_Velocity_Mode(0,4,2000,WheelSpeed[3]);
}

void Dog_Text(void)
{
	ChassisParam.LF.Target_Speed=DBUS_ReceiveData.ch2*10;
	ChassisParam.LB.Target_Speed=-DBUS_ReceiveData.ch1*10;
	if(!HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_2))
	{
		ChassisParam.LF.Target_Speed=ChassisParam.LF.Target_Speed>0?0:ChassisParam.LF.Target_Speed;
	}
	if(!HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_7))
	{
		ChassisParam.LF.Target_Speed=ChassisParam.LF.Target_Speed<0?0:ChassisParam.LF.Target_Speed;
	}
	M3508_PID_Set();
	Set_moto_current(&hcan1,0x200, ChassisParam.LB.Target_Current, ChassisParam.RB.Target_Current, \
					ChassisParam.RF.Target_Current, ChassisParam.LF.Target_Current);
}

