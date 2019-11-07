#include "Driver_SupCap.h"
uint8_t Sup_Cap,CAP_OFF=1;
/*
 0位------------芯片使能状态         1为使能
 1位------------低压检测引脚         1为低压
 2位------------满电检测引脚         1为满电
 3位------------充电电流设置检测引脚  1为2A充电
 4位------------供电源检测引脚        1为超级电容向底盘供电 
*/
void Init_SupCap(void)
{
		Read_Cap_State();
}

/*
	读取超级电容状态
*/
void Read_Cap_State(void)
{
		uint8_t capParam=0;
		capParam|=((~CAP_READ_E)<<0)&0x01;
		capParam|=((~CAP_READ_V)<<1)&0x06;
		capParam|=CAP_READ_C<<3;
		capParam|=CAP_READ_S<<4;
		Sup_Cap=capParam;
}
/*
	超级电容模式，超级电容充满电自动切换超级电容供电
*/
void Cap_Mode(void)
{
	static uint8_t Flag1=0;
	if((DBUS_CheckPush(KEY_R)&&DBUS_CheckPush(KEY_SHIFT)))
	{
		if(Flag1)
		{
			CAP_OFF=!CAP_OFF;
			Flag1=0;
		}
	}
    else 
	{	
		Flag1=1;
	}
if(CAP_OFF)
{
	  CAP_CHARGING(unenable);
  	HAL_GPIO_WritePin(LED_8_GPIO_Port,LED_8_Pin,GPIO_PIN_SET);
}
else
{
		CAP_CHARGING(enable);
	  HAL_GPIO_WritePin(LED_8_GPIO_Port,LED_8_Pin,GPIO_PIN_RESET);
}

		if(Sup_Cap&0x01)
		{
				if(Sup_Cap&0x02&&Sup_Cap&0x10)//低压底盘电源切回电池供电
				{
					  HAL_GPIO_WritePin(LED_7_GPIO_Port,LED_7_Pin,GPIO_PIN_SET);
						CAP_POWER_SWITCH(battery_supply);
						CAP_SET_CURRENT(_05A_current);
				}
			  if(Sup_Cap&0x04&&(Sup_Cap&0x10)==0x00)
				{
					  HAL_GPIO_WritePin(LED_7_GPIO_Port,LED_7_Pin,GPIO_PIN_RESET);
						CAP_POWER_SWITCH(cap_supply);
						CAP_SET_CURRENT(_2A_current);
				}
		}
		else//失效充电管理时地盘电源切回电池来供电
		{
				CAP_POWER_SWITCH(battery_supply);
				CAP_SET_CURRENT(_05A_current);
		}
}
/*
	正常模式启动超级电容充电管理，并以0.5A充电
*/
void Normal_Mode(void)
{
		CAP_CHARGING(enable);
		CAP_POWER_SWITCH(battery_supply);
		CAP_SET_CURRENT(_05A_current);
}
/*
	超级电容充满电就关闭充电管理，以减小发热
*/
void Cap_Casual(void)//休闲模式
{
		////////////超级电容充电
//		CAP_CHARGING(enable);			
		if(Sup_Cap&0x01)
		{
				CAP_POWER_SWITCH(cap_supply);
				if((Sup_Cap&0x04)==0)
				{
						
						CAP_SET_CURRENT(_2A_current);
				}
				else
				{
						CAP_CHARGING(unenable);
						CAP_SET_CURRENT(_05A_current);
				}
		}
		else//不使能芯片时切回电池供电
		{
				CAP_POWER_SWITCH(battery_supply);
		}
}
