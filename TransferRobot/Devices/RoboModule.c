#include "RoboModule.h"
#include "Task_Can.h"
#define ABS(x) ((x)>0? (x):(-(x)))
/**
  * @Data    2019-03-28 13:46
  * @brief   初始化
  * @param   void
  * @retval  void
  */
void RoboModule_Init(void)
{
	RoboModule_DRV_Reset(0,1);
	RoboModule_DRV_Reset(0,2);
	RoboModule_DRV_Reset(0,3);
						
	vTaskDelay(1000);
	RoboModule_DRV_Mode_Choice(0,1,Velocity_Mode);
	RoboModule_DRV_Mode_Choice(0,2,Velocity_Mode);
	RoboModule_DRV_Mode_Choice(0,3,Velocity_Mode);
	vTaskDelay(500);
}

/**
  * @Data    2019-03-28 13:46
  * @brief   复位指令
  * @param   Group   取值范围 0-7
  *          Number  取值范围 0-15，其中Number==0时，为广播发送
  * @retval  void
  */
void RoboModule_DRV_Reset(uint8_t Group,uint8_t Number)
{
	  static CanSend_t canSendData;
    uint8_t stdId = 0x000;
  
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.RTR = CAN_RTR_DATA;  
    canSendData.SendCanTxMsg.DLC = 0x08;          
    
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      

    canSendData.SendCanTxMsg.Data[0] = 0x55;
    canSendData.SendCanTxMsg.Data[1] = 0x55;
    canSendData.SendCanTxMsg.Data[2] = 0x55;
    canSendData.SendCanTxMsg.Data[3] = 0x55;
    canSendData.SendCanTxMsg.Data[4] = 0x55;
    canSendData.SendCanTxMsg.Data[5] = 0x55;
    canSendData.SendCanTxMsg.Data[6] = 0x55;
    canSendData.SendCanTxMsg.Data[7] = 0x55;
    
    xQueueSend(xCanSendQueue, &canSendData, 20);
}

/**
  * @Data    2019-03-28 13:46
  * @brief   模式选择指令
  * @param  Group   取值范围 0-7
            Number  取值范围 0-15，其中Number==0时，为广播发送
            Mode    取值范围
  * @retval  void
  */
void RoboModule_DRV_Mode_Choice(uint8_t Group,uint8_t Number,uint8_t Mode)
{
    uint8_t stdId = 0x001;
    static CanSend_t canSendData;
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.RTR = CAN_RTR_DATA;  
    canSendData.SendCanTxMsg.DLC = 0x08;          
    
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      
    
    canSendData.SendCanTxMsg.Data[0] = Mode;
    canSendData.SendCanTxMsg.Data[1] = 0x55;
    canSendData.SendCanTxMsg.Data[2] = 0x55;
    canSendData.SendCanTxMsg.Data[3] = 0x55;
    canSendData.SendCanTxMsg.Data[4] = 0x55;
    canSendData.SendCanTxMsg.Data[5] = 0x55;
    canSendData.SendCanTxMsg.Data[6] = 0x55;
    canSendData.SendCanTxMsg.Data[7] = 0x55;

    xQueueSend(xCanSendQueue, &canSendData, 20);
}

/**
  * @Data    2019-03-28 13:46
  * @brief   开环模式下的数据指令
  * @param   Group   取值范围 0-7

             Number  取值范围 0-15，其中Number==0时，为广播发送

             temp_pwm的取值范围如下：
             -5000 ~ +5000,满值5000，其中temp_pwm = ±5000时，最大输出电压为电源电压
  * @retval  void
  */
void RoboModule_DRV_OpenLoop_Mode(uint8_t Group,uint8_t Number,
	                                                  int16_t Temp_PWM)
{
    uint8_t stdId = 0x002;
    static CanSend_t canSendData;
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.RTR = CAN_RTR_DATA;  
    canSendData.SendCanTxMsg.DLC = 0x08;          
    
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    canSendData.SendCanTxMsg.Data[0] = (uint8_t)((Temp_PWM>>8)&0xff);
    canSendData.SendCanTxMsg.Data[1] = (uint8_t)(Temp_PWM&0xff);
    canSendData.SendCanTxMsg.Data[2] = 0x55;
    canSendData.SendCanTxMsg.Data[3] = 0x55;
    canSendData.SendCanTxMsg.Data[4] = 0x55;
    canSendData.SendCanTxMsg.Data[5] = 0x55;
    canSendData.SendCanTxMsg.Data[6] = 0x55;
    canSendData.SendCanTxMsg.Data[7] = 0x55;

    xQueueSend(xCanSendQueue, &canSendData, 20);
}

/**
  * @Data    2019-03-28 13:46
  * @brief   电流模式下的数据指令
  * @param   Group   取值范围 0-7
  * 
             Number  取值范围 0-15，其中Number==0时，为广播发送

             temp_pwm的取值范围如下：
             0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

            temp_current的取值范围如下：
            -32768 ~ +32767，单位mA
  * @retval  void
  */
void RoboModule_DRV_Current_Mode(uint8_t Group,uint8_t Number,
	                                   int16_t Temp_PWM,int16_t Temp_Current)
{
    uint8_t stdId = 0x003;
    static CanSend_t canSendData;
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.RTR = CAN_RTR_DATA;  
    canSendData.SendCanTxMsg.DLC = 0x08;          
    
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = ABS(Temp_PWM);
    }
    
    canSendData.SendCanTxMsg.Data[0] = (uint8_t)((Temp_PWM>>8)&0xff);
    canSendData.SendCanTxMsg.Data[1] = (uint8_t)(Temp_PWM&0xff);
    canSendData.SendCanTxMsg.Data[2] = (uint8_t)((Temp_Current>>8)&0xff);
    canSendData.SendCanTxMsg.Data[3] = (uint8_t)(Temp_Current&0xff);
    canSendData.SendCanTxMsg.Data[4] = 0x55;
    canSendData.SendCanTxMsg.Data[5] = 0x55;
    canSendData.SendCanTxMsg.Data[6] = 0x55;
    canSendData.SendCanTxMsg.Data[7] = 0x55;
    
    xQueueSend(xCanSendQueue, &canSendData, 20);
}

/**
  * @Data    2019-03-28 13:46
  * @brief   速度模式下的数据指令
  * @param   Group   取值范围 0-7

             Number  取值范围 0-15，其中Number==0时，为广播发送

             temp_pwm的取值范围如下：
             0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

             temp_velocity的取值范围如下：
             -32768 ~ +32767，单位RPM
  * @retval  void
  */
void RoboModule_DRV_Velocity_Mode(uint8_t Group,uint8_t Number,
	                                    int16_t Temp_PWM,int16_t Temp_Velocity)
{
    uint8_t stdId = 0x004;
    CanSend_t canSendData;
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.RTR = CAN_RTR_DATA;  
    canSendData.SendCanTxMsg.DLC = 0x08;          
    
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = ABS(Temp_PWM);
    }
    
    canSendData.SendCanTxMsg.Data[0] = (uint8_t)((Temp_PWM>>8)&0xff);
    canSendData.SendCanTxMsg.Data[1] = (uint8_t)(Temp_PWM&0xff);
    canSendData.SendCanTxMsg.Data[2] = (uint8_t)((Temp_Velocity>>8)&0xff);
    canSendData.SendCanTxMsg.Data[3] = (uint8_t)(Temp_Velocity&0xff);
    canSendData.SendCanTxMsg.Data[4] = 0x55;
    canSendData.SendCanTxMsg.Data[5] = 0x55;
    canSendData.SendCanTxMsg.Data[6] = 0x55;
    canSendData.SendCanTxMsg.Data[7] = 0x55;

    xQueueSend(xCanSendQueue, &canSendData, 20);

}

/**
  * @Data    2019-03-28 13:46
  * @brief   位置模式下的数据指令
  * @param   Group   取值范围 0-7

             Number  取值范围 0-15，其中Number==0时，为广播发送

             temp_pwm的取值范围如下：
             0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

             temp_position的取值范围如下：
             -2147483648~+2147483647，单位qc
  * @retval  void
  */

void RoboModule_DRV_Position_Mode(uint8_t Group,uint8_t Number,
	                                      int16_t Temp_PWM,uint32_t Temp_Position)
{
    uint8_t stdId = 0x005;
    static CanSend_t canSendData;
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.RTR = CAN_RTR_DATA;  
    canSendData.SendCanTxMsg.DLC = 0x08;          
    
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = ABS(Temp_PWM);
    }
    
    canSendData.SendCanTxMsg.Data[0] = (uint8_t)((Temp_PWM>>8)&0xff);
    canSendData.SendCanTxMsg.Data[1] = (uint8_t)(Temp_PWM&0xff);
    canSendData.SendCanTxMsg.Data[2] = 0x55;
    canSendData.SendCanTxMsg.Data[3] = 0x55;
    canSendData.SendCanTxMsg.Data[4] = (uint8_t)((Temp_Position>>24)&0xff);
    canSendData.SendCanTxMsg.Data[5] = (uint8_t)((Temp_Position>>16)&0xff);
    canSendData.SendCanTxMsg.Data[6] = (uint8_t)((Temp_Position>>8)&0xff);
    canSendData.SendCanTxMsg.Data[7] = (uint8_t)(Temp_Position&0xff);
    
    xQueueSend(xCanSendQueue, &canSendData, 20);
}

/**
  * @Data    2019-03-28 13:46
  * @brief   速度位置模式下的数据指令
  * @param   Group   取值范围 0-7

             Number  取值范围 0-15，其中Number==0时，为广播发送

             temp_pwm的取值范围如下：
             0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

             temp_velocity的取值范围如下：
             0 ~ +32767，单位RPM

             temp_position的取值范围如下：
             -2147483648~+2147483647，单位qc
  * @retval  void
  */

void RoboModule_DRV_Velocity_Position_Mode(uint8_t Group,uint8_t Number,
	                int16_t Temp_PWM,int16_t Temp_Velocity,uint32_t Temp_Position)
{
    uint8_t stdId = 0x006;
    static CanSend_t canSendData;
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.RTR = CAN_RTR_DATA;  
    canSendData.SendCanTxMsg.DLC = 0x08;          
    
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = ABS(Temp_PWM);
    }
    
    if(Temp_Velocity < 0)
    {
        Temp_Velocity = ABS(Temp_Velocity);
    }
    
    canSendData.SendCanTxMsg.Data[0] = (uint8_t)((Temp_PWM>>8)&0xff);
    canSendData.SendCanTxMsg.Data[1] = (uint8_t)(Temp_PWM&0xff);
    canSendData.SendCanTxMsg.Data[2] = (uint8_t)((Temp_Velocity>>8)&0xff);
    canSendData.SendCanTxMsg.Data[3] = (uint8_t)(Temp_Velocity&0xff);
    canSendData.SendCanTxMsg.Data[4] = (uint8_t)((Temp_Position>>24)&0xff);
    canSendData.SendCanTxMsg.Data[5] = (uint8_t)((Temp_Position>>16)&0xff);
    canSendData.SendCanTxMsg.Data[6] = (uint8_t)((Temp_Position>>8)&0xff);
    canSendData.SendCanTxMsg.Data[7] = (uint8_t)(Temp_Position&0xff);

    xQueueSend(xCanSendQueue, &canSendData, 20);
}

/**
  * @Data    2019-03-28 13:46
  * @brief   电流速度模式下的数据指令
  * @param   Group   取值范围 0-7

             Number  取值范围 0-15，其中Number==0时，为广播发送

             temp_current的取值范围如下：
             0 ~ +32767，单位mA

             temp_velocity的取值范围如下：
             -32768 ~ +32767，单位RPM
  * @retval  void
  */

void RoboModule_DRV_Current_Velocity_Mode(uint8_t Group,uint8_t Number,
	                                  int16_t Temp_Current,int16_t Temp_Velocity)
{
    uint8_t stdId = 0x007;
    static CanSend_t canSendData;
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.RTR = CAN_RTR_DATA;  
    canSendData.SendCanTxMsg.DLC = 0x08;          
    
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      
    
    if(Temp_Current < 0)
    {
        Temp_Current = ABS(Temp_Current);
    }
    
    canSendData.SendCanTxMsg.Data[0] = (uint8_t)((Temp_Current>>8)&0xff);
    canSendData.SendCanTxMsg.Data[1] = (uint8_t)(Temp_Current&0xff);
    canSendData.SendCanTxMsg.Data[2] = (uint8_t)((Temp_Velocity>>8)&0xff);
    canSendData.SendCanTxMsg.Data[3] = (uint8_t)(Temp_Velocity&0xff);
    canSendData.SendCanTxMsg.Data[4] = 0x55;
    canSendData.SendCanTxMsg.Data[5] = 0x55;
    canSendData.SendCanTxMsg.Data[6] = 0x55;
    canSendData.SendCanTxMsg.Data[7] = 0x55;
    
    xQueueSend(xCanSendQueue, &canSendData, 20);
}

/**
  * @Data    2019-03-28 13:46
  * @brief   电流位置模式下的数据指令
  * @param   Group   取值范围 0-7

             Number  取值范围 0-15，其中Number==0时，为广播发送

             temp_current的取值范围如下：
             0 ~ +32767，单位mA

             temp_position的取值范围如下：
             -2147483648~+2147483647，单位qc
  * @retval  void
  */

void RoboModule_DRV_Current_Position_Mode(uint8_t Group,uint8_t Number,
	                                   int16_t Temp_Current,int32_t Temp_Position)
{
    uint8_t stdId = 0x008;
    static CanSend_t canSendData;
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.RTR = CAN_RTR_DATA;  
    canSendData.SendCanTxMsg.DLC = 0x08;          
    
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      

    
    if(Temp_Current < 0)
    {
        Temp_Current = ABS(Temp_Current);
    }
    
    canSendData.SendCanTxMsg.Data[0] = (uint8_t)((Temp_Current>>8)&0xff);
    canSendData.SendCanTxMsg.Data[1] = (uint8_t)(Temp_Current&0xff);
    canSendData.SendCanTxMsg.Data[2] = 0x55;
    canSendData.SendCanTxMsg.Data[3] = 0x55;
    canSendData.SendCanTxMsg.Data[4] = (uint8_t)((Temp_Position>>24)&0xff);
    canSendData.SendCanTxMsg.Data[5] = (uint8_t)((Temp_Position>>16)&0xff);
    canSendData.SendCanTxMsg.Data[6] = (uint8_t)((Temp_Position>>8)&0xff);
    canSendData.SendCanTxMsg.Data[7] = (uint8_t)(Temp_Position&0xff);
    
    xQueueSend(xCanSendQueue, &canSendData, 20);
}

/**
  * @Data    2019-03-28 13:46
  * @brief   电流速度位置模式下的数据指令
  * @param   Group   取值范围 0-7

             Number  取值范围 0-15，其中Number==0时，为广播发送

             temp_current的取值范围如下：
             0 ~ +32767，单位mA

             temp_velocity的取值范围如下：
             0 ~ +32767，单位RPM

             temp_position的取值范围如下：
             -2147483648~+2147483647，单位qc
  * @retval  void
  */

void RoboModule_DRV_Current_Velocity_Position_Mode(uint8_t Group,uint8_t Number,
	             int16_t Temp_Current,int16_t Temp_Velocity,int32_t Temp_Position)
{
    uint8_t stdId = 0x009;
    static CanSend_t canSendData;
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.RTR = CAN_RTR_DATA;  
    canSendData.SendCanTxMsg.DLC = 0x08;          
    
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      
    
    if(Temp_Current < 0)
    {
        Temp_Current = ABS(Temp_Current);
    }
    
    if(Temp_Velocity < 0)
    {
        Temp_Velocity = ABS(Temp_Velocity);
    }
    
    canSendData.SendCanTxMsg.Data[0] = (uint8_t)((Temp_Current>>8)&0xff);
    canSendData.SendCanTxMsg.Data[1] = (uint8_t)(Temp_Current&0xff);
    canSendData.SendCanTxMsg.Data[2] = (uint8_t)((Temp_Velocity>>8)&0xff);
    canSendData.SendCanTxMsg.Data[3] = (uint8_t)(Temp_Velocity&0xff);
    canSendData.SendCanTxMsg.Data[4] = (uint8_t)((Temp_Position>>24)&0xff);
    canSendData.SendCanTxMsg.Data[5] = (uint8_t)((Temp_Position>>16)&0xff);
    canSendData.SendCanTxMsg.Data[6] = (uint8_t)((Temp_Position>>8)&0xff);
    canSendData.SendCanTxMsg.Data[7] = (uint8_t)(Temp_Position&0xff);
    
    xQueueSend(xCanSendQueue, &canSendData, 20);
}

/**
  * @Data    2019-03-28 13:46
  * @brief   配置指令
  * @param   Temp_Time的取值范围: 0 ~ 255，为0时候，为关闭电流速度位置反馈功能
             Ctl1_Ctl2的取值范围：0 or 1 ，当不为0 or 1，则认为是0，为关闭左右
             限位检测功能特别提示：Ctl1，Ctl2的功能仅存在于102 301，其余版本驱动器，
            Ctl1_Ctl2 = 0 即可
  * @retval  void
  */

void RoboModule_DRV_Config(uint8_t Group,uint8_t Number, 
	                         uint8_t Temp_Time,uint8_t Ctl1_Ctl2)
{
    uint8_t stdId = 0x00A;
    static CanSend_t canSendData;
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.DLC = 0x08;          
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    if((Ctl1_Ctl2 != 0x00)&&(Ctl1_Ctl2 != 0x01))
    {
        Ctl1_Ctl2 = 0x00;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      
    
    canSendData.SendCanTxMsg.Data[0] = Temp_Time;
    canSendData.SendCanTxMsg.Data[1] = Ctl1_Ctl2;
    canSendData.SendCanTxMsg.Data[2] = 0x55;
    canSendData.SendCanTxMsg.Data[3] = 0x55;
    canSendData.SendCanTxMsg.Data[4] = 0x55;
    canSendData.SendCanTxMsg.Data[5] = 0x55;
    canSendData.SendCanTxMsg.Data[6] = 0x55;
    canSendData.SendCanTxMsg.Data[7] = 0x55;
    
    xQueueSend(xCanSendQueue, &canSendData, 20);
 
}

/**
  * @Data    2019-03-28 13:46
  * @brief   在线检测
  * @param   void
  * @retval  void
  */
void RoboModule_DRV_Online_Check(uint8_t Group,uint8_t Number)
{
    uint8_t stdId = 0x00F;
    static CanSend_t canSendData;
    canSendData.CANx=1;
    canSendData.SendCanTxMsg.IDE = CAN_ID_STD;    
    canSendData.SendCanTxMsg.RTR = CAN_RTR_DATA;  
    canSendData.SendCanTxMsg.DLC = 0x08;          
    
    if((Group<=7)&&(Number<=15))
    {
        stdId |= Group<<8;
        stdId |= Number<<4;
    }
    else
    {
        return;
    }
    
    canSendData.SendCanTxMsg.StdId = stdId;      
    
    canSendData.SendCanTxMsg.Data[0] = 0x55;
    canSendData.SendCanTxMsg.Data[1] = 0x55;
    canSendData.SendCanTxMsg.Data[2] = 0x55;
    canSendData.SendCanTxMsg.Data[3] = 0x55;
    canSendData.SendCanTxMsg.Data[4] = 0x55;
    canSendData.SendCanTxMsg.Data[5] = 0x55;
    canSendData.SendCanTxMsg.Data[6] = 0x55;
    canSendData.SendCanTxMsg.Data[7] = 0x55;
    
    xQueueSend(xCanSendQueue, &canSendData, 20);
}
