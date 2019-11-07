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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//�����ж�
{
	static uint8_t Tran_Time=0;
	if(htim->Instance == TIM6)
	{
	    HAL_GPIO_TogglePin(GPIOB, Trig0_Pin|Trig1_Pin|Trig2_Pin|Trig3_Pin 
                          |Trig4_Pin|Trig5_Pin|Trig6_Pin|Trig7_Pin);


		if(TIM2CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance1=TIM2CH1_CAPTURE_STA&0X3F;
			Distance1*=65536;					        //���ʱ���ܺ�
			Distance1+=TIM2CH1_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance1=Distance1*170/1000;
//			printf("%d \r\n",Distance);
			TIM2CH1_CAPTURE_STA=0;			//������һ�β���	
			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_1);
			HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);
			
		}
		if(TIM3CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance5=TIM3CH1_CAPTURE_STA&0X3F;
			Distance5*=65536;					        //���ʱ���ܺ�
			Distance5+=TIM3CH1_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance5=Distance5*170/1000;
//			printf("%d \r\n",Distance);
			TIM3CH1_CAPTURE_STA=0;			//������һ�β���	
			HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_1);
			HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);
		}

		if(TIM2CH2_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance2=TIM2CH2_CAPTURE_STA&0X3F;
			Distance2*=65536;					        //���ʱ���ܺ�
			Distance2+=TIM2CH2_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance2=Distance2*170/1000;
//			printf("%d \r\n",Distance);
			TIM2CH2_CAPTURE_STA=0;			//������һ�β���	
			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_2);
			HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_3);
			
		}
		if(TIM3CH2_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance6=TIM3CH2_CAPTURE_STA&0X3F;
			Distance6*=65536;					        //���ʱ���ܺ�
			Distance6+=TIM3CH2_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance6=Distance6*170/1000;
//			printf("%d \r\n",Distance);
			TIM3CH2_CAPTURE_STA=0;			//������һ�β���	
			HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_2);
			HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_3);
		}

		if(TIM2CH3_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance3=TIM2CH3_CAPTURE_STA&0X3F;
			Distance3*=65536;					        //���ʱ���ܺ�
			Distance3+=TIM2CH3_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance3=Distance3*170/1000;
//			printf("%d \r\n",Distance);
			TIM2CH3_CAPTURE_STA=0;			//������һ�β���	
			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_3);
			HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_4);
			
		}
		if(TIM3CH3_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance7=TIM3CH3_CAPTURE_STA&0X3F;
			Distance7*=65536;					        //���ʱ���ܺ�
			Distance7+=TIM3CH3_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance7=Distance7*170/1000;
//			printf("%d \r\n",Distance7);
			TIM3CH3_CAPTURE_STA=0;			//������һ�β���	
			HAL_TIM_IC_Stop_IT(&htim3,TIM_CHANNEL_3);
			HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_4);
		}

		if(TIM2CH4_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance4=TIM2CH4_CAPTURE_STA&0X3F;
			Distance4*=65536;					        //���ʱ���ܺ�
			Distance4+=TIM2CH4_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance4=Distance4*170/1000;
//			printf("%d \r\n",Distance);
			TIM2CH4_CAPTURE_STA=0;			//������һ�β���	
			HAL_TIM_IC_Stop_IT(&htim2,TIM_CHANNEL_4);
			HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);

		}
		if(TIM3CH4_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			Distance8=TIM3CH4_CAPTURE_STA&0X3F;
			Distance8*=65536;					        //���ʱ���ܺ�
			Distance8+=TIM3CH4_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			Distance8=Distance8*170/1000;
//			printf("%d \r\n",Distance);
			TIM3CH4_CAPTURE_STA=0;			//������һ�β���	
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

			if((TIM2CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM2CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
				{	  						
					TIM2CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM2CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_1);//��ȡ��ǰ�Ĳ���ֵ.
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//����TIM3ͨ��1�����ز���
				}else  								//��δ��ʼ,��һ�β���������
				{
					TIM2CH1_CAPTURE_STA=0;			//���
					TIM2CH1_CAPTURE_VAL=0;
					TIM2CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
					__HAL_TIM_DISABLE(&htim2);        //�رն�ʱ��3
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//��ʱ��3ͨ��1����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&htim2);//ʹ�ܶ�ʱ��3

				}		    
			}
		}
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if((TIM2CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM2CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
				{	  			
					TIM2CH2_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM2CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_2);//��ȡ��ǰ�Ĳ���ֵ.
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//����TIM3ͨ��3�����ز���
				}else  								//��δ��ʼ,��һ�β���������
				{
					TIM2CH2_CAPTURE_STA=0;			//���
					TIM2CH2_CAPTURE_VAL=0;
					TIM2CH2_CAPTURE_STA|=0X40;		//��ǲ�����������
					__HAL_TIM_DISABLE(&htim2);        //�رն�ʱ��3
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);//��ʱ��3ͨ��3����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&htim2);//ʹ�ܶ�ʱ��3

				}		    
			}
		}
	    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if((TIM2CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM2CH3_CAPTURE_STA&0X40)		//����һ���½��� 		
				{	  			
					TIM2CH3_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM2CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_3);//��ȡ��ǰ�Ĳ���ֵ.
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//����TIM3ͨ��3�����ز���
				}else  								//��δ��ʼ,��һ�β���������
				{
					TIM2CH3_CAPTURE_STA=0;			//���
					TIM2CH3_CAPTURE_VAL=0;
					TIM2CH3_CAPTURE_STA|=0X40;		//��ǲ�����������
					__HAL_TIM_DISABLE(&htim2);        //�رն�ʱ��3
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//��ʱ��3ͨ��3����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&htim2);//ʹ�ܶ�ʱ��3

				}		    
			}
		}
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		 {
			if((TIM2CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM2CH4_CAPTURE_STA&0X40)		//����һ���½��� 		
				{	  			
					TIM2CH4_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM2CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim2,TIM_CHANNEL_4);//��ȡ��ǰ�Ĳ���ֵ.
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_4);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//����TIM3ͨ��3�����ز���
				}else  								//��δ��ʼ,��һ�β���������
				{
					TIM2CH4_CAPTURE_STA=0;			//���
					TIM2CH4_CAPTURE_VAL=0;
					TIM2CH4_CAPTURE_STA|=0X40;		//��ǲ�����������
					__HAL_TIM_DISABLE(&htim2);        //�رն�ʱ��3
					__HAL_TIM_SET_COUNTER(&htim2,0);
					TIM_RESET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_4);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);//��ʱ��3ͨ��3����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&htim2);//ʹ�ܶ�ʱ��3

				}		    
			}
		}	

	}
	else if(htim->Instance == TIM3)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{

			if((TIM3CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM3CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
				{	  						
					TIM3CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM3CH1_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);//��ȡ��ǰ�Ĳ���ֵ.
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//����TIM3ͨ��1�����ز���
				}else  								//��δ��ʼ,��һ�β���������
				{
					TIM3CH1_CAPTURE_STA=0;			//���
					TIM3CH1_CAPTURE_VAL=0;
					TIM3CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
					__HAL_TIM_DISABLE(&htim3);        //�رն�ʱ��3
					__HAL_TIM_SET_COUNTER(&htim3,0);
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//��ʱ��3ͨ��1����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&htim3);//ʹ�ܶ�ʱ��3

				}		    
			}
		}
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			if((TIM3CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM3CH2_CAPTURE_STA&0X40)		//����һ���½��� 		
				{	  			
					TIM3CH2_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM3CH2_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_2);//��ȡ��ǰ�Ĳ���ֵ.
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_2);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_2,TIM_ICPOLARITY_RISING);//����TIM3ͨ��3�����ز���
				}else  								//��δ��ʼ,��һ�β���������
				{
					TIM3CH2_CAPTURE_STA=0;			//���
					TIM3CH2_CAPTURE_VAL=0;
					TIM3CH2_CAPTURE_STA|=0X40;		//��ǲ�����������
					__HAL_TIM_DISABLE(&htim3);        //�رն�ʱ��3
					__HAL_TIM_SET_COUNTER(&htim3,0);
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_2);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_2,TIM_ICPOLARITY_FALLING);//��ʱ��3ͨ��3����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&htim3);//ʹ�ܶ�ʱ��3

				}		    
			}
		}
	    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
			if((TIM3CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM3CH3_CAPTURE_STA&0X40)		//����һ���½��� 		
				{	  			
					TIM3CH3_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM3CH3_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_3);//��ȡ��ǰ�Ĳ���ֵ.
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_3,TIM_ICPOLARITY_RISING);//����TIM3ͨ��3�����ز���
				}else  								//��δ��ʼ,��һ�β���������
				{
					TIM3CH3_CAPTURE_STA=0;			//���
					TIM3CH3_CAPTURE_VAL=0;
					TIM3CH3_CAPTURE_STA|=0X40;		//��ǲ�����������
					__HAL_TIM_DISABLE(&htim3);        //�رն�ʱ��3
					__HAL_TIM_SET_COUNTER(&htim3,0);
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_3);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_3,TIM_ICPOLARITY_FALLING);//��ʱ��3ͨ��3����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&htim3);//ʹ�ܶ�ʱ��3

				}		    
			}
		}
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
		 {
			if((TIM3CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����
			{
				if(TIM3CH4_CAPTURE_STA&0X40)		//����һ���½��� 		
				{	  			
					TIM3CH4_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
					TIM3CH4_CAPTURE_VAL=HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_4);//��ȡ��ǰ�Ĳ���ֵ.
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_4);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_4,TIM_ICPOLARITY_RISING);//����TIM3ͨ��3�����ز���
				}else  								//��δ��ʼ,��һ�β���������
				{
					TIM3CH4_CAPTURE_STA=0;			//���
					TIM3CH4_CAPTURE_VAL=0;
					TIM3CH4_CAPTURE_STA|=0X40;		//��ǲ�����������
					__HAL_TIM_DISABLE(&htim3);        //�رն�ʱ��3
					__HAL_TIM_SET_COUNTER(&htim3,0);
					TIM_RESET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_4);   //һ��Ҫ�����ԭ�������ã���
					TIM_SET_CAPTUREPOLARITY(&htim3,TIM_CHANNEL_4,TIM_ICPOLARITY_FALLING);//��ʱ��3ͨ��3����Ϊ�½��ز���
					__HAL_TIM_ENABLE(&htim3);//ʹ�ܶ�ʱ��3

				}		    
			}
		}	

	}
}


