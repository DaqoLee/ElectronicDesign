#include "BSP_I2C.h"


void BSP_I2C2_Init(void)
{

  I2C_InitTypeDef   I2C_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB1PeriphClockCmd(I2C2_SDA_GPIO_CLK|I2C2_SCL_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;    
	
	GPIO_InitStructure.GPIO_Pin   =  I2C2_SCL_Pin|I2C2_SDA_Pin ;
	GPIO_Init(I2C2_SCL_GPIO_PORT, &GPIO_InitStructure);
	
//	GPIO_PinAFConfig(I2C2_SDA_GPIO_PORT, I2C2_SDA_PINSOURCE, GPIO_AF_I2C2);
//	GPIO_PinAFConfig(I2C2_SCL_GPIO_PORT, I2C2_SCL_PINSOURCE, GPIO_AF_I2C2);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
	I2C_DeInit(I2C2);//使用I2C2
	I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1         = 0x30;
	I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed          = 400000;

	I2C_Cmd(I2C2, ENABLE);
	I2C_Init(I2C2, &I2C_InitStructure);
}


void I2C_WriteByte(uint8_t addr,uint8_t data)
{
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C2, ENABLE);
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	I2C_Send7bitAddress(I2C2, 0x78, I2C_Direction_Transmitter);  
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C2, addr);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C2, data);
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C2, ENABLE);
}
