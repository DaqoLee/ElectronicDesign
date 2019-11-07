/**
  * @file    protocol.h
  * @brief   Protocol configuration file.
  */
#include "protocol.h"
#include "check.h"
#include "string.h"

/*定义一个协议管理器*/
TransportProtocol_Manager_Typedef  TransportProtocol_Manager;


/**
  *@brief:数据解包
  */
static TransportProtocol_Result TransportProtocol_Unpacked()
{	
	int check_len = 0;   	    //校验字节的长度   
	uint16_t checksum = 0;      //接收到的校验值
	uint16_t checksum_cal = 0;  //计算得到的校验值
	uint8_t *checksum_pos = 0;  //校验值的偏移位置

	/*帧格式错误，非法长度*/
	if( TransportProtocol_Manager.RecieveByteCount > MAX_FRAME_LENGTH 
	 || TransportProtocol_Manager.RecieveByteCount < MIN_FRAME_LENGTH)
	{
		return FRAME_FORMAT_ERR;
	}

	/*得到有效数据长度*/
	TransportProtocol_Manager.TransportProtocol->Data_Length = \
	                                             TransportProtocol_Manager.Buf[3];
	/*校验字节的长度 = 接收的帧长度-4个字节的头-有效数据的长度*/
	check_len = TransportProtocol_Manager.RecieveByteCount - 4 - \
	                     TransportProtocol_Manager.TransportProtocol->Data_Length;
	/*校验值格式错误 校验字节个数情况*/
	if(check_len<1 || check_len>2)
	{
		return CHECK_FORMAR_ERR;
	}

	/*得到校验值的偏移地址*/
	checksum_pos = (uint8_t *)(4+TransportProtocol_Manager.TransportProtocol->\
	                                 Data_Length + TransportProtocol_Manager.Buf);
	if(check_len==1)
	{
		checksum = (*checksum_pos)&0xff;
	}else if(check_len==2)
	{
		checksum  = (*checksum_pos)&0xff;
		checksum_pos++;
		checksum <<=8;
		checksum |= *checksum_pos;
	}
	/*计算 帧头头和数据的校验值*/
	checksum_cal = TransportProtocol_Manager.Check(TransportProtocol_Manager.Buf,\
                  	4+TransportProtocol_Manager.TransportProtocol->Data_Length);
	/*校验错误*/
	if(checksum!=checksum_cal)
	{
		return CHECK_ERR;
	}
	
	/*设备地址*/
	TransportProtocol_Manager.TransportProtocol->Device_Address = \
	                                             TransportProtocol_Manager.Buf[0];
	/*帧功能*/
	TransportProtocol_Manager.TransportProtocol->Function_Type = \
	                                             TransportProtocol_Manager.Buf[1];
	/*帧序列,当前第几帧*/
	TransportProtocol_Manager.TransportProtocol->Sequence = \
	                                             TransportProtocol_Manager.Buf[2];
	/*帧有效数据长度 (前面已获取)*/
	
	/*帧数据*/
	TransportProtocol_Manager.TransportProtocol->Data = &\
	                                             TransportProtocol_Manager.Buf[4];
	/*帧校验值*/
	TransportProtocol_Manager.TransportProtocol->Checksum = checksum_cal;
	
	return UPACKED_SUCCESS;  
}


/**
  *@brief:数据封包
  */
static void TransportProtocol_Packed()
{
	uint16_t checksum=0;
	uint8_t *check_pos;  //校验值的偏移位置
	
	/*设备地址*/
	TransportProtocol_Manager.Buf[0] = TransportProtocol_Manager.\
	                                            TransportProtocol->Device_Address;
	/*帧功能*/
	TransportProtocol_Manager.Buf[1] = TransportProtocol_Manager.\
	                                             TransportProtocol->Function_Type;
	/*帧序列*/
	TransportProtocol_Manager.Buf[2] = TransportProtocol_Manager.\
	                                                  TransportProtocol->Sequence;
	/*帧有效数据大小*/
	TransportProtocol_Manager.Buf[3] = TransportProtocol_Manager.\
	                                               TransportProtocol->Data_Length;
	/*帧数据*/
	memcpy(&TransportProtocol_Manager.Buf[4],TransportProtocol_Manager.\
                             TransportProtocol->Data,TransportProtocol_Manager.\
	                                              TransportProtocol->Data_Length);
	/*计算校验值*/
	checksum = TransportProtocol_Manager.Check(TransportProtocol_Manager.Buf,\
	                  4+TransportProtocol_Manager.TransportProtocol->Data_Length);
	/*校验值*/
	check_pos = TransportProtocol_Manager.Buf+4+\
	                     TransportProtocol_Manager.TransportProtocol->Data_Length;
	
	if(checksum<256)
	{ 
		(*check_pos) = checksum&0xff;
		/*记录帧总长度*/
		TransportProtocol_Manager.FrameTotalLength = 4+\
		                 TransportProtocol_Manager.TransportProtocol->Data_Length+1;
	}else  /*CRC16*/
	{	
		(*check_pos) = (uint8_t)(checksum>>8)&0xff;  //高字节
		check_pos++;
		(*check_pos) = (uint8_t)(checksum&0xff);	 //低字节
		/*I记录帧总长度*/
		TransportProtocol_Manager.FrameTotalLength = 4+\
		                 TransportProtocol_Manager.TransportProtocol->Data_Length+2;
	}	
}


/**
  *@brief:初始化传输协议
  *@param[in]:TransportPortocol,传输帧
  *@param[in]:*buf,缓冲区数据
  *@param[in]:uint16_t (*check)(uint8_t *,uint16_t len),校验方式
  *           Checksum_Sum:和校验
  *           Checksum_XOR：异或校验
  *           Checksum_CRC8:CRC8校验
  *           Checksum_CRC16:CRC16校验
  */
void  TransportProtocol_Init(TransportProtocol_Typedef *TransportProtocol,
                             uint8_t *buf,
							 uint16_t (*check)(uint8_t *,uint16_t len))
{	
	/*协议包*/
	TransportProtocol_Manager.TransportProtocol = TransportProtocol;
	/*传输协议缓存*/ 
	TransportProtocol_Manager.Buf = buf;
	/*选择校验方式*/ 
	TransportProtocol_Manager.Check = check;
	/*打包函数*/
	TransportProtocol_Manager.Packed = TransportProtocol_Packed;
	/*解包函数*/
	TransportProtocol_Manager.Unpacked = TransportProtocol_Unpacked;	
}



