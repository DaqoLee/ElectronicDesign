#ifndef __MPU6500_H
#define __MPU6500_H
#include "spi.h"
#include "ist8310.h"
#define GPIOF_ODR_Addr 0x40021414 
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 


// 定义MPU9250内部地址
/*****************************************************************/
#define	SMPLRT_DIV		                      0x19	//陀螺仪采样率
#define	CONFIG			                        0x1A	
#define	GYRO_CONFIG		                      0x1B	
#define	ACCEL_CONFIG	                      0x1C	
#define	ACCEL_CONFIG_2                      0x1D 

#define MPU_INT_EN_REG			0X38	//中断使能寄存器
#define INT_PIN_CFG                         0x37 //中断配置
#define USER_CTRL                           0x6a
#define I2C_MST_CTRL                        0x24
#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存器
#define I2C_MST_DELAY_CTRL                  0x67
#define MPU_MDETECT_CTRL_REG	0X69	//运动检测控制寄存器
//--------------------i2c slv0-------------------------------//
#define I2C_SLV0_ADDR                       0x25  
#define I2C_SLV0_REG                        0x26
#define I2C_SLV0_CTRL                       0x27 
//#define MPU6500_I2C_SLV1_ADDR       				0x28
#define I2C_SLV0_DO                         0x63 //output reg
//#define MPU6500_I2C_SLV4_ADDR      					0x31
#define MPU6500_I2C_SLV1_CTRL       				(0x2A)
#define MPU6500_I2C_SLV1_REG        				(0x29)
#define MPU6500_I2C_SLV1_DO         				(0x64)
#define MPU6500_I2C_SLV4_REG        				(0x32)
#define MPU6500_I2C_SLV4_CTRL       				(0x34)
#define MPU6500_I2C_SLV4_DI         				(0x35)
#define MPU6500_I2C_MST_DELAY_CTRL  				(0x67)
#define MPU6500_I2C_SLV4_DO         (0x33)
#define MPU6500_EXT_SENS_DATA_04    (0x4D)
#define MPU6500_EXT_SENS_DATA_01    (0x4A)
//--------------------AK8963 reg addr------------------------//
#define MPU9250_AK8963_ADDR                 0x0C  //AKM addr
#define AK8963_WHOAMI_REG                   0x00  //AKM ID addr
#define AK8963_WHOAMI_ID                    0x48  //ID
#define AK8963_ST1_REG                      0x02  //Data Status1
#define AK8963_ST2_REG                      0x09  //Data reading end register & check Magnetic sensor overflow occurred
#define AK8963_ST1_DOR                      0x02
#define AK8963_ST1_DRDY                     0x01 //Data Ready
#define AK8963_ST2_BITM                     0x10
#define AK8963_ST2_HOFL                     0x08 // Magnetic sensor overflow 
#define AK8963_CNTL1_REG                    0x0A
#define AK8963_CNTL2_REG                    0x0B
#define AK8963_CNTL2_SRST                   0x01 //soft Reset
#define AK8963_ASAX                         0x10 //X-axis sensitivity adjustment value 
#define AK8963_ASAY                         0x11 //Y-axis sensitivity adjustment value
#define AK8963_ASAZ                         0x12 //Z-axis sensitivity adjustment value
#define MAG_WIA															0x00	//AK8963的器件ID寄存器地址
//--------------------9axis  reg addr-----------------------//
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41   //temperture
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
		
#define MAG_XOUT_L		0x03
#define MAG_XOUT_H		0x04
#define MAG_YOUT_L		0x05
#define MAG_YOUT_H		0x06
#define MAG_ZOUT_L		0x07
#define MAG_ZOUT_H		0x08
//--------------------other reg addr-----------------------//
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define PWR_MGMT_2		0X6C	//电源管理寄存器2 
#define	WHO_AM_I		  0x75	//ID地址寄存器(正确数值0x71，只读)

#define EXT_SENS_DATA_00    0x49  //MPU9250 IIC外挂器件读取返回寄存器00
#define EXT_SENS_DATA_01    0x4a  //MPU9250 IIC外挂器件读取返回寄存器01
#define EXT_SENS_DATA_02    0x4b  //MPU9250 IIC外挂器件读取返回寄存器02
#define EXT_SENS_DATA_03    0x4c  //MPU9250 IIC外挂器件读取返回寄存器03

#define MPU_ADDR				0X70
/************************SPI CS ********************************/
#define spi_CS	PFout(6)
#define BYTE16(Type, ByteH, ByteL)  ((Type)((((uint16_t)(ByteH))<<8) | ((uint16_t)(ByteL))))

typedef struct{
	short Accel[3];//Accel X,Y,Z
	short Gyro[3];//Gyro X,Y,Z
	short Mag[3];	//Mag X,Y,Z	
}MPU_value;

extern int16_t accel_x,accel_y,accel_z,groy_x,groy_y,groy_z;
extern MPU_value mpu_value;

void susart_printf(void);
uint8_t Init_MPU6500(void);
void READ_MPU6500_ACCEL(void);//读取加速度
void READ_MPU6500_GYRO(void);//读取陀螺仪
void READ_MPU6500_MAG(void);//读取地磁计
void Printf(void);
void MPU6500_Write_Reg(uint8_t reg,uint8_t value);
uint8_t MPU6500_Read_Reg(uint8_t reg);
static void spi_Mag_write(uint8_t reg,uint8_t value);
static uint8_t spi_Mag_read(uint8_t reg);
short Read_MPU6500(void);
void MPU6500_ReadValueRaw(void);
uint8_t SPI5_Read_Write_Byte(uint8_t TxData);
//void spi_Mag_write(uint8_t reg, uint8_t value);
//uint8_t spi_Mag_read(uint8_t reg);
//uint8_t MPU6500_Write_Reg(uint8_t reg,uint8_t value);
//uint8_t MPU6500_Read_Reg(uint8_t reg);
//uint8_t SPI5_Read_Write_Byte(uint8_t TxData);

#endif


