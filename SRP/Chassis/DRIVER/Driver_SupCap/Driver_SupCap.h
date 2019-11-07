#ifndef __DRIVER_SUPCAP_H
#define __DRIVER_SUPCAP_H
#include "control.h"

typedef enum
{
		enable					=0x01,//表示是否是使能充电
		unenable				,	//表示不使能充电芯片			
		cap_supply			,//表示超级电容供电，默认是电源供电
		battery_supply  ,//电池给底盘供电
		_2A_current			,//表示启用2A电流对超级电容充电
		_05A_current		,//表示启用0.5A电流对超级电容充电
		under_voltage		,//表示超级电容电压低于78%
		charge_complete ,//表示超级电容电压高于96%
		unenable_cap		, //超级电容失效不使用超级电容模块
		enable_cap        //启用超级电容模块
}Sup_Cap_State;

/*
	PA3--------------使能充电芯片引脚
	PF10-------------供电切换引脚
	PA5--------------电流设置引脚
	PC5--------------满电检测引脚
	PC4--------------低压检测引脚
*/

///////////////////////使能充电芯片////////////////////////////////////////////////////
#define CAP_CHARGING(x)  		if(x==enable||x==unenable){\
															if(x==enable)\
															{GPIOE->BSRR=1<<21;}\
															else{GPIOE->BSRR=1<<5;}	}															
///////////////////////切换电源////////////////////////////////////////////////////													
#define CAP_POWER_SWITCH(x)  if(x==battery_supply||x==cap_supply){\
															if(x==cap_supply)\
															{GPIOC->BSRR=1<<4;}\
															else{GPIOC->BSRR=1<<20;}}
/////////////////////////设置电流//////////////////////////////////////////////////
#define CAP_SET_CURRENT(x)  if(x==_2A_current||x==_05A_current){\
															if(x==_2A_current)\
															{GPIOE->BSRR=1<<6;}\
															else{GPIOE->BSRR=1<<22;}}
#define CAP_READ_E        (GPIOE->IDR&=1<<5)>>5								//读使能芯片引脚
#define CAP_READ_S        (GPIOC->IDR&=1<<4)>>4					     	//读电源切换引脚					
#define CAP_READ_C        (GPIOE->IDR&=1<<6)>>6 							//读电流设置引脚
#define CAP_READ_V        (GPIOC->IDR&=3<<2)>>2 							//读电压阈值引脚			
void Init_SupCap(void);
void Read_Cap_State(void);
void Cap_Mode(void);
void Normal_Mode(void);
void Cap_Casual(void);
															
extern uint8_t Sup_Cap,CAP_OFF;
#endif

