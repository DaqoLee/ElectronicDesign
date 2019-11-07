/************************************************************************************
*  Copyright (c), 2014, HelTec Automatic Technology co.,LTD.
*            All rights reserved.
*
* Http:    www.heltec.cn
* Email:   cn.heltec@gmail.com
* WebShop: heltec.taobao.com
*
* File name: OLED_I2C.c
* Project  : HelTec.uvprij
* Processor: STM32F103C8T6
* Compiler : MDK fo ARM
* 
* Author : 小林
* Version: 1.00
* Date   : 2014.4.8
* Email  : hello14blog@gmail.com
* Modification: none
* 
* Description:128*64点阵的OLED显示屏驱动文件，仅适用于惠特自动化(heltec.taobao.com)的SD1306驱动IIC通信方式显示屏
*
* Others: none;
*
* Function List:
*	1. void I2C_Configuration(void) -- 配置CPU的硬件I2C
* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- 向寄存器地址写一个byte的数据
* 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
* 4. void WriteDat(unsigned char I2C_Data) -- 写数据
* 5. void OLED_Init(void) -- OLED屏初始化
* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
* 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
* 8. void OLED_CLS(void) -- 清屏
* 9. void OLED_ON(void) -- 唤醒
* 10. void OLED_OFF(void) -- 睡眠
* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- 显示中文(中文需要先取模，然后放到codetab.h中)
* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
*
* History: none;
*
*************************************************************************************/
/////////////////////////////////////////////////
//在Text6时0x06*n其中n为隔几个字符22各字
///////////////////////////////////////////////
#include "Dirver_OLED.h"
#include "codetab.h"
#include "i2c.h"
I2C_HandleTypeDef* user_i2c;

void I2C_Configuration(I2C_HandleTypeDef* _i2c_)
{
		user_i2c=_i2c_;
}

void I2C_WriteByte(uint8_t addr,uint8_t data)
{

}

void WriteCmd(unsigned char I2C_Command)//写命令
{
	uint8_t temp[2]={0x00,I2C_Command};
	HAL_I2C_Master_Transmit(user_i2c,OLED_ADDRESS,temp,2,0xffff);
}

void WriteDat(unsigned char I2C_Data)//写数据
{
	uint8_t temp[2]={0x40,I2C_Data};
	HAL_I2C_Master_Transmit(user_i2c,OLED_ADDRESS,temp,2,0xffff);
}

void OLED_Init(void)
{
	//HAL_StatusTypeDef flag;
	I2C_Configuration(&hi2c2);
	uint8_t Cmd[]={0x00,0xAE,0x20,0x10,0xb0,0xc8,0x00,0x10,\
								 0x40,0x81,0xff,0xa1,0xa6,0xa8,0x3F,\
								 0xa4,0xd3,0x00,0xd5,0xf0,0xd9,0x22,\
								 0xda,0x12,0xdb,0x20,0x8d,0x14,0xaf};

	HAL_Delay(100);//这里的延时很重要
	HAL_I2C_Master_Transmit(user_i2c,OLED_ADDRESS,(uint8_t*)Cmd,29,0xffff);
								 
	OLED_Fill(0xff);
    OLED_Fill(0x00);//清屏
								 
								 
//   Play_OLED_Chassis_Init();
}

void OLED_SetPos(unsigned char x, unsigned char y,uint8_t flag) //设置起始点坐标
{ 
//		WriteCmd(0xb0+y);
//		WriteCmd(((x&0xf0)>>4)|0x10);
//		WriteCmd((x&0x0f)|0x01);
		uint8_t temp[5]={0x00};
		temp[1]=0xb0+y;
		temp[2]=(x&0x0f)|0x00;
		temp[3]=((x&0xf0)>>4)|0x10;
		temp[4]=0x04;
		
//		if(flag)
//		{
//				//temp[2]=(x&0xf0>>4)|0x10;
//				
//				temp[3]=((x&0x0f)|0x01);
//			  temp[4]=0x04;
//		}
		//temp[3]=((x&0x0f)|0x01);
		HAL_I2C_Master_Transmit(user_i2c,OLED_ADDRESS,temp,temp[4],0xffff);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n,temp[4];
	for(m=0;m<8;m++)
	{
		temp[0]=0x00;//命令
		temp[1]=0xb0+m;//页的起始地址
		temp[2]=0x00;//low column start address低4位	
		temp[3]=0x00;//high column start address高4位
		HAL_I2C_Master_Transmit(user_i2c,OLED_ADDRESS,(uint8_t*)temp,4,0xffff);
		for(n=0;n<128;n++)
		{
			WriteDat(fill_Data);
		}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
		//设置电荷泵  0x8D
		//开启电荷泵  0x14
		//OLED唤醒   0xAF
		uint8_t temp[5]={0x00,0x8D,0x14,0x00,0xAF};
		HAL_I2C_Master_Transmit(user_i2c,OLED_ADDRESS,temp,5,0xffff);
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
		//设置电荷泵  0x8D
		//关闭电荷泵  0x10
		//OLED休眠    0xAE
		uint8_t temp[5]={0x00,0x8D,0x10,0x00,0xAE};
		HAL_I2C_Master_Transmit(user_i2c,OLED_ADDRESS,temp,5,0xffff);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], _TextSize_ TextSize)
{
		unsigned char c = 0,i = 0,j = 0;
		switch(TextSize)
		{
				case Text6:
				{
						while(ch[j] != '\0')
						{
							c = ch[j] - 32;
							if(x > 126)
							{
									x = 0;
									y++;
							}
							OLED_SetPos(x,y,0);
							for(i=0;i<6;i++)
								WriteDat(F6x8[c][i]);
							x += 6;
							j++;
						}
				}break;
				case Text8:
				{
						while(ch[j] != '\0')
						{
								c = ch[j] - 32;
								if(x > 120)
								{
										x = 0;
										y++;
								}
								OLED_SetPos(x,y,1);
								for(i=0;i<8;i++)
									WriteDat(F8X16[c*16+i]);
								OLED_SetPos(x,y+1,1);
								for(i=0;i<8;i++)
									WriteDat(F8X16[c*16+i+8]);
								x += 8;
								j++;
						}
				}break;
		}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------
//void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
//{
//	unsigned char wm=0;
//	unsigned int  adder=32*N;
//	OLED_SetPos(x , y,0);
//	for(wm = 0;wm < 16;wm++)
//	{
//		WriteDat(F16x16[adder]);
//		adder += 1;
//	}
//	OLED_SetPos(x,y + 1,1);
//	for(wm = 0;wm < 16;wm++)
//	{
//		WriteDat(F16x16[adder]);
//		adder += 1;
//	}
//}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
//void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
//{
//	unsigned int j=0;
//	unsigned char x,y;

//  if(y1%8==0)
//		y = y1/8;
//  else
//		y = y1/8 + 1;
//	for(y=y0;y<y1;y++)
//	{
//		OLED_SetPos(x0,y,0);
//    for(x=x0;x<x1;x++)
//		{
//			WriteDat(BMP[j++]);
//		}
//	}
//}
///////////////////////////////////////////////////////
////把显示数据转换成字符串显示在给定的x，y位置上字符大小是6*8
////signedData为时0为无符号显示
////////////////////////////////////////////////////////
void OLED_ShowData_16(uint8_t x,uint8_t y,uint16_t data,uint8_t signedData)
{
		char str[6]="     0";
		uint8_t Miriade,Thousand,Hundred,Ten,Single;//万，千，百，十
		int16_t temp=0;
		if(signedData)
		{
				if(data&0x8000)//为负数
				{
						temp=data;
						temp=-temp;
						str[0]='-';
						data=temp;//去掉最高位
				}
		}
		Miriade=data/10000;
		Thousand=(data%10000)/1000;//得到千位
		Hundred=(data%1000)/100;
		Ten=(data%100)/10;
		Single=(data%10);
		if(Miriade==0)
		{
				str[1]=' ';
				if(Thousand==0)
				{
						str[2]=' ';	
						if(Hundred==0)
						{
								str[3]=' ';
								if(Ten==0)
								{
										str[4]=' ';
										str[5]='0'+Single;
								}
								else
								{
										str[4]='0'+Ten;
										str[5]='0'+Single;
								}
						}
						else
						{
								str[3]='0'+Hundred;
								str[4]='0'+Ten;
								str[5]='0'+Single;
						}
				}
				else 
				{
						str[2]='0'+Thousand;
						str[3]='0'+Hundred;
						str[4]='0'+Ten;
						str[5]='0'+Single;
				}
		}
		else 
		{
				str[1]='0'+Miriade;
				str[2]='0'+Thousand;
				str[3]='0'+Hundred;
				str[4]='0'+Ten;
				str[5]='0'+Single;
		}
		OLED_ShowStr(x,y,(uint8_t*)str,Text6);
}


void Play_OLED_Chassis_Init()
{
	OLED_Fill(0xff);
	OLED_Fill(0x00);//清屏
	OLED_ShowStr(0*6,0,(unsigned char*)"Power_Max",Text6);
	OLED_ShowStr(0*6,4,(unsigned char*)"Pbuff_Min",Text6);
}


void Play_OLED_Chassis()
{
	OLED_ShowData_16(8*6,0,Judge_RobotPowerHeatData.chassisVolt,1);
	OLED_ShowData_16(8*6,2,Judge_RobotPowerHeatData.chassisCurrent*1000,1);
	OLED_ShowData_16(8*6,4,Judge_RobotPowerHeatData.chassisPower,1);
//	OLED_ShowData_16(2*6,6,ChassisParam.LB.Real_Current,1);
//	OLED_ShowData_16(10*6,6,ChassisParam.LB.Target_Current,1);
	
}




