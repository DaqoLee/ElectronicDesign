#ifndef  PROTOCOL_H_
#define  PROTOCOL_H_

#include <stdint.h>

/**
  *@brief:传输帧数据包
  */
typedef struct
{
	#define MAX_FRAME_LENGTH (256+6)		//最大桢长度
	#define MIN_FRAME_LENGTH  5			//最小帧长度
	uint8_t Device_Address;				//设备地址
	uint8_t Function_Type;				//帧功能
	uint8_t Sequence;                   //帧序列
	uint8_t Data_Length;				    //有效数据长度
	uint8_t *Data;						//数据
	uint16_t Checksum;					//校验值
}TransportProtocol_Typedef;


/**
  *@brief:传输结果 
  */
typedef enum
{
	FRAME_FORMAT_ERR = 1,			//帧格式错误
	CHECK_FORMAR_ERR = 2,			//校验值格式错误
    CHECK_ERR = 3,					//校验值错位
	UPACKED_SUCCESS = 4             //解包成功
}TransportProtocol_Result;


/**
  *@brief:协议管理器
  */
typedef struct
{	
	TransportProtocol_Typedef * TransportProtocol;  	//传输帧
	uint32_t  RecieveByteCount;						//接收的字节数
	uint8_t* Buf; 									//传输桢缓存
	uint16_t FrameTotalLength;						//帧总长度
	TransportProtocol_Result (*Unpacked)(void);     //解包函数
	void (*Packed)(void);							//打包函数
	uint16_t (*Check)(uint8_t *,uint16_t len);      //校验函数
}TransportProtocol_Manager_Typedef;


/*初始化传输协议*/
void  TransportProtocol_Init(TransportProtocol_Typedef *TransportProtocol,\
	                      uint8_t *buf,uint16_t (*check)(uint8_t *,uint16_t len));

/*外部声明协议管理器*/
extern TransportProtocol_Manager_Typedef TransportProtocol_Manager;

#endif


