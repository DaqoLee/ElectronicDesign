#include "Ctrl_Frict.h"
uint16_t FRICT_SPEED=800;//摩擦轮速度

 /*
  * @brief 摩擦轮与激光参数设置
  * @param None
  * @retval None
  */
void Frict_Param_Set()//摩擦轮
{
	static uint8_t Flag1=0,Flag2=0;

	if((DBUS_CheckPush(KEY_V)&&DBUS_CheckPush(KEY_CTRL)))
	{
		if(Flag1)
		{
			Flag2=!Flag2;
			Flag1=0;
		}
	}
    else 
	{	
		Flag1=1;
	}

	if((Flag2||(DBUS_ReceiveData.switch_right==3&&DBUS_ReceiveData.switch_left!=1))&&(Mode!=SUPPLY)&&DBUS_ReceiveData.switch_right!=2)
	{//摩擦轮速度缓慢加速上去，新主控板启动太快会导致过流保护
		TIM1->CCR1=TIM1->CCR1>=1000+FRICT_SPEED?1000+FRICT_SPEED:TIM1->CCR1+5;
		TIM1->CCR2=TIM1->CCR2>=1000+FRICT_SPEED?1000+FRICT_SPEED:TIM1->CCR2+5;
		TIM1->CCR3=TIM1->CCR3>=1000+FRICT_SPEED?1000+FRICT_SPEED:TIM1->CCR3+5;
		TIM1->CCR4=TIM1->CCR4>=1000+FRICT_SPEED?1000+FRICT_SPEED:TIM1->CCR4+5;
		HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_SET);//开激光	
	}
	else
	{
		TIM1->CCR1=1000;
		TIM1->CCR2=1000;
		TIM1->CCR3=1000;
		TIM1->CCR4=1000;
		HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_RESET);//关激光	
	}
	
}



