#include "IM1253B.h"


IM1253B_t IM1253B;
uint8_t Rx_Buffer[40];
uint8_t read_enable,receive_finished;


uint16_t calccrc(uint8_t crcbuf,uint16_t crc)
{
	uint8_t i;
	uint8_t chk;
	crc=crc ^ crcbuf;
	for(i=0;i<8;i++)
	{
		chk=( uint8_t)(crc&1);
		crc=crc>>1;
		crc=crc&0x7fff;
		if (chk==1)
		crc=crc^0xa001;
		crc=crc&0xffff;
	}
	return crc;
}
uint16_t chkcrc(uint8_t *buf,uint8_t len)
{
	uint8_t hi,lo;
	uint16_t i;
	uint16_t crc;
	crc=0xFFFF;
	for(i=0;i<len;i++)
	{
		crc=calccrc(*buf,crc);
		buf++;
	}
	hi=( uint8_t)(crc%256);
	lo=( uint8_t)(crc/256);
	crc=(((uint16_t)(hi))<<8)|lo;
	return crc;
}

void IM1253B_readData(void)
{
	
	USARTSend_t usartSend;
	uint8_t sendBuff[8];
	usartSend.pUSARTSendBuff=sendBuff;
	union crcdata
	{
		uint16_t word16;
		uint8_t byte[2];
	}crcnow;
	
	usartSend.USART_x=USART_3;
	
//	if(read_enable==1) // 到时间抄读模块，抄读间隔 1 秒钟(或其他)
	{
//		read_enable=0;
		usartSend.pUSARTSendBuff[0]=Read_ID; //模块的 ID 号，默认 ID 为 0x01
		usartSend.pUSARTSendBuff[1]=0x03;
		usartSend.pUSARTSendBuff[2]=0x00;
		usartSend.pUSARTSendBuff[3]=0x48;
		usartSend.pUSARTSendBuff[4]=0x00;
		usartSend.pUSARTSendBuff[5]=0x06;
		crcnow.word16=chkcrc(usartSend.pUSARTSendBuff,6);
		usartSend.pUSARTSendBuff[6]=crcnow.byte[1]; //CRC 效验低字节在前
		usartSend.pUSARTSendBuff[7]=crcnow.byte[0];
		
		xQueueSend(xusartTxQueue, &usartSend, 20);
		//Send_data(8); //发送 8 个数据，请根据单片机类型自己编程
	}
}
void IM1253B_getMessage(uint8_t *RX_Buffer,uint8_t reveive_number)
{
	
	union crcdata
	{
		uint16_t word16;
		uint8_t byte[2];
	}crcnow;
//	if(receive_finished==1) //接收完成
	{
		receive_finished=0;
		if(RX_Buffer[0]==Read_ID) //确认 ID 正确
		{
			crcnow.word16=chkcrc(RX_Buffer,reveive_number-2); //reveive_numbe 是接收数据总长度
//			if((crcnow.byte[0]==RX_Buffer[reveive_number-1])&&(crcnow.byte[1]==RX_Buffer[reveive_number-2]))
			{
				IM1253B.Voltage_data=(((uint32_t)(RX_Buffer[3]))<<24)|((( 
				uint32_t)(RX_Buffer[4]))<<16)|(((uint32_t)(RX_Buffer[5]))<<8)|RX_Buffer[6];
				
				IM1253B.Current_data=(((uint32_t)(RX_Buffer[7]))<<24)|((( 
				uint32_t)(RX_Buffer[8]))<<16)|(((uint32_t)(RX_Buffer[9]))<<8)|RX_Buffer[10];
				IM1253B.Power_data=(((uint32_t)(RX_Buffer[11]))<<24)|((( 
				uint32_t)(RX_Buffer[12]))<<16)|(((uint32_t)(RX_Buffer[13]))<<8)|RX_Buffer[14];
				IM1253B.Energy_data=(((uint32_t)(RX_Buffer[15]))<<24)|((( 
				uint32_t)(RX_Buffer[16]))<<16)|(((uint32_t)(RX_Buffer[17]))<<8)|RX_Buffer[18];
				IM1253B.Pf_data=(((uint32_t)(RX_Buffer[19]))<<24)|((( 
				uint32_t)(RX_Buffer[20]))<<16)|(((uint32_t)(RX_Buffer[21]))<<8)|RX_Buffer[22];
				IM1253B.CO2_data=(((uint32_t)(RX_Buffer[23]))<<24)|((( 
				uint32_t)(RX_Buffer[24]))<<16)|(((uint32_t)(RX_Buffer[25]))<<8)|RX_Buffer[26];
			}
		}
	}
}

