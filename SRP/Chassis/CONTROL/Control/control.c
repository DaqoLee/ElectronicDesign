#include "control.h"
#include "Ctrl_Chassis.h"
#include "Ctrl_Cloud.h"
#include "Ctrl_Rammer.h"
#include "Ctrl_Frict.h"
#include "Driver_Display.h"
#include "User_can.h"
#include "Driver_Hierogram.h"
#include "Driver_Judge.h"
#include "Driver_SupCap.h"
#include "User.h"
#include "Driver_SupCap.h"
#include "User_ADC.h"
Current_Meter_Struct   Current_Meter;
 ChassisParam_Struct   ChassisParam;
   CloudParam_Struct   CloudParam;
     Motor2006_Param   M2006;
		  UWB_Struct   UWB;
		  SRP_Struct   SRP;
			uint8_t    Mode=COMMON;
		   uint16_t    First_Yaw_Angle,First_Pitch_Angle;//上电云台电机初始位置
              float    Mou_x,Mou_y;//鼠标滤波后的值
int16_t T;
  volatile uint16_t    Cnt_1Hz, Cnt_10Hz, Cnt_20Hz, Cnt_50Hz;	
		    uint8_t	   Flag1_1Hz, Flag1_10Hz, Flag1_20Hz, Flag1_50Hz,
					   Flag2_1Hz, Flag2_10Hz, Flag2_20Hz, Flag2_50Hz,
					   Flag3_1Hz, Flag3_10Hz, Flag3_20Hz, Flag3_50Hz;		
				   

 /*
  * @brief 定时器中断回调函数，所有任务在TIM7 10ms中断里处理
  * @param *htim发生中断的定时器结构体指针
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{	 
    if(htim == &htim7)//10ms中断一次
    {   
		Chassis();
//		Get_Adc_Average();
//		text();
		
//		T=Gezxcvbnm//		Frame_Rate_Statistics();//帧率统计	
//		Data_Display();//匿名上位机，MiniBalance，OLED数据反馈显示
//		Read_Cap_State();//超级电容器
//		Mode_Set();//模式设置 
		if((DBUS_ReceiveData.switch_right==1||DBUS_ReceiveData.switch_right==3))		
		{	
					
				Dog_Text();
	
//			switch(Mode)
//			{
///*****************************一般模式**********************************************/
//				case    COMMON:						
//							CloudParam.Yaw.PID.Out.f_pid_reset(&CloudParam.Yaw.PID.Out,2.2f,0,0);
//							CloudParam.Pitch.PID.Out.f_pid_reset(&CloudParam.Pitch.PID.Out,6.5f,0,0);						
//							Hierogram_flag=0;
//							Cloud_Param_Set();//云台				
//							Chassis_Param_Set(4000,4000);//底盘，最大速度4000
//							Rammer_Param_Set();//拨弹
//							Frict_Param_Set();//摩擦轮
//							if((Judge_RobotGetBuff.buffType&0x01)&&DBUS_CheckPush(KEY_CTRL))
//								Cap_Casual();//补血点按住Ctrl大电流给超级电容充电
//							else
//								Cap_Mode();//超级电容0.5A充电
//							LED_Set(&Flag1_20Hz);//LED状态灯闪烁
//							break;
///*****************************补给模式**********************************************/				
//				case    SUPPLY:
//							CloudParam.Yaw.PID.Out.f_pid_reset(&CloudParam.Yaw.PID.Out,2,0,0);
//							CloudParam.Pitch.PID.Out.f_pid_reset(&CloudParam.Pitch.PID.Out,6.5,0,0);
//							Cloud_Target_Angle(MEDIAN_PITCH-500,MEDIAN_YAW);
//							Cloud_Param_Set();//云台
//							Chassis_Param_Set(800,800);//底盘
//							Rammer_Param_Set();//拨弹
//							Frict_Param_Set();//摩擦轮
//							LED_Set(&Flag1_20Hz);	
//							Cap_Casual();//补给模式大电流给超级电容充电
//							break;				
///*****************************神符模式**********************************************/						
//	 		case HIEROGRAM:
//							Cloud_Target_Angle(DBUS_ReceiveData.mouse.y,DBUS_ReceiveData.mouse.x);
//							CloudParam.Yaw.PID.Out.f_pid_reset(&CloudParam.Yaw.PID.Out,4,0,0);
//                         	CloudParam.Pitch.PID.Out.f_pid_reset(&CloudParam.Pitch.PID.Out,5,0,2);
//							Hierogram_Get();//获取小电脑数据
//							Cloud_Param_Set();//云台
//							Chassis_Param_Set(0,0);//底盘
//						    Frict_Param_Set();//摩擦轮
//							Rammer_Param_Set();//拨弹
//							LED_Set(&Flag1_20Hz);
//							break;
//				default:
//							break;
//			}
//			//写入电流
//			Set_moto_current(&hcan1,0x200, ChassisParam.LB.Target_Current, ChassisParam.RB.Target_Current, \
//					ChassisParam.RF.Target_Current, ChassisParam.LF.Target_Current);
//			Set_moto_current(&hcan1,0x1FF, CloudParam.Yaw.Target_Current, CloudParam.Pitch.Target_Current, \
//					M2006.Target_Current, 0);
		}			
		else
		{
			Cap_Casual();//大电流给超级电容充电
			Frict_Param_Set();
			Set_moto_current(&hcan1,0x200, 0, 0, 0, 0);	
			Set_moto_current(&hcan1,0x1FF, 0, 0, 0, 0);	
		}
//			
//		
//		if(Flag1_10Hz)//操作界面显示数据，官方要求10Hz
//		{
//			//如果超级电容有电，界面6个灯全部亮（绿色）
//			Flag1_10Hz=0;
//			Transmit_Judge(22,ChassisParam.LB.Real_Speed,Shoot.Num,Judge_RobotPowerHeatData.chassisPowerBuffer,!CAP_OFF|(0xff*(CAP_READ_S))<<1);
//		}
	}
    else if(htim == &htim6)//1ms中断一次
    {
		if(++Cnt_1Hz>=1000)//1000ms
		{  
			Cnt_1Hz=0;
			Flag1_1Hz=1;
			Flag2_1Hz=1;
			Flag3_1Hz=1;//帧率统计频率
		}	
		if(++Cnt_10Hz>=100)//100ms
		{
			Cnt_10Hz=0;
			Flag1_10Hz=1;//操作界面数据刷新频率
			Flag2_10Hz=1;//调试的OLED等刷新频率
			Flag3_10Hz=1;
		}
		
		if(++Cnt_20Hz>=50)//50ms
		{
			Cnt_20Hz=0;
			Flag1_20Hz=1;//主控LED闪烁频率
			Flag2_20Hz=1;
			Flag3_20Hz=1;
		}
		if(++Cnt_50Hz>=20)//20ms
		{
			Cnt_50Hz=0;
			Flag1_50Hz=1;
			Flag2_50Hz=1;
			Flag3_50Hz=1;
		}
		if(++Shoot.Time>=1000/Shoot.Freq)//连发频率
		{
			Shoot.Time=0;
			Shoot.Flag=1;
		}
    }
}

 /*
  * @brief 正常，打符，补给模式设置
  * @param None
  * @retval None
  */
void Mode_Set()
{
	if(DBUS_CheckPush(KEY_CTRL)&&DBUS_CheckPush(KEY_C))
	{
		 Mode=COMMON;
	}
	else if(DBUS_CheckPush(KEY_SHIFT)&&DBUS_CheckPush(KEY_X))
	{
		 Mode=HIEROGRAM;
	}
	else if(DBUS_CheckPush(KEY_CTRL)&&DBUS_CheckPush(KEY_X))
	{
		 Mode=SUPPLY;
	}
}

 /*
  * @brief 外设报文返回帧率统计，频率1Hz
  * @param None
  * @retval None
  */
void Frame_Rate_Statistics()//帧率统计1000ms一次
{
if(Flag3_1Hz)
{
	Flag3_1Hz=0;
	
	if(CloudParam.Gyro.FrameRate==0)
		CloudParam.Gyro.Offline=1;
	else
		CloudParam.Gyro.Offline=0;
	
	if(CloudParam.Pitch.FrameRate==0)
		CloudParam.Pitch.Offline=1;
	else
		CloudParam.Pitch.Offline=0;
	
	if(CloudParam.Yaw.FrameRate==0)
		CloudParam.Yaw.Offline=1;
	else
		CloudParam.Yaw.Offline=0;
	
	if(CloudParam.Cloud_Gyro.FrameRate==0)
	{
		CloudParam.Cloud_Gyro.Offline=1;
		HAL_GPIO_WritePin(LED_1_GPIO_Port,LED_1_Pin,GPIO_PIN_RESET);
	}
	else
	{
		CloudParam.Cloud_Gyro.Offline=0;
		HAL_GPIO_WritePin(LED_1_GPIO_Port,LED_1_Pin,GPIO_PIN_SET);
	}
/******************************************************************/	
	if(ChassisParam.LB.FrameRate==0)
		ChassisParam.LB.Offline=1;
	else
		ChassisParam.LB.Offline=0;
	
	if(ChassisParam.LF.FrameRate==0)
		ChassisParam.LF.Offline=1;
	else
		ChassisParam.LF.Offline=0;
	
	if(ChassisParam.RB.FrameRate==0)
		ChassisParam.RB.Offline=1;
	else
		ChassisParam.RB.Offline=0;
	
	if(ChassisParam.RF.FrameRate==0)
		ChassisParam.RF.Offline=1;
	else
		ChassisParam.RF.Offline=0;
	
	if(ChassisParam.Chassis_Gyro.FrameRate==0)
		ChassisParam.Chassis_Gyro.Offline=1;
	else
		ChassisParam.Chassis_Gyro.Offline=0;
/******************************************************************/		
	if(M2006.FrameRate==0)
		M2006.Offline=1;
	else
		M2006.Offline=0;
	
	if(Judge.FrameRate==0)
	{
		Judge.Offline=1;
		HAL_GPIO_WritePin(LED_2_GPIO_Port,LED_2_Pin,GPIO_PIN_RESET);
	}
	else
	{
		Judge.Offline=0;
		HAL_GPIO_WritePin(LED_2_GPIO_Port,LED_2_Pin,GPIO_PIN_SET);
	}
	
	if(Remote.FrameRate==0)
		Remote.Offline=1;
	else
		Remote.Offline=0;
	
	if(PC_Data.FrameRate==0)
		PC_Data.Offline=1;
	else
		PC_Data.Offline=0;
	
	if(Cur_Meter.FrameRate==0)
	{
		Cur_Meter.Offline=1;
		HAL_GPIO_WritePin(LED_3_GPIO_Port,LED_3_Pin,GPIO_PIN_RESET);
	}
	else
	{
		Cur_Meter.Offline=0;
		HAL_GPIO_WritePin(LED_3_GPIO_Port,LED_3_Pin,GPIO_PIN_SET);
	}
/******************************************************************/	
	
	CloudParam.Gyro.FrameRate=0;
	CloudParam.Pitch.FrameRate=0;
	CloudParam.Yaw.FrameRate=0;
	CloudParam.Cloud_Gyro.FrameRate=0;
	
	ChassisParam.LB.FrameRate=0;
	ChassisParam.LF.FrameRate=0;
	ChassisParam.RB.FrameRate=0;
	ChassisParam.RF.FrameRate=0;
	ChassisParam.Chassis_Gyro.FrameRate=0;
	
	M2006.FrameRate=0;
	Judge.FrameRate=0;
	Remote.FrameRate=0;
	PC_Data.FrameRate=0;
	Cur_Meter.FrameRate=0;
}

}

 /*
  * @brief 滤波函数，数据平滑处理
  * @param Val 滤波前，*Value滤波后
  * @retval None
  */
void Filter(int16_t Val,float *Value,float Rate)
{

	*Value+=Rate*(Val-*Value);
	
}
