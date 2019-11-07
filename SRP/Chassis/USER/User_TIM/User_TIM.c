#include "User_TIM.h"
#include "tim.h"
void TIM_Init()
{

  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);//TIM1 通道1使能（摩擦轮）
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);//TIM1 通道2使能（摩擦轮）
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);//TIM1 通道1使能（摩擦轮）
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);//TIM1 通道2使能（摩擦轮）
  HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim6);//定时器6中断使能 1  ms
  HAL_TIM_Base_Start_IT(&htim7);//定时器7中断使能 10 ms
	
}

