/**
  ************************************* Copyright ****************************** 
  *
  * (C) Copyright 2018,Daqo Lee,China, GCU.
  *		All Rights Reserved
  *                              
  * By(ZHE WOLF TEAM OF GCU ROBOT)
  * https://github.com/GCUWildwolfteam
  *    
  * FileName   : Ultrasonic.c   
  * Version    : v1.0		
  * Author     : Daqo Lee			
  * Date       : 2018-11-04         
  * Description:    
  * Function List:  
  	1. ....
  	   <version>: 		
  <modify staff>:
  		  <data>:
   <description>:  
  	2. ...
  ******************************************************************************
 */

#include "Ultrasonic.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "User.h"
uint16_t Distance1,Distance2,Distance3,Distance4;
uint16_t TIM2CH1_CAPTURE_STA,TIM2CH1_CAPTURE_VAL;
uint16_t TIM2CH2_CAPTURE_STA,TIM2CH2_CAPTURE_VAL;
uint16_t TIM2CH3_CAPTURE_STA,TIM2CH3_CAPTURE_VAL;
uint16_t TIM2CH4_CAPTURE_STA,TIM2CH4_CAPTURE_VAL;

uint16_t Distance5,Distance6,Distance7,Distance8;
uint16_t TIM3CH1_CAPTURE_STA,TIM3CH1_CAPTURE_VAL;
uint16_t TIM3CH2_CAPTURE_STA,TIM3CH2_CAPTURE_VAL;
uint16_t TIM3CH3_CAPTURE_STA,TIM3CH3_CAPTURE_VAL;
uint16_t TIM3CH4_CAPTURE_STA,TIM3CH4_CAPTURE_VAL;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//更新中断
{
	static uint8_t Tran_Time=0;
	if(htim->Instance == TIM6)
	{
	    HAL_GPIO_TogglePin(GPIOB, Trig0_Pin|Trig1_Pin|Trig2_Pin|Trig3_Pin 
                          |Trig4_Pin|Trig5_Pin|Trig6_Pin|Trig7_Pin);


		if(TIM2CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			Distance1=TIM2CH1_CAPTURE_STA&0X3F;
			Distance1*=65536;					        //溢出时间总和
			Distance1+=TIM2CH1_CAPTURE_VAL;		//得到总的高电平时间
			Distance1=Distance1*170/1000;
//			printf("%d \r\n",Distance);
			TIM2CH1_CAPTURE_STA=0;			//开启下一次捕获	
			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_1);
			HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
			
		}
		if(TIM3CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			Distance5=TIM3CH1_CAPTURE_STA&0X3F;
			Distance5*=65536;					        //溢出时间总和
			Distance5+=TIM3CH1_CAPTURE_VAL;		//得到总的高电平时间
			Distance5=Distance5*170/1000;
//			printf("%d \r\n",Distance);
			TIM3CH1_CAPTURE_STA=0;			//开启下一次捕获	
			HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1);
			HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);
		}

		if(TIM2CH2_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			Distance2=TIM2CH2_CAPTURE_STA&0X3F;
			Distance2*=65536;					        //溢出时间总和
			Distance2+=TIM2CH2_CAPTURE_VAL;		//得到总的高电平时间
			Distance2=Distance2*170/1000;
//			printf("%d \r\n",Distance);
			TIM2CH2_CAPTURE_STA=0;			//开启下一次捕获	
			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_2);
			HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_3);
			
		}
		if(TIM3CH2_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			Distance6=TIM3CH2_CAPTURE_STA&0X3F;
			Distance6*=65536;					        //溢出时间总和
			Distance6+=TIM3CH2_CAPTURE_VAL;		//得到总的高电平时间
			Distance6=Distance6*170/1000;
//			printf("%d \r\n",Distance);
			TIM3CH2_CAPTURE_STA=0;			//开启下一次捕获	
			HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_2);
			HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_3);
		}

		if(TIM2CH3_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			Distance3=TIM2CH3_CAPTURE_STA&0X3F;
			Distance3*=65536;					        //溢出时间总和
			Distance3+=TIM2CH3_CAPTURE_VAL;		//得到总的高电平时间
			Distance3=Distance3*170/1000;
//			printf("%d \r\n",Distance);
			TIM2CH3_CAPTURE_STA=0;			//开启下一次捕获	
			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_3);
			HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_4);
			
		}
		if(TIM3CH3_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			Distance7=TIM3CH3_CAPTURE_STA&0X3F;
			Distance7*=65536;					        //溢出时间总和
			Distance7+=TIM3CH3_CAPTURE_VAL;		//得到总的高电平时间
			Distance7=Distance7*170/1000;
//			printf("%d \r\n",Distance7);
			TIM3CH3_CAPTURE_STA=0;			//开启下一次捕获	
			HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_3);
			HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_4);
		}

		if(TIM2CH4_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			Distance4=TIM2CH4_CAPTURE_STA&0X3F;
			Distance4*=65536;					        //溢出时间总和
			Distance4+=TIM2CH4_CAPTURE_VAL;		//得到总的高电平时间
			Distance4=Distance4*170/1000;
//			printf("%d \r\n",Distance);
			TIM2CH4_CAPTURE_STA=0;			//开启下一次捕获	
			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_4);
			HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);

		}
		if(TIM3CH4_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			Distance8=TIM3CH4_CAPTURE_STA&0X3F;
			Distance8*=65536;					        //溢出时间总和
			Distance8+=TIM3CH4_CAPTURE_VAL;		//得到总的高电平时间
			Distance8=Distance8*170/1000;
//			printf("%d \r\n",Distance);
			TIM3CH4_CAPTURE_STA=0;			//开启下一次捕获	
			HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_4);
			HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);

		}		
		if(++Tran_Time==10)
		{
			Ultrasonic_Transmit(20,Distance1,Distance2,Distance3,Distance4,Distance5,Distance6,Distance7,Distance8);
			Tran_Time=0;
		}

	}

}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
	{		
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{

			if((TIM2CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM2CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
				{	  						
					TIM2CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
					TIM2CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//配置TIM3通道1上升沿捕获
				}else  								//还未开始,第一次捕获上升沿
				{
					TIM2CH1_CAPTURE_STA=0;			//清空
					TIM2CH1_CAPTURE_VAL=0;
					TIM2CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
					__HAL_TIM_DISABLE(&htim2);        //关闭定时器3
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//定时器3通道1设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim2);//使能定时器3

				}		    
			}
		}
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if((TIM2CH2_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM2CH2_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
				{	  			
					TIM2CH2_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
					TIM2CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_2);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//配置TIM3通道3上升沿捕获
				}else  								//还未开始,第一次捕获上升沿
				{
					TIM2CH2_CAPTURE_STA=0;			//清空
					TIM2CH2_CAPTURE_VAL=0;
					TIM2CH2_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
					__HAL_TIM_DISABLE(&htim2);        //关闭定时器3
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);//定时器3通道3设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim2);//使能定时器3

				}		    
			}
		}
	    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if((TIM2CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM2CH3_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
				{	  			
					TIM2CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
					TIM2CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_3);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//配置TIM3通道3上升沿捕获
				}else  								//还未开始,第一次捕获上升沿
				{
					TIM2CH3_CAPTURE_STA=0;			//清空
					TIM2CH3_CAPTURE_VAL=0;
					TIM2CH3_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
					__HAL_TIM_DISABLE(&htim2);        //关闭定时器3
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//定时器3通道3设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim2);//使能定时器3

				}		    
			}
		}
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		 {
			if((TIM2CH4_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM2CH4_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
				{	  			
					TIM2CH4_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
					TIM2CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_4);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//配置TIM3通道3上升沿捕获
				}else  								//还未开始,第一次捕获上升沿
				{
					TIM2CH4_CAPTURE_STA=0;			//清空
					TIM2CH4_CAPTURE_VAL=0;
					TIM2CH4_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
					__HAL_TIM_DISABLE(&htim2);        //关闭定时器3
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);//定时器3通道3设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim2);//使能定时器3

				}		    
			}
		}	

	}
	else if(htim->Instance == TIM3)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{

			if((TIM3CH1_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM3CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
				{	  						
					TIM3CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
					TIM3CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//配置TIM3通道1上升沿捕获
				}else  								//还未开始,第一次捕获上升沿
				{
					TIM3CH1_CAPTURE_STA=0;			//清空
					TIM3CH1_CAPTURE_VAL=0;
					TIM3CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
					__HAL_TIM_DISABLE(&htim3);        //关闭定时器3
					__HAL_TIM_SET_COUNTER(&htim3,0);
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//定时器3通道1设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim3);//使能定时器3

				}		    
			}
		}
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if((TIM3CH2_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM3CH2_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
				{	  			
					TIM3CH2_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
					TIM3CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_2);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//配置TIM3通道3上升沿捕获
				}else  								//还未开始,第一次捕获上升沿
				{
					TIM3CH2_CAPTURE_STA=0;			//清空
					TIM3CH2_CAPTURE_VAL=0;
					TIM3CH2_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
					__HAL_TIM_DISABLE(&htim3);        //关闭定时器3
					__HAL_TIM_SET_COUNTER(&htim3,0);
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_2);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);//定时器3通道3设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim3);//使能定时器3

				}		    
			}
		}
	    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if((TIM3CH3_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM3CH3_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
				{	  			
					TIM3CH3_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
					TIM3CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_3);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//配置TIM3通道3上升沿捕获
				}else  								//还未开始,第一次捕获上升沿
				{
					TIM3CH3_CAPTURE_STA=0;			//清空
					TIM3CH3_CAPTURE_VAL=0;
					TIM3CH3_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
					__HAL_TIM_DISABLE(&htim3);        //关闭定时器3
					__HAL_TIM_SET_COUNTER(&htim3,0);
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_3);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//定时器3通道3设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim3);//使能定时器3

				}		    
			}
		}
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		 {
			if((TIM3CH4_CAPTURE_STA&0X80)==0)//还未成功捕获
			{
				if(TIM3CH4_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
				{	  			
					TIM3CH4_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
					TIM3CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_4);//获取当前的捕获值.
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//配置TIM3通道3上升沿捕获
				}else  								//还未开始,第一次捕获上升沿
				{
					TIM3CH4_CAPTURE_STA=0;			//清空
					TIM3CH4_CAPTURE_VAL=0;
					TIM3CH4_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
					__HAL_TIM_DISABLE(&htim3);        //关闭定时器3
					__HAL_TIM_SET_COUNTER(&htim3,0);
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_4);   //一定要先清除原来的设置！！
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);//定时器3通道3设置为下降沿捕获
					__HAL_TIM_ENABLE(&htim3);//使能定时器3

				}		    
			}
		}	

	}
}


