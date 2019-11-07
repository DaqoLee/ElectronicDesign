#include "User_can.h"
#include "can.h"
#include "control.h"
#include "Ctrl_Rammer.h"
FormatTrans   Type;//类型转换
FrameRate_Struct Cur_Meter;
uint8_t Meter_Power_Limit;
uint8_t can_tx_success_flag=0;
 /*
  * @brief CAN初始化
  * @param * hcan CAN结构体指针
  * @retval None
  */
void CAN_Init(CAN_HandleTypeDef* _hcan)
{
	//can1 &can2 use same filter config
	CAN_FilterConfTypeDef		CAN_FilterConfigStructure;
	static CanTxMsgTypeDef		Tx1Message;
	static CanRxMsgTypeDef 		Rx1Message;
	static CanTxMsgTypeDef		Tx2Message;
	static CanRxMsgTypeDef 		Rx2Message;

	CAN_FilterConfigStructure.FilterNumber = 0;
	CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterConfigStructure.FilterIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterConfigStructure.BankNumber = 14;//
	CAN_FilterConfigStructure.FilterActivation = ENABLE;

	if(HAL_CAN_ConfigFilter(_hcan, &CAN_FilterConfigStructure) != HAL_OK)
	{
		//err_deadloop(); //show error!
	}

	//filter config for can2 
	//can1(0-13)和can2(14-27)分别得到一半的filter
	CAN_FilterConfigStructure.FilterNumber = 14;
	if(HAL_CAN_ConfigFilter(_hcan, &CAN_FilterConfigStructure) != HAL_OK)
	{
     	//err_deadloop();
	}

	if(_hcan == &hcan1)
	{
		_hcan->pTxMsg = &Tx1Message;
		_hcan->pRxMsg = &Rx1Message;
	}

	if(_hcan == &hcan2)
	{
		_hcan->pTxMsg = &Tx2Message;
		_hcan->pRxMsg = &Rx2Message;
	}
	 __HAL_CAN_ENABLE_IT(&hcan1,CAN_IT_FMP0);
	 __HAL_CAN_ENABLE_IT(&hcan2,CAN_IT_FMP0);
}

 /*
  * @brief CAN报文解析
  * @param * hcan CAN结构体指针
  * @retval None
  */
void Analysis_RM_Can(CAN_HandleTypeDef* hcan)//报文解析
{
	static uint8_t i,flag;
	switch(hcan->pRxMsg->StdId)
	{
		case 0x201:
			ChassisParam.LB.Real_Angle=((int16_t)(hcan->pRxMsg->Data[0]<<8)|hcan->pRxMsg->Data[1]);
		    ChassisParam.LB.Real_Speed=((int16_t)(hcan->pRxMsg->Data[2]<<8)|hcan->pRxMsg->Data[3]);
			ChassisParam.LB.Real_Current=((int16_t)(hcan->pRxMsg->Data[4]<<8)|hcan->pRxMsg->Data[5]);
		    ChassisParam.LB.FrameRate++;
			break;
		case 0x202:
			ChassisParam.RB.Real_Angle=((int16_t)(hcan->pRxMsg->Data[0]<<8)|hcan->pRxMsg->Data[1]);
		    ChassisParam.RB.Real_Speed=((int16_t)(hcan->pRxMsg->Data[2]<<8)|hcan->pRxMsg->Data[3]);
			ChassisParam.RB.Real_Current=((int16_t)(hcan->pRxMsg->Data[4]<<8)|hcan->pRxMsg->Data[5]);
			ChassisParam.RB.FrameRate++;
			break;
		case 0x203:
			ChassisParam.RF.Real_Angle=((int16_t)(hcan->pRxMsg->Data[0]<<8)|hcan->pRxMsg->Data[1]);
		    ChassisParam.RF.Real_Speed=((int16_t)(hcan->pRxMsg->Data[2]<<8)|hcan->pRxMsg->Data[3]);
			ChassisParam.RF.Real_Current=((int16_t)(hcan->pRxMsg->Data[4]<<8)|hcan->pRxMsg->Data[5]);
		    ChassisParam.RF.FrameRate++;
			break;
		case 0x204:
			ChassisParam.LF.Real_Angle=((int16_t)(hcan->pRxMsg->Data[0]<<8)|hcan->pRxMsg->Data[1]);
		    ChassisParam.LF.Real_Speed=((int16_t)(hcan->pRxMsg->Data[2]<<8)|hcan->pRxMsg->Data[3]);
			ChassisParam.LF.Real_Current=((int16_t)(hcan->pRxMsg->Data[4]<<8)|hcan->pRxMsg->Data[5]);
			ChassisParam.LF.FrameRate++;
			break;
		case 0x205:
			CloudParam.Yaw.Real_Angle=((int16_t)(hcan->pRxMsg->Data[0]<<8)|hcan->pRxMsg->Data[1]);
			CloudParam.Yaw.Real_Current=((int16_t)(hcan->pRxMsg->Data[2]<<8)|hcan->pRxMsg->Data[3]);
			CloudParam.Yaw.FrameRate++;
			break;
		case 0x206:
			CloudParam.Pitch.Real_Angle=((int16_t)(hcan->pRxMsg->Data[0]<<8)|hcan->pRxMsg->Data[1]);
			CloudParam.Pitch.Real_Current=((int16_t)(hcan->pRxMsg->Data[2]<<8)|hcan->pRxMsg->Data[3]);
			CloudParam.Pitch.FrameRate++;
			break;
		case 0x207:
			M2006.Real_Angle=((int16_t)(hcan->pRxMsg->Data[0]<<8)|hcan->pRxMsg->Data[1]);
			M2006.Real_Speed=((int16_t)(hcan->pRxMsg->Data[2]<<8)|hcan->pRxMsg->Data[3]);
		    M2006.Real_Current=((int16_t)(hcan->pRxMsg->Data[4]<<8)|hcan->pRxMsg->Data[5]);
		    Rammer_Angle();
			M2006.FrameRate++;
		
		    break;
		case 0x208:
			break;
		
		case 0x259:	//UWB定位标签
			
       if(hcan->pRxMsg->DLC==8)
			{
			  flag++;
				if(flag==1)
				{
					UWB.X=((int16_t)(hcan->pRxMsg->Data[1]<<8)|hcan->pRxMsg->Data[0]);
					UWB.Y=((int16_t)(hcan->pRxMsg->Data[3]<<8)|hcan->pRxMsg->Data[2]);
					UWB.Angle=((uint16_t)(hcan->pRxMsg->Data[5]<<8)|hcan->pRxMsg->Data[4])/100;
					UWB.Distance[0]=((int16_t)(hcan->pRxMsg->Data[7]<<8)|hcan->pRxMsg->Data[6]);
					
				}	
				else if(flag==2)
				{
					
					UWB.Distance[1]=((int16_t)(hcan->pRxMsg->Data[1]<<8)|hcan->pRxMsg->Data[0]);
					UWB.Distance[2]=((int16_t)(hcan->pRxMsg->Data[3]<<8)|hcan->pRxMsg->Data[2]);
					UWB.Distance[3]=((int16_t)(hcan->pRxMsg->Data[5]<<8)|hcan->pRxMsg->Data[4]);
					UWB.Distance[4]=((int16_t)(hcan->pRxMsg->Data[7]<<8)|hcan->pRxMsg->Data[6]);
				}
				
			}
			
			if(hcan->pRxMsg->DLC==6)
			{    flag=0;
				 UWB.Distance[5]=((int16_t)(hcan->pRxMsg->Data[1]<<8)|hcan->pRxMsg->Data[0]);
				 UWB.Error=((int16_t)(hcan->pRxMsg->Data[3]<<8)|hcan->pRxMsg->Data[2]);
					
			}
		
			break;
		case 0x11://云台陀螺仪
			if(hcan->pRxMsg->DLC==4)
			{
				for(i=0;i<4;i++)
				{
				   Type.U[i]=(int16_t)(hcan->pRxMsg->Data[i]);
				}
				   CloudParam.Gyro.radian=Type.F;
				   CloudParam.Gyro.angle=57.3f*CloudParam.Gyro.radian;
			}
		    CloudParam.Gyro.FrameRate++;
			break;
		case 0x401://电流计
			
		//	memcpy(Current_Meter.data.dataBuff,hcan->pRxMsg->Data,sizeof(uint8_t[8]));
		    Current_Meter.data.ChassisVolt=((uint16_t)(hcan->pRxMsg->Data[1]<<8)|hcan->pRxMsg->Data[0])/100.0f;
		    Current_Meter.data.ChassisCurrent=((uint16_t)(hcan->pRxMsg->Data[3]<<8)|hcan->pRxMsg->Data[2])/100.0f;
		    Current_Meter.data.ChassisPower=((uint16_t)(hcan->pRxMsg->Data[5]<<8)|hcan->pRxMsg->Data[4])/100.0f;
            Current_Meter.data.ChassisPowerBuffer=((uint16_t)(hcan->pRxMsg->Data[7]<<8)|hcan->pRxMsg->Data[6])/100.0f;
		
			if(Current_Meter.data.ChassisPowerBuffer<30)
				Meter_Power_Limit=1;
			else if(Current_Meter.data.ChassisPowerBuffer<15)
				Meter_Power_Limit=2;
			else if(Current_Meter.data.ChassisPowerBuffer>50)
				Meter_Power_Limit=0;
		    Cur_Meter.FrameRate++;
			break;
	
				
		default:
			break;
	
	}

}

 /*
  * @brief CAN中断回调函数
  * @param * hcan CAN结构体指针
  * @retval None
  */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* _hcan)
{
	
	if(_hcan==&hcan1)
	{
	    Analysis_RM_Can(&hcan1);//CAN1解析
	    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_FMP0);
	}
	if(_hcan==&hcan2)
	{
	    Analysis_RM_Can(&hcan2);//CAN2解析
	    __HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_FMP0);
	}
	
}
void Set_moto_current(CAN_HandleTypeDef* hcan,uint16_t ID,int16_t Current1, int16_t Current2, int16_t Current3, int16_t Current4)
{

	hcan->pTxMsg->StdId = ID;
	hcan->pTxMsg->IDE = CAN_ID_STD;
	hcan->pTxMsg->RTR = CAN_RTR_DATA;
	hcan->pTxMsg->DLC = 0x08;
	hcan->pTxMsg->Data[0] = Current1 >> 8;
	hcan->pTxMsg->Data[1] = Current1;
	hcan->pTxMsg->Data[2] = Current2 >> 8;
	hcan->pTxMsg->Data[3] = Current2;
	hcan->pTxMsg->Data[4] = Current3 >> 8;
	hcan->pTxMsg->Data[5] = Current3 ;
	hcan->pTxMsg->Data[6] = Current4 >> 8;
	hcan->pTxMsg->Data[7] = Current4;
	
	HAL_CAN_Transmit(hcan, 1000);
}	

void Set_6623_current(CAN_HandleTypeDef* hcan,int16_t Current1, int16_t Current2)
{

	hcan->pTxMsg->StdId = 0x1FF;
	hcan->pTxMsg->IDE = CAN_ID_STD;
	hcan->pTxMsg->RTR = CAN_RTR_DATA;
	hcan->pTxMsg->DLC = 0x08;
	hcan->pTxMsg->Data[0] = Current1 >> 8;
	hcan->pTxMsg->Data[1] = Current1;
	hcan->pTxMsg->Data[2] = Current2 >> 8;
	hcan->pTxMsg->Data[3] = Current2;

	HAL_CAN_Transmit(hcan, 1000);
}	
void Set_2006_current(CAN_HandleTypeDef* hcan,int16_t Current1)
{

	hcan->pTxMsg->StdId = 0x1FF;
	hcan->pTxMsg->IDE = CAN_ID_STD;
	hcan->pTxMsg->RTR = CAN_RTR_DATA;
	hcan->pTxMsg->DLC = 0x08;
	hcan->pTxMsg->Data[4] = Current1 >> 8;
	hcan->pTxMsg->Data[5] = Current1;

	HAL_CAN_Transmit(hcan, 1000);
}	


/****************************************************************************************
                                       复位指令
Group   取值范围 0-7
Number  取值范围 0-15，其中Number==0时，为广播发送
*****************************************************************************************/
void CAN_RoboModule_DRV_Reset(unsigned char Group,unsigned char Number)
{
    unsigned short can_id = 0x000;
    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    hcan1.pTxMsg->Data[0] = 0x55;
    hcan1.pTxMsg->Data[1] = 0x55;
    hcan1.pTxMsg->Data[2] = 0x55;
    hcan1.pTxMsg->Data[3] = 0x55;
    hcan1.pTxMsg->Data[4] = 0x55;
    hcan1.pTxMsg->Data[5] = 0x55;
    hcan1.pTxMsg->Data[6] = 0x55;
    hcan1.pTxMsg->Data[7] = 0x55;
    
    can_tx_success_flag = 0;
	
    HAL_CAN_Transmit(&hcan1,1000);
//    while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}

/****************************************************************************************
                                     模式选择指令
Group   取值范围 0-7
Number  取值范围 0-15，其中Number==0时，为广播发送

Mode    取值范围

OpenLoop_Mode                       0x01
Current_Mode                        0x02
Velocity_Mode                       0x03
Position_Mode                       0x04
Velocity_Position_Mode              0x05
Current_Velocity_Mode               0x06
Current_Position_Mode               0x07
Current_Velocity_Position_Mode      0x08
*****************************************************************************************/
void CAN_RoboModule_DRV_Mode_Choice(unsigned char Group,unsigned char Number,unsigned char Mode)
{
    unsigned short can_id = 0x001;
    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    hcan1.pTxMsg->Data[0] = Mode;
    hcan1.pTxMsg->Data[1] = 0x55;
    hcan1.pTxMsg->Data[2] = 0x55;
    hcan1.pTxMsg->Data[3] = 0x55;
    hcan1.pTxMsg->Data[4] = 0x55;
    hcan1.pTxMsg->Data[5] = 0x55;
    hcan1.pTxMsg->Data[6] = 0x55;
    hcan1.pTxMsg->Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    HAL_CAN_Transmit(&hcan1,1000);
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}

/****************************************************************************************
                                   开环模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_pwm的取值范围如下：
-5000 ~ +5000，满值5000，其中temp_pwm = ±5000时，最大输出电压为电源电压

*****************************************************************************************/
void CAN_RoboModule_DRV_OpenLoop_Mode(unsigned char Group,unsigned char Number,short Temp_PWM)
{
    unsigned short can_id = 0x002;
    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    hcan1.pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    hcan1.pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
    hcan1.pTxMsg->Data[2] = 0x55;
    hcan1.pTxMsg->Data[3] = 0x55;
    hcan1.pTxMsg->Data[4] = 0x55;
    hcan1.pTxMsg->Data[5] = 0x55;
    hcan1.pTxMsg->Data[6] = 0x55;
    hcan1.pTxMsg->Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    HAL_CAN_Transmit(&hcan1,1000);
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}

/****************************************************************************************
                                   电流模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_pwm的取值范围如下：
0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

temp_current的取值范围如下：
-32768 ~ +32767，单位mA

*****************************************************************************************/
void CAN_RoboModule_DRV_Current_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Current)
{
    unsigned short can_id = 0x003;
    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

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
    
    hcan1.pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    hcan1.pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
    hcan1.pTxMsg->Data[2] = (unsigned char)((Temp_Current>>8)&0xff);
    hcan1.pTxMsg->Data[3] = (unsigned char)(Temp_Current&0xff);
    hcan1.pTxMsg->Data[4] = 0x55;
    hcan1.pTxMsg->Data[5] = 0x55;
    hcan1.pTxMsg->Data[6] = 0x55;
    hcan1.pTxMsg->Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    HAL_CAN_Transmit(&hcan1,1000);
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}

/****************************************************************************************
                                   速度模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_pwm的取值范围如下：
0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

temp_velocity的取值范围如下：
-32768 ~ +32767，单位RPM

*****************************************************************************************/
void CAN_RoboModule_DRV_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity)
{
    unsigned short can_id = 0x004;
    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8

    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

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
    
    hcan1.pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    hcan1.pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
    hcan1.pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    hcan1.pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
    hcan1.pTxMsg->Data[4] = 0x55;
    hcan1.pTxMsg->Data[5] = 0x55;
    hcan1.pTxMsg->Data[6] = 0x55;
    hcan1.pTxMsg->Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    HAL_CAN_Transmit(&hcan1,1000);
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}

/****************************************************************************************
                                   位置模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_pwm的取值范围如下：
0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

temp_position的取值范围如下：
-2147483648~+2147483647，单位qc

*****************************************************************************************/
void CAN_RoboModule_DRV_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,long Temp_Position)
{
    unsigned short can_id = 0x005;

    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

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
    
    hcan1.pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    hcan1.pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
    hcan1.pTxMsg->Data[2] = 0x55;
    hcan1.pTxMsg->Data[3] = 0x55;
    hcan1.pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    hcan1.pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    hcan1.pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    hcan1.pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can_tx_success_flag = 0;

    HAL_CAN_Transmit(&hcan1,1000);
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}

/****************************************************************************************
                                  速度位置模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_pwm的取值范围如下：
0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

temp_velocity的取值范围如下：
0 ~ +32767，单位RPM

temp_position的取值范围如下：
-2147483648~+2147483647，单位qc
*****************************************************************************************/
void CAN_RoboModule_DRV_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity,long Temp_Position)
{
    unsigned short can_id = 0x006;
    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

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
    
    hcan1.pTxMsg->Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    hcan1.pTxMsg->Data[1] = (unsigned char)(Temp_PWM&0xff);
    hcan1.pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    hcan1.pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
    hcan1.pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    hcan1.pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    hcan1.pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    hcan1.pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can_tx_success_flag = 0;
    HAL_CAN_Transmit(&hcan1,1000);
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}


/****************************************************************************************
                                  电流速度模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_current的取值范围如下：
0 ~ +32767，单位mA

temp_velocity的取值范围如下：
-32768 ~ +32767，单位RPM

*****************************************************************************************/
void CAN_RoboModule_DRV_Current_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity)
{
    unsigned short can_id = 0x007;
    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    if(Temp_Current < 0)
    {
        Temp_Current = ABS(Temp_Current);
    }
    
    hcan1.pTxMsg->Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    hcan1.pTxMsg->Data[1] = (unsigned char)(Temp_Current&0xff);
    hcan1.pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    hcan1.pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
    hcan1.pTxMsg->Data[4] = 0x55;
    hcan1.pTxMsg->Data[5] = 0x55;
    hcan1.pTxMsg->Data[6] = 0x55;
    hcan1.pTxMsg->Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    HAL_CAN_Transmit(&hcan1,1000);
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}


/****************************************************************************************
                                  电流位置模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_current的取值范围如下：
0 ~ +32767，单位mA

temp_position的取值范围如下：
-2147483648~+2147483647，单位qc

*****************************************************************************************/
void CAN_RoboModule_DRV_Current_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,long Temp_Position)
{
    unsigned short can_id = 0x008;
    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID

    
    if(Temp_Current < 0)
    {
        Temp_Current = ABS(Temp_Current);
    }
    
    hcan1.pTxMsg->Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    hcan1.pTxMsg->Data[1] = (unsigned char)(Temp_Current&0xff);
    hcan1.pTxMsg->Data[2] = 0x55;
    hcan1.pTxMsg->Data[3] = 0x55;
    hcan1.pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    hcan1.pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
	hcan1.pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    hcan1.pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can_tx_success_flag = 0;
    HAL_CAN_Transmit(&hcan1,1000);
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}


/****************************************************************************************
                                  电流速度位置模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_current的取值范围如下：
0 ~ +32767，单位mA

temp_velocity的取值范围如下：
0 ~ +32767，单位RPM

temp_position的取值范围如下：
-2147483648~+2147483647，单位qc

*****************************************************************************************/
void CAN_RoboModule_DRV_Current_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity,long Temp_Position)
{
    unsigned short can_id = 0x009;
    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    if(Temp_Current < 0)
    {
        Temp_Current = ABS(Temp_Current);
    }
    
    if(Temp_Velocity < 0)
    {
        Temp_Velocity = ABS(Temp_Velocity);
    }
    
    hcan1.pTxMsg->Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    hcan1.pTxMsg->Data[1] = (unsigned char)(Temp_Current&0xff);
    hcan1.pTxMsg->Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    hcan1.pTxMsg->Data[3] = (unsigned char)(Temp_Velocity&0xff);
    hcan1.pTxMsg->Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    hcan1.pTxMsg->Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    hcan1.pTxMsg->Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    hcan1.pTxMsg->Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can_tx_success_flag = 0;
    HAL_CAN_Transmit(&hcan1,1000);
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}

/****************************************************************************************
                                      配置指令
Temp_Time的取值范围: 0 ~ 255，为0时候，为关闭电流速度位置反馈功能
Ctl1_Ctl2的取值范围：0 or 1 ，当不为0 or 1，则认为是0，为关闭左右限位检测功能
特别提示：Ctl1，Ctl2的功能仅存在于102 301，其余版本驱动器，Ctl1_Ctl2 = 0 即可
*****************************************************************************************/
void CAN_RoboModule_DRV_Config(unsigned char Group,unsigned char Number,unsigned char Temp_Time,unsigned char Ctl1_Ctl2)
{
    unsigned short can_id = 0x00A;
    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    if((Ctl1_Ctl2 != 0x00)&&(Ctl1_Ctl2 != 0x01))
    {
        Ctl1_Ctl2 = 0x00;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    hcan1.pTxMsg->Data[0] = Temp_Time;
    hcan1.pTxMsg->Data[1] = Ctl1_Ctl2;
    hcan1.pTxMsg->Data[2] = 0x55;
    hcan1.pTxMsg->Data[3] = 0x55;
    hcan1.pTxMsg->Data[4] = 0x55;
    hcan1.pTxMsg->Data[5] = 0x55;
    hcan1.pTxMsg->Data[6] = 0x55;
    hcan1.pTxMsg->Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    HAL_CAN_Transmit(&hcan1,1000);
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}

/****************************************************************************************
                                      在线检测
*****************************************************************************************/
void CAN_RoboModule_DRV_Online_Check(unsigned char Group,unsigned char Number)
{
    unsigned short can_id = 0x00F;
    hcan1.pTxMsg->IDE = CAN_ID_STD;    //标准帧
    hcan1.pTxMsg->RTR = CAN_RTR_DATA;  //数据帧
    hcan1.pTxMsg->DLC = 0x08;          //帧长度为8
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return;
    }
    
    hcan1.pTxMsg->StdId = can_id;      //帧ID为传入参数的CAN_ID
    
    hcan1.pTxMsg->Data[0] = 0x55;
    hcan1.pTxMsg->Data[1] = 0x55;
    hcan1.pTxMsg->Data[2] = 0x55;
    hcan1.pTxMsg->Data[3] = 0x55;
    hcan1.pTxMsg->Data[4] = 0x55;
    hcan1.pTxMsg->Data[5] = 0x55;
    hcan1.pTxMsg->Data[6] = 0x55;
    hcan1.pTxMsg->Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    HAL_CAN_Transmit(&hcan1,1000);
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}

