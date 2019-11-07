#include "Driver_Niming.h"
UART_HandleTypeDef* user_huart;
uint32_t user_Time=0;
uint8_t send_buf[32]= {0};
/*获取串口指针*/
void Init_Ninming(UART_HandleTypeDef* _huart_)
{
    user_huart=_huart_;
}
/**/
uint16_t user_UART_WaitOnFlagUntilTimeout(void)
{
    return	(user_huart->Instance->SR)&(user_TI_Succeed);
}

//串口1发送1个字符
//c:要发送的字符
void usart1_send_char(uint8_t c)
{
    /*
    	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕
    	USART_SendData(USART1,c);
    	*/
    user_Time = HAL_GetTick();
    while((user_UART_WaitOnFlagUntilTimeout() != user_TI_Succeed)&&((user_Time-HAL_GetTick())<user_Timeout));//循环发送直到发送成功，有弊端

    if((user_Time-HAL_GetTick())<user_Timeout)
    {
        user_huart->Instance->DR = (c & (uint16_t)0x01FFU);
        //printf("\n发送成功\n");
    }
    else
    {
        printf("\n超时发送失败\n");
    }


}
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(uint8_t fun, uint8_t *data, uint8_t len)
{

    uint8_t i;
//    HAL_StatusTypeDef flag;
    if(len > 28)return;	//最多28字节数据
#if NIMING_4_22
    send_buf[len + 4] = 0;	//校验数置零
    send_buf[0] = 0xaa;	//帧头
    send_buf[1] = 0xaa;
    send_buf[2] = fun;	//功能字
    send_buf[3] = len;	//数据长度
    for(i = 0; i < len; i++)send_buf[4+ i] = data[i];			//复制数据
    for(i = 0; i < len + 4; i++)send_buf[len + 4] += send_buf[i];	//计算校验和
    for(i = 0; i < len + 5; i++)usart1_send_char(send_buf[i]);	//发送数据到串口1
#else
    send_buf[len + 3] = 0;	//校验数置零
    send_buf[0] = 0x88;	//帧头
    send_buf[1] = fun;	//功能字
    send_buf[2] = len;	//数据长度
    for(i = 0; i < len; i++)send_buf[3+ i] = data[i];			//复制数据
    for(i = 0; i < len + 3; i++)send_buf[len + 3] += send_buf[i];	//计算校验和
    for(i = 0; i < len + 4; i++)usart1_send_char(send_buf[i]);	//发送数据到串口1
#endif
//		flag=HAL_UART_Transmit_DMA(huartX,send_buf,len+4);//发送不要启用DMA发送不然发送的数据都是误码
//		huartX->gState=HAL_UART_STATE_READY;

}
//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//temp 真实温度值应为temp/100得到整数部分，temp%100得到小数部分
//注意不能显示小数
void Send_PC_Data(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz)
{
    //u8 tbuf[12];
    uint8_t tbuf[12];



    tbuf[0] = (aacx >> 8) & 0XFF; /////////////第1波形  数据位1
    tbuf[1] = aacx & 0XFF;


    tbuf[2] = (aacy >> 8) & 0XFF; /////////////第2波形  数据位2
    tbuf[3] = aacy & 0XFF;
    tbuf[4] = (aacz >> 8) & 0XFF; /////////////第3波形  数据位3
    tbuf[5] = aacz & 0XFF;
    tbuf[6] = (gyrox >> 8) & 0XFF; ////////////第4波形  数据位4
    tbuf[7] = gyrox & 0XFF;
    tbuf[8] = (gyroy >> 8) & 0XFF; ////////////第5波形  数据位5
    tbuf[9] = gyroy & 0XFF;
    tbuf[10] = (gyroz >> 8) & 0XFF; ///////////第6波形  数据位6
    tbuf[11] = gyroz & 0XFF;

    //增加温度值
//    tbuf[12] = (temp >> 8) & 0xff; ////////////第7波形  数据位7
//    tbuf[13] = temp & 0xff;

    //usart1_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
    usart1_niming_report(0xa1, tbuf, 12); //增加一个值
}
//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short aacx, short aacy, short aacz, short gyrox, short gyroy, short gyroz, short roll, short pitch, short yaw)
{
    uint8_t tbuf[28];
    uint8_t i;
    for(i = 0; i < 28; i++)tbuf[i] = 0; //清0
    tbuf[0] = (aacx >> 8) & 0XFF;
    tbuf[1] = aacx & 0XFF;
    tbuf[2] = (aacy >> 8) & 0XFF;
    tbuf[3] = aacy & 0XFF;
    tbuf[4] = (aacz >> 8) & 0XFF;
    tbuf[5] = aacz & 0XFF;
    tbuf[6] = (gyrox >> 8) & 0XFF;
    tbuf[7] = gyrox & 0XFF;
    tbuf[8] = (gyroy >> 8) & 0XFF;
    tbuf[9] = gyroy & 0XFF;
    tbuf[10] = (gyroz >> 8) & 0XFF;
    tbuf[11] = gyroz & 0XFF;
    tbuf[18] = (roll >> 8) & 0XFF;
    tbuf[19] = roll & 0XFF;
    tbuf[20] = (pitch >> 8) & 0XFF;
    tbuf[21] = pitch & 0XFF;
    tbuf[22] = (yaw >> 8) & 0XFF;
    tbuf[23] = yaw & 0XFF;
    usart1_niming_report(0XAF, tbuf, 28); //飞控显示帧,0XAF
}


