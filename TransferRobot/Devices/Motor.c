/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Motor.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-01-08               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/

/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "Motor.h"
#include "Task_Can.h"
/*-------------------------- D E F I N E S -----------------------------------*/

Motor_t Motor;

const uint16_t M6020MiniAng=(M6020_RANGE/2); /*6020目标角度范围内的最小值*/
const	uint16_t M6020MaxAng=(MOTOR_MAX_ANGLE-M6020MiniAng);
/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

static void M3508_getMessage(CanRxMsg RxMessage);
static void M2006_getMessage(CanRxMsg RxMessage);
static void M6020_getMessage(CanRxMsg RxMessage);
static void M6623_getMessage(CanRxMsg RxMessage);

static void M3508_setCurrent(CANx_e CANx);
static void M2006_setCurrent(CANx_e CANx);
static void M6020_setCurrent(CANx_e CANx);
static void M6020_setTargetAngle(uint8_t M6020_ID, int16_t DR16_chx);
static void M6020_setLimitAngle(uint8_t M6020_ID,int16_t *TargetAngle);

/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 16:45
	* @brief   电机参数初始化
	* @param   void
	* @retval  void
	*/
void Motor_Init(void)
{
	Motor.M6020[0].medianAngle = 4000;
	Motor.M6020[1].medianAngle = 4000;
	Motor.M6020[2].medianAngle = 4000;
	Motor.M6020[3].medianAngle = 4000;

  Motor.M6020[0].OutPID.maxInput = MOTOR_MAX_ANGLE;
	Motor.M6020[1].OutPID.maxInput = MOTOR_MAX_ANGLE;
	Motor.M6020[2].OutPID.maxInput = MOTOR_MAX_ANGLE;
	Motor.M6020[3].OutPID.maxInput = MOTOR_MAX_ANGLE;
	
	Motor.p_M6020setCur = M6020_setCurrent;
	Motor.p_M3508setCur = M3508_setCurrent;
	Motor.p_M2006setCur = M2006_setCurrent;
	
	Motor.p_M6020setTarAngle = M6020_setTargetAngle;
	
	Motor.p_M6020getMsg = M6020_getMessage;
	Motor.p_M3508getMsg = M3508_getMessage;
	Motor.p_M2006getMsg = M2006_getMessage;
	Motor.p_M6623getMsg = M6623_getMessage;

	PID_StructInit(&Motor.M6020[0].OutPID,POSITION_PID,\
	                                      25000, 500, 10.0f,  0.0f, 2.8f);
	PID_StructInit(&Motor.M6020[1].OutPID,POSITION_PID,\
	                                      25000, 10000, 50.0f, 0.0f, 10.0f);       
	PID_StructInit(&Motor.M6020[2].OutPID,POSITION_PID,\
	                                      20000, 500, 10.0f,  0.0f, 2.8f);
	PID_StructInit(&Motor.M6020[3].OutPID,POSITION_PID,\
	                                      25000, 5000, 10.0f, 0.0f, 5.0f);                        
}

/*---------------------L O C A L - F U N C T I O N S--------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 16:45
	* @brief   解析3508报文
	* @param   [in] RxMessage: CAN报文缓存 
	* @retval  void
	*/
static void M3508_getMessage(CanRxMsg RxMessage)
{
	uint8_t stdId = 0;
	if(RxMessage.StdId<M3508_ID_START||RxMessage.StdId>M3508_ID_END)
		return;
	stdId = RxMessage.StdId-M3508_ID_START;
	Motor.M3508[stdId].realAngle = (uint16_t)(RxMessage.Data[0]<<8\
	                                        | RxMessage.Data[1]);
	Motor.M3508[stdId].realSpeed = (int16_t)(RxMessage.Data[2]<<8\
	                                       | RxMessage.Data[3]);
	Motor.M3508[stdId].realCurrent = (int16_t)(RxMessage.Data[4]<<8\
	                                         | RxMessage.Data[5]);
	Motor.M3508[stdId].temperture = RxMessage.Data[6];
	if(Motor.M3508[stdId].realAngle - Motor.M3508[stdId].lastRealAngle >6000)
	{
	  Motor.M3508[stdId].turnsNum--;
	}
	else if(Motor.M3508[stdId].realAngle - Motor.M3508[stdId].lastRealAngle <-6000)
	{
	  Motor.M3508[stdId].turnsNum++;
	}
	
	Motor.M3508[stdId].totalAngle=Motor.M3508[stdId].turnsNum*8192;
	Motor.M3508[stdId].lastRealAngle = Motor.M3508[stdId].realAngle;
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 16:45
	* @brief   解析6623报文
	* @param   [in] RxMessage: CAN报文缓存 
	* @retval  void
	*/
static void M6623_getMessage(CanRxMsg RxMessage)
{
	uint8_t stdId = 0;
	if(RxMessage.StdId<M6623_ID_START||RxMessage.StdId>M6623_ID_END)
		return;
	stdId = RxMessage.StdId-M6623_ID_START;
	Motor.M6623[stdId].realAngle = (uint16_t)(RxMessage.Data[0]<<8\
	                                        | RxMessage.Data[1]);
	Motor.M6623[stdId].realCurrent = (int16_t)(RxMessage.Data[2]<<8\
	                                         | RxMessage.Data[3]);
	
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 16:45
	* @brief   解析6020报文
	* @param   [in] RxMessage: CAN报文缓存 
	* @retval  void
	*/
static void M6020_getMessage(CanRxMsg RxMessage)
{
	uint8_t stdId = 0;
	if(RxMessage.StdId<M6623_ID_START||RxMessage.StdId>M6623_ID_END)
		return;
	stdId = RxMessage.StdId-M6623_ID_START;
	Motor.M6020[stdId].realAngle = (uint16_t)(RxMessage.Data[0]<<8\
	                                        | RxMessage.Data[1]);
	Motor.M6020[stdId].realSpeed = (int16_t)(RxMessage.Data[2]<<8\
	                                       | RxMessage.Data[3]);
	Motor.M6020[stdId].realCurrent = (int16_t)(RxMessage.Data[4]<<8\
	                                        | RxMessage.Data[5]);
	Motor.M6020[stdId].temperture = (uint16_t)RxMessage.Data[6];
	
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 16:45
	* @brief   解析2006报文
	* @param   [in] RxMessage: CAN报文缓存 
	* @retval  void
	*/
static void M2006_getMessage(CanRxMsg RxMessage)
{
	uint8_t stdId = 0;
	if(RxMessage.StdId<M3508_ID_START||RxMessage.StdId>M3508_ID_END)
		return;
	stdId = RxMessage.StdId-M3508_ID_START;
	Motor.M3508[stdId].realAngle = (uint16_t)(RxMessage.Data[0]<<8\
	                                        | RxMessage.Data[1]);
	Motor.M3508[stdId].realSpeed = (int16_t)(RxMessage.Data[2]<<8\
	                                       | RxMessage.Data[3]);
	Motor.M3508[stdId].realCurrent = (int16_t)(RxMessage.Data[4]<<8\
	                                         | RxMessage.Data[5]);
	Motor.M3508[stdId].temperture = RxMessage.Data[6];
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 16:45
	* @brief   设定6020电流
	* @param   [in] RxMessage: CAN报文缓存 
	* @retval  void
	*/
static void M6020_setCurrent(CANx_e CANx)
{
	static CanSend_t canSendData;
	
	canSendData.CANx = CANx;
	canSendData.SendCanTxMsg.DLC = 8;
	canSendData.SendCanTxMsg.IDE = CAN_ID_STD;
	canSendData.SendCanTxMsg.RTR = CAN_RTR_Data;
	canSendData.SendCanTxMsg.StdId = 0x1FF;

	canSendData.SendCanTxMsg.Data[0] = Motor.M6020[0].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[1] = Motor.M6020[0].targetCurrent ;
	canSendData.SendCanTxMsg.Data[2] = Motor.M6020[1].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[3] = Motor.M6020[1].targetCurrent ;
	canSendData.SendCanTxMsg.Data[4] = Motor.M6020[2].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[5] = Motor.M6020[2].targetCurrent ;
	canSendData.SendCanTxMsg.Data[6] = Motor.M6020[3].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[7] = Motor.M6020[3].targetCurrent ;
	
	xQueueSend(xCanSendQueue, &canSendData, 20);
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 16:45
	* @brief   设定3508电流
	* @param   [in] RxMessage: CAN报文缓存 
	* @retval  void
	*/
static void M3508_setCurrent(CANx_e CANx)
{
	static CanSend_t canSendData;
	
	canSendData.CANx = CANx;
	canSendData.SendCanTxMsg.DLC = 8;
	canSendData.SendCanTxMsg.IDE = CAN_ID_STD;
	canSendData.SendCanTxMsg.RTR = CAN_RTR_Data;
	canSendData.SendCanTxMsg.StdId = 0x200;
	canSendData.SendCanTxMsg.Data[0] = Motor.M3508[0].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[1] = Motor.M3508[0].targetCurrent ;
	canSendData.SendCanTxMsg.Data[2] = Motor.M3508[1].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[3] = Motor.M3508[1].targetCurrent ;
	canSendData.SendCanTxMsg.Data[4] = Motor.M3508[2].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[5] = Motor.M3508[2].targetCurrent ;
	canSendData.SendCanTxMsg.Data[6] = Motor.M3508[3].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[7] = Motor.M3508[3].targetCurrent ;
	
	xQueueSend(xCanSendQueue, &canSendData, 20);
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 16:45
	* @brief   设定2006电流
	* @param   [in] RxMessage: CAN报文缓存 
	* @retval  void
	*/
static void M2006_setCurrent(CANx_e CANx)
{
	static CanSend_t canSendData;
	
	canSendData.CANx = CANx;
	canSendData.SendCanTxMsg.DLC = 8;
	canSendData.SendCanTxMsg.IDE = CAN_ID_STD;
	canSendData.SendCanTxMsg.RTR = CAN_RTR_Data;
	canSendData.SendCanTxMsg.StdId = 0x200;
	
	canSendData.SendCanTxMsg.Data[0] = Motor.M2006[0].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[1] = Motor.M2006[0].targetCurrent ;
	canSendData.SendCanTxMsg.Data[2] = Motor.M2006[1].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[3] = Motor.M2006[1].targetCurrent ;
	canSendData.SendCanTxMsg.Data[4] = Motor.M2006[2].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[5] = Motor.M2006[2].targetCurrent ;
	canSendData.SendCanTxMsg.Data[6] = Motor.M2006[3].targetCurrent >> 8;
	canSendData.SendCanTxMsg.Data[7] = Motor.M2006[3].targetCurrent ;
	
	xQueueSend(xCanSendQueue, &canSendData, 20);
}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 16:45
	* @brief   设置6020目标角度
  * @param  [in] Ratio     遥控通道累加的倍率
  *         [in] M6020_ID  电机M6020_ID
  *         [in] DR16_chx  遥控通道 
	* @retval  void
	*/
static void M6020_setTargetAngle(uint8_t M6020_ID, int16_t DR16_chx)
{
  static int16_t targetAngle = 0;

	targetAngle = targetAngle > 8191 ? targetAngle - 8191\
                        	: targetAngle + RATIO * DR16_chx;
	targetAngle = targetAngle < 0 ? 8191 + targetAngle\
	                        : targetAngle + RATIO * DR16_chx;
	
  M6020_setLimitAngle(M6020_ID,&targetAngle);
	
	Motor.M6020[M6020_ID].targetAngle = targetAngle;

}

/*------------------------------80 Chars Limit--------------------------------*/
	/**
	* @Data    2019-01-08 16:45
	* @brief   设置6020角度幅值
  * @param  [in] M6020_ID  电机M6020_ID
  *         [in] *p_TargetAng  目标值
	* @retval  void
	*/
static void M6020_setLimitAngle(uint8_t M6020_ID, int16_t *p_TargetAng)
{

  if(M6020_MEDIAN(M6020_ID)>M6020MiniAng && M6020_MEDIAN(M6020_ID)<M6020MaxAng)
	{
    *p_TargetAng = *p_TargetAng > M6020_MEDIAN(M6020_ID) + M6020MiniAng\
		? M6020_MEDIAN(M6020_ID) + M6020MiniAng : *p_TargetAng;

		*p_TargetAng = *p_TargetAng < M6020_MEDIAN(M6020_ID) - M6020MiniAng 
		? M6020_MEDIAN(M6020_ID) - M6020MiniAng : *p_TargetAng;
	}
	else if(M6020_MEDIAN(M6020_ID) < M6020MiniAng)
	{
		*p_TargetAng = (*p_TargetAng > (M6020_MEDIAN(M6020_ID) + M6020MiniAng))\
		&&(*p_TargetAng < (M6020_MEDIAN(M6020_ID) + 4096)) ? M6020_MEDIAN(M6020_ID)\
		+ M6020MiniAng : *p_TargetAng;

		*p_TargetAng = (*p_TargetAng > (M6020_MEDIAN(M6020_ID) + 4096))\
		&& (*p_TargetAng < (M6020_MEDIAN(M6020_ID) + M6020MaxAng))\
		? M6020_MEDIAN(M6020_ID) + M6020MaxAng : *p_TargetAng;
	}
	else 
	{
		*p_TargetAng = (*p_TargetAng	> (M6020_MEDIAN(M6020_ID) - 4096))\
		&& (*p_TargetAng < (M6020_MEDIAN(M6020_ID) - M6020MiniAng))\
		? M6020_MEDIAN(M6020_ID) - M6020MiniAng : *p_TargetAng;

		*p_TargetAng = (*p_TargetAng > (M6020_MEDIAN(M6020_ID) - M6020MaxAng))\
		&& (*p_TargetAng < (M6020_MEDIAN(M6020_ID) - 4096))\
		? M6020_MEDIAN(M6020_ID) - M6020MaxAng : *p_TargetAng;	

	}
	
}
/*-----------------------------------FILE OF END------------------------------*/

