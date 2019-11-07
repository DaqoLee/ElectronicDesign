#include "User.h"

uint8_t Ultrasonic_Message[20];
uint8_t RFID_RC522_Message[12];
void Ultrasonic_Transmit(uint8_t Length,uint16_t Data1,uint16_t Data2,uint16_t Data3,uint16_t Data4,\
	               uint16_t Data5,uint16_t Data6,uint16_t Data7,uint16_t Data8)
{
	Ultrasonic_Message[0]=0xAA;
	Ultrasonic_Message[1]=0xAA;
	Ultrasonic_Message[2]=Data1>>8;
	Ultrasonic_Message[3]=Data1;
	Ultrasonic_Message[4]=Data2>>8;
	Ultrasonic_Message[5]=Data2;
	Ultrasonic_Message[6]=Data3>>8;
	Ultrasonic_Message[7]=Data3;
	Ultrasonic_Message[8]=Data4>>8;
	Ultrasonic_Message[9]=Data4;
	Ultrasonic_Message[10]=Data5>>8;
	Ultrasonic_Message[11]=Data5;
	Ultrasonic_Message[12]=Data6>>8;
	Ultrasonic_Message[13]=Data6;
	Ultrasonic_Message[14]=Data7>>8;
	Ultrasonic_Message[15]=Data7;
	Ultrasonic_Message[16]=Data8>>8;
	Ultrasonic_Message[17]=Data8;
	Ultrasonic_Message[18]=0xAA;
	Ultrasonic_Message[19]=0xAA;	
	
	HAL_UART_Transmit_DMA(&huart3,Ultrasonic_Message,Length);
}

void RFID_RC522_Transmit(uint8_t Length,char *str)
{
	RFID_RC522_Message[0]=0xFF;
	RFID_RC522_Message[1]=0xFF;
	RFID_RC522_Message[2]=str[0];
	RFID_RC522_Message[3]=str[1];
	RFID_RC522_Message[4]=str[2];
	RFID_RC522_Message[5]=str[3];
	RFID_RC522_Message[6]=str[4];
	RFID_RC522_Message[7]=str[5];
	RFID_RC522_Message[8]=str[6];
	RFID_RC522_Message[9]=str[7];
	RFID_RC522_Message[10]=0xFF;
	RFID_RC522_Message[11]=0xFF;	
	HAL_UART_Transmit_DMA(&huart3,RFID_RC522_Message,Length);

}

