/**
  ************************************* Copyright ****************************** 
  *
  * (C) Copyright 2018,Daqo Lee,China, GCU.
  *		All Rights Reserved
  *                              
  * By(ZHE WOLF TEAM OF GCU ROBOT)
  * https://github.com/GCUWildwolfteam
  *    
  * FileName   : BSP_ADC.c   
  * Version    : v1.0		
  * Author     : Daqo Lee			
  * Date       : 2018-10-06         
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

#include "User_ADC.h"
uint32_t ADC_Value[N][M];
uint32_t WDD35D4[M];
float Text;


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    ADC_Init
  * @brief  
  * @param   None
  * @retval
  * @author  Daqo Lee 
  * @Data    2018-10-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void ADC1_Init(void)
{
	  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_Value, N*M);
}

/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    Get_Adc
  * @brief  
  * @param   None
  * @retval
  * @author  Daqo Lee 
  * @Data    2018-10-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

uint16_t Get_Adc(void)   
{

    HAL_ADC_Start(&hadc1);                               //开启ADC
	
    HAL_ADC_PollForConversion(&hadc1,10);                //轮询转换
  
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	            //返回最近一次ADC1规则组的转换结果
}


/* -------------------------------- begin  -------------------------------- */
/**
  * @Name    Get_Adc_Average
  * @brief  
  * @param   None
  * @retval
  * @author  Daqo Lee 
  * @Data    2018-10-06
  * 1. ...
  * <modify staff>:
  * <data>        :
  * <description> :
  * 2. ...
 **/
/* -------------------------------- end -------------------------------- */

void Get_Adc_Average(void)
{
	uint32_t sum=0;
	uint8_t i,n;

	for(i=0;i<M;i++)
	{
		for ( n=0;n<N;n++) 
		{
			sum += ADC_Value[n][i];
		}
		WDD35D4[i]=sum/N;
		sum=0;
	}
	Text=WDD35D4[0];
//	Text=((float) WDD35D4[0]/4096*3.3)*1.67+0.225;
} 

