/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Filter.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-03-03               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Filter.h" 
  
/*-------------------------- D E F I N E S -----------------------------------*/

Filter_t Filter;

/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

static void Filter_Limit(float *Value,float Max, float Mini);
static float Filter_Lowpass(float Value, float *p_Value, float Rate);
static float Filter_Absolute(float Value);
/*------------------G L O B A L - F U N C T I O N S --------------------------*/
/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-03-03 13:05
  * @brief   滤波器初始化
  * @param   void
  * @retval  void
  */
void Filter_Init(void)
{
  Filter.p_Limit = Filter_Limit;
  Filter.p_ABS = Filter_Absolute;
  Filter.p_Lowpass = Filter_Lowpass;
}
/*---------------------L O C A L - F U N C T I O N S--------------------------*/
/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-03-03 13:05
  * @brief   限幅
  * @param   void
  * @retval  void
  */
static void Filter_Limit(float *Value,float Max, float Mini)
{
	if(*Value>=0)
	{
		*Value = *Value >= Max ? Max : *Value;
		*Value = *Value <= Mini ? Mini : *Value;
	}
	else
	{
	  *Value = *Value <= Max ? Max : *Value;
		*Value = *Value >= Mini ? Mini : *Value;
	}
}

/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-03-03 13:05
  * @brief   绝对值
  * @param   void
  * @retval  void
  */
static float Filter_Absolute(float Value)
{
  Value = Value > 0 ? Value : -Value;

  return Value;
}

/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-03-03 13:05
  * @brief   低通滤波
  * @param   void
  * @retval  void
  */
static float Filter_Lowpass(float Value, float *p_Value, float Rate) 
{ 
  *p_Value+=Rate*(Value-*p_Value);
  return *p_Value;
}
/*-----------------------------------FILE OF END------------------------------*/


