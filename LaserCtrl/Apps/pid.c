/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : pid.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-14               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "pid.h"  
/*-------------------------- D E F I N E S -----------------------------------*/

int16_t FuzzyErrRule[7] = {60, -40, -20, 0, 20, 40, 60};
int16_t FuzzyErcRule[7] = {-15, -10, -5, 0, 5, 10, 15};

int16_t FuzzyKpRule[7][7] = {{PB,PB,PM,PM,PS,ZO,ZO},
                             {PB,PB,PM,PS,PS,ZO,NS},
                             {PM,PM,PM,PS,ZO,NS,NS},
                             {PM,PM,PS,ZO,NS,NM,NM},
                             {PS,PS,ZO,NS,NS,NM,NM},
                             {PS,ZO,NS,NM,NM,NM,NB},
                             {ZO,ZO,NM,NM,NM,NB,NB}};

/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

static void pidParamInit(pid_t *pid, uint32_t mode, uint32_t maxout,
           	uint32_t intergral_limit, float kp, float ki, float kd);
static void pidReset(pid_t	*pid, float kp, float ki, float kd);
//static void LinearQuantization(pid_t* pid, float get, float set ,float *p_Value);
//static void CalcMembership(pid_t* pid );

/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-01-13 19:13
  * @brief   
  * @param   void
  * @retval  void
  */

void absLimit(float *a, float ABS_MAX)
{
	if(*a > ABS_MAX)
			*a = ABS_MAX;
	if(*a < -ABS_MAX)
			*a = -ABS_MAX;
}


float PID_Calc(pid_t* pid, float get, float set)
{
		
    pid->get[NOW] = get;
    pid->set[NOW] = set;
    pid->err[NOW] = set - get;	//set - measure
  //	pid->erc[NOW] = pid->err[NOW] - pid->err[LAST];
    if (pid->maxErr != 0 && ABS(pid->err[NOW]) >  pid->maxErr  )
		  return 0;
  	if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband)
		  return 0;
    
    if(pid->pidMode == POSITION_PID) //位置式P
    {	
			//CalcMembership(pid);
			
      pid->Pout = pid->P * pid->err[NOW];
      pid->Iout += pid->I * pid->err[NOW];
      pid->Dout = pid->D * (pid->err[NOW] - pid->err[LAST] );
		 
      absLimit(&(pid->Iout), pid->integralLimit);

	    pid->posOut = pid->Pout + pid->Iout + pid->Dout;
			
      absLimit(&(pid->posOut), pid->maxOutput);
      pid->lastPosOut = pid->posOut;	//update last time 
    }
    else if(pid->pidMode == DELTA_PID)//增量式P
    {
			pid->Pout = pid->P * (pid->err[NOW] - pid->err[LAST]);
			pid->Iout = pid->I * pid->err[NOW];
			pid->Dout = pid->D * (pid->err[NOW] - 2*pid->err[LAST] + pid->err[LLAST]);
				
			absLimit(&(pid->Iout), pid->integralLimit);
			pid->deltaU = pid->Pout + pid->Iout + pid->Dout;
		
			pid->deltaOut = pid->lastDeltaOut + pid->deltaU;
			absLimit(&(pid->deltaOut), pid->maxOutput);
			pid->lastDeltaOut = pid->deltaOut;	//update last time
    }
    
    pid->err[LLAST] = pid->err[LAST];
    pid->err[LAST] = pid->err[NOW];
	
    pid->get[LLAST] = pid->get[LAST];
    pid->get[LAST] = pid->get[NOW];
	
    pid->set[LLAST] = pid->set[LAST];
    pid->set[LAST] = pid->set[NOW];
    return pid->pidMode==POSITION_PID ? pid->posOut : pid->deltaOut;
}

void PID_StructInit(
    pid_t* pid,
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,
    
    float 	kp, 
    float 	ki, 
    float 	kd)
{
	/*init function pointer*/
	pid->f_ParamInit = pidParamInit;
	pid->f_PID_Reset = pidReset;
	/*init pid param */
	pid->f_ParamInit(pid, mode, maxout, intergral_limit, kp, ki, kd);
	
}

/*---------------------L O C A L - F U N C T I O N S--------------------------*/


static void pidParamInit(
    pid_t *pid, 
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,
    float 	kp, 
    float 	ki, 
    float 	kd)
{
	
	pid->integralLimit = intergral_limit;
	pid->maxOutput = maxout;
	pid->pidMode = mode;
	
	pid->P = kp;
	pid->I = ki;
	pid->D = kd;
    	
}


static void pidReset(pid_t	*pid, float kp, float ki, float kd)
{
    pid->P = kp;
    pid->I = ki;
    pid->D = kd;
}
#if 0
/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-01-13 19:13
  * @brief   
  * @param   void
  * @retval  void
  */
//static void LinearQuantization(pid_t* pid, float get, float set ,float *p_Value)
//{
//  pid->err[NOW] = set - get;	//set - measure
//  pid->erc[NOW] = pid->err[NOW] - pid->err[LAST];


//  p_Value[0]=6.0f*pid->err[NOW]/pid->errRange;
//  p_Value[1]=3.0f*pid->erc[NOW]/pid->errRange;

//  pid->err[LAST] = pid-> err[NOW];
//  pid->erc[LAST] = pid-> err[NOW];
//}
/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-01-13 19:13
  * @brief   
  * @param   void
  * @retval  void
  */
static void CalcMembership(pid_t* pid )
{
	float errMembership[2] = {0,0};
	float ercMembership[2] = {0,0};
  
  uint8_t errIndex;
  uint8_t ercIndex;


  if(pid->err[NOW]<FuzzyErrRule[0])
  {
    errMembership[0] = 1.0f;
    errIndex = 0;
  }
  else if(pid->err[NOW] >= FuzzyErrRule[0] && pid->err[NOW] < FuzzyErrRule[1])
  {
    errMembership[0] = (FuzzyErrRule[1] - pid->err[NOW])/20;
    errIndex = 0;
  }
  else if(pid->err[NOW] >= FuzzyErrRule[1] && pid->err[NOW] < FuzzyErrRule[2])
  {
    errMembership[0] = (FuzzyErrRule[2] - pid->err[NOW])/20;
    errIndex = 1;
  }
  else if(pid->err[NOW] >= FuzzyErrRule[0] && pid->err[NOW] < FuzzyErrRule[3])
  {
    errMembership[0] = (FuzzyErrRule[3] - pid->err[NOW])/20;
    errIndex = 2;
  }
  else if(pid->err[NOW] >= FuzzyErrRule[0] && pid->err[NOW] < FuzzyErrRule[4])
  {
    errMembership[0] = (FuzzyErrRule[4] - pid->err[NOW])/20;
    errIndex = 3;
  }
  else if(pid->err[NOW] >= FuzzyErrRule[0] && pid->err[NOW] < FuzzyErrRule[5])
  {
    errMembership[0] = (FuzzyErrRule[5] - pid->err[NOW])/20;
    errIndex = 4;
  }
  else if(pid->err[NOW] >= FuzzyErrRule[0] && pid->err[NOW] < FuzzyErrRule[6])
  {
    errMembership[0] = (FuzzyErrRule[6] - pid->err[NOW])/20;
    errIndex = 5;
  }
  else
  {
    errMembership[0] = 0;
    errIndex = 5;
  }
  
  errMembership[1] = 1.0f - errMembership[0];

  if(pid->erc[NOW]<FuzzyErcRule[0])
  {
    ercMembership[0] = 1.0f;
    ercIndex = 0;
  }
  else if(pid->erc[NOW] >= FuzzyErcRule[0] && pid->erc[NOW] < FuzzyErcRule[1])
  {
    ercMembership[0] = (FuzzyErcRule[1] - pid->erc[NOW])/5;
    ercIndex = 0;
  }
  else if(pid->erc[NOW] >= FuzzyErcRule[1] && pid->erc[NOW] < FuzzyErcRule[2])
  {
    ercMembership[0] = (FuzzyErcRule[2] - pid->erc[NOW])/5;
    ercIndex = 1;
  }
  else if(pid->erc[NOW] >= FuzzyErcRule[0] && pid->erc[NOW] < FuzzyErcRule[3])
  {
    ercMembership[0] = (FuzzyErcRule[3] - pid->erc[NOW])/5;
    ercIndex = 2;
  }
  else if(pid->erc[NOW] >= FuzzyErcRule[0] && pid->erc[NOW] < FuzzyErcRule[4])
  {
    ercMembership[0] = (FuzzyErcRule[4] - pid->erc[NOW])/5;
    ercIndex = 3;
  }
  else if(pid->erc[NOW] >= FuzzyErcRule[0] && pid->erc[NOW] < FuzzyErcRule[5])
  {
    ercMembership[0] = (FuzzyErcRule[5] - pid->erc[NOW])/5;
    ercIndex = 4;
  }
  else if(pid->erc[NOW] >= FuzzyErcRule[0] && pid->erc[NOW] < FuzzyErcRule[6])
  {
    ercMembership[0] = (FuzzyErcRule[6] - pid->erc[NOW])/5;
    ercIndex = 5;
  }
  else
  {
    ercMembership[0] = 0;
    ercIndex = 5;
  }

  ercMembership[1] = 1.0f - ercMembership[0];
  
  pid->P=20+ABS((errMembership[0] * ercMembership[0] * FuzzyKpRule[errIndex][ercIndex]\
  + errMembership[0] * ercMembership[1] * FuzzyKpRule[errIndex][ercIndex + 1]\
  + errMembership[1] * ercMembership[0] * FuzzyKpRule[errIndex + 1][ercIndex]\
  + errMembership[1] * ercMembership[1] * FuzzyKpRule[errIndex + 1][ercIndex + 1]));
  
}
#endif
/*-----------------------------------FILE OF END------------------------------*/

