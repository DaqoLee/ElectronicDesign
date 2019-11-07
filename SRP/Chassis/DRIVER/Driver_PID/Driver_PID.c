#include "Driver_PID.h"
#include "control.h"
#include "Ctrl_Rammer.h"
#include "User.h"
void abs_limit(float *a, float ABS_MAX){
    if(*a > ABS_MAX)
        *a = ABS_MAX;
    if(*a < -ABS_MAX)
        *a = -ABS_MAX;
}


/*参数初始化--------------------------------------------------------------*/
static void pid_param_init(
    pid_t *pid, 
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,
    float 	kp, 
    float 	ki, 
    float 	kd)
{
    
    pid->IntegralLimit = intergral_limit;
    pid->MaxOutput = maxout;
    pid->pid_mode = mode;
    
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
    	
}

/*中途更改参数设定(调试)------------------------------------------------------------*/

static void pid_reset(pid_t	*pid, float kp, float ki, float kd)
{
    pid->p = kp;
    pid->i = ki;
    pid->d = kd;
}

/*
 *@bref. calculate delta PID and position PID
 *@param[in] set： target
 *@param[in] real	measure
 */

float pid_calc(pid_t* pid, float get, float set)
	{
		
    pid->get[NOW] = get;
    pid->set[NOW] = set;
    pid->err[NOW] = set - get;	//set - measure
    if (pid->max_err != 0 && ABS(pid->err[NOW]) >  pid->max_err  )
		return 0;
	if (pid->deadband != 0 && ABS(pid->err[NOW]) < pid->deadband)
		return 0;
    
    if(pid->pid_mode == POSITION_PID) //位置式p
    {
/************************************过零点时比较权重挑最近的路到目标值***********************************/
		if(pid==&CloudParam.Pitch.PID.Out||pid==&CloudParam.Yaw.PID.Out||pid==&ChassisParam.Chassis_Gyro.Chassis_PID)
		{
			if(pid->err[NOW]<0)
				pid->err[NOW]=ABS(pid->err[NOW])>ABS(8191-ABS(pid->err[NOW]))?8191-ABS(pid->err[NOW]):pid->err[NOW];
			else if(pid->err[NOW]>0)
				pid->err[NOW]=ABS(pid->err[NOW])>ABS(8191-ABS(pid->err[NOW]))?ABS(pid->err[NOW])-8191:pid->err[NOW];
	    }
		else if(pid==&M2006.PID.Out)
		{
			if(pid->err[NOW]<0)
				pid->err[NOW]=ABS(pid->err[NOW])>ABS(Rammer_Max_Angle-ABS(pid->err[NOW]))?Rammer_Max_Angle-ABS(pid->err[NOW]):pid->err[NOW];
			else if(pid->err[NOW]>0)
				pid->err[NOW]=ABS(pid->err[NOW])>ABS(Rammer_Max_Angle-ABS(pid->err[NOW]))?ABS(pid->err[NOW])-Rammer_Max_Angle:pid->err[NOW];
		}
		else if(pid==&CloudParam.Cloud_Gyro.Yaw_PID.Out||pid==&SRP.Om_PID)
		{
			if(pid->err[NOW]<0)
				pid->err[NOW]=ABS(pid->err[NOW])>ABS(360-ABS(pid->err[NOW]))?360-ABS(pid->err[NOW]):pid->err[NOW];
			else if(pid->err[NOW]>0)
				pid->err[NOW]=ABS(pid->err[NOW])>ABS(360-ABS(pid->err[NOW]))?ABS(pid->err[NOW])-360:pid->err[NOW];
		}
		
		else if(pid==&TextPID)
		{
			if(pid->err[NOW]<0)
				pid->err[NOW]=ABS(pid->err[NOW])>ABS(4000-ABS(pid->err[NOW]))?4000-ABS(pid->err[NOW]):pid->err[NOW];
			else if(pid->err[NOW]>0)
				pid->err[NOW]=ABS(pid->err[NOW])>ABS(4000-ABS(pid->err[NOW]))?ABS(pid->err[NOW])-4000:pid->err[NOW];
		}
/*********************************************************************************************************/		
        pid->pout = pid->p * pid->err[NOW];
        pid->iout += pid->i * pid->err[NOW];
        pid->dout = pid->d * (pid->err[NOW] - pid->err[LAST] );
		
        abs_limit(&(pid->iout), pid->IntegralLimit);

		pid->pos_out = pid->pout + pid->iout + pid->dout;
			
        abs_limit(&(pid->pos_out), pid->MaxOutput);
        pid->last_pos_out = pid->pos_out;	//update last time 
    }
    else if(pid->pid_mode == DELTA_PID)//增量式P
    {
        pid->pout = pid->p * (pid->err[NOW] - pid->err[LAST]);
        pid->iout = pid->i * pid->err[NOW];
        pid->dout = pid->d * (pid->err[NOW] - 2*pid->err[LAST] + pid->err[LLAST]);
        
        abs_limit(&(pid->iout), pid->IntegralLimit);
        pid->delta_u = pid->pout + pid->iout + pid->dout;
		
        pid->delta_out = pid->last_delta_out + pid->delta_u;
        abs_limit(&(pid->delta_out), pid->MaxOutput);
        pid->last_delta_out = pid->delta_out;	//update last time
    }
    
    pid->err[LLAST] = pid->err[LAST];
    pid->err[LAST] = pid->err[NOW];
	
    pid->get[LLAST] = pid->get[LAST];
    pid->get[LAST] = pid->get[NOW];
	
    pid->set[LLAST] = pid->set[LAST];
    pid->set[LAST] = pid->set[NOW];
    return pid->pid_mode==POSITION_PID ? pid->pos_out : pid->delta_out;
//	
}


/*pid总体初始化-----------------------------------------------------------------*/

void PID_struct_init(
    pid_t* pid,
    uint32_t mode,
    uint32_t maxout,
    uint32_t intergral_limit,
    
    float 	kp, 
    float 	ki, 
    float 	kd)
{
    /*init function pointer*/
    pid->f_param_init = pid_param_init;
    pid->f_pid_reset = pid_reset;
//	pid->f_cal_pid = pid_calc;	
//	pid->f_cal_sp_pid = pid_sp_calc;	//addition
		
    /*init pid param */
    pid->f_param_init(pid, mode, maxout, intergral_limit, kp, ki, kd);
	
}
