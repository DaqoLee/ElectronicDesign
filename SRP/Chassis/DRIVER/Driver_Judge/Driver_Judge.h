#ifndef __DRIVER_JUDGE_H
#define __DRIVER_JUDGE_H

#include "stm32f4xx.h"
#define JudgeBufferLength       150
#define JudgeFrameLength_1      17
#define JudgeFrameLength_2      10
#define JudgeFrameLength_3      15
#define JudgeFrameLength_4      29
#define JudgeFrameLength_5      11
#define JudgeFrameLength_6      11
#define JudgeFrameLength_7      12
#define JudgeFrameLength_8      25
#define JudgeFrameLength_100    22
#define JudgeFrameLength_4_8    54

#define JudgeFrameHeader        0xA5        //帧头 
#define JudageDataOffset		0x07		//数据偏移量

#ifdef  __DRIVER_GLOBALS
#define __DRIVER_EXT
#else
#define __DRIVER_EXT extern
#endif
/////////////比赛机器人状态//////////////////
typedef __packed struct
{
		uint8_t validFlag;
		float x;
		float y;
		float z;
		float yaw;
}position_t;
typedef __packed struct
{
		uint16_t stageRemianTime;
		uint8_t gameProgress;
		uint8_t robotLevel;
		uint16_t remainHP;
		uint16_t maxHP;
}extGameRobotState_t;
typedef __packed struct
{
		uint8_t armorType;
		uint8_t hurtType;
}extRobotHurt_t;
typedef __packed struct
{
		uint8_t bulletType;
		uint8_t bulletFreq;
		float bulletSpeed;
}extShootData_t;
typedef __packed struct
{
		float chassisVolt;
		float chassisCurrent;
		float chassisPower;
		float chassisPowerBuffer;
		uint16_t shooterHeat0;
		uint16_t shooterHeat1;
}extPowerHeatData_t;
typedef __packed struct
{
		uint8_t cardType;
		uint8_t cardIdx;
}extRfidDetect_t;
typedef __packed struct
{
		uint8_t buffType;
		uint8_t buffAddition;
}extGetBuff_t;
typedef __packed struct
{
		float x;
		float y;
		float z;
		float yaw;
}extGameRobotPos_t;
typedef __packed struct
{
		float data1;
		float data2;
		float data3;
		uint8_t mask;
}extShowData_t;
///////////////////////////////////////////




//小符状态枚举
typedef enum
{
    BUFF_TYPE_NONE, //无效
    BUFF_TYPE_ARMOR = 0x01, //防御符
    BUFF_TYPE_SUPPLY = 0x04, //加血符
    BUFF_TYPE_BULLFTS= 0x08, //加弹符
}LBuffType_Enum;


//位置状态结构体
typedef __packed struct
{
    uint8_t flag; //0 无效， 1 有效
    uint32_t x;
    uint32_t y;
    uint32_t z;
    uint32_t compass;
}GpsData_Struct;


//比赛进程信息结构体
typedef __packed struct
{
    uint32_t remainTime;
    uint16_t remainLifeValue;
    float realChassisOutV;
    float realChassisOutA;
    uint8_t runeStatus[4];
    uint8_t bigRune0Status;
    uint8_t bigRune1status;
    uint8_t conveyorBelts0:2;
    uint8_t conveyorBelts1:2;
    uint8_t parkingApron0:1;
    uint8_t parkingApron1:1;
    uint8_t parkingApron2:1;
    uint8_t parkingApron3:1;
    GpsData_Struct gpsData;
}GameInfo_Struct;


//实时血量变化信息结构体
typedef __packed struct
{
    uint8_t weakId:4;
    uint8_t way:4;
    uint16_t value;
}RealBloodChangedData_Struct;


//实时射击信息结构体
typedef __packed struct
{
    float realBulletShootSpeed;
    float realBulletShootFreq;
    float realGolfShootSpeed;
    float realGolfShootFreq;
}RealShootData_Struct;


//裁判系统结构体
typedef struct
{
    float RealVoltage;                  //实时电压
    float RealCurrent;                  //实时电流
    int16_t LastBlood;                  //剩余血量
    uint8_t LastHartID;                 //上次收到伤害的装甲板ID号
//    portTickType LastHartTick;          //上次受伤害时间 
    float LastShotSpeed;                //上次射击速度
//    portTickType LastShotTick;          //上次射击时间
#if INFANTRY == 7
    uint16_t ShootNum;                  //已发射子弹数
    uint8_t BulletUseUp;                //1 基地子弹射完          0 基地子弹未射完
    uint16_t ShootFail;                 //发射失败时间 
#endif
}InfantryJudge_Struct;


//裁判系统数据缓存
__DRIVER_EXT uint8_t JudgeDataBuffer[JudgeBufferLength];
//实时电压
__DRIVER_EXT InfantryJudge_Struct InfantryJudge;
//帧率计数器
__DRIVER_EXT float JudgeFrameCounter;
//帧率
__DRIVER_EXT float JudgeFrameRate;

extern uint8_t Judge_Power_Limit;


//裁判系统机器人状态
__DRIVER_EXT extGameRobotState_t Judge_RobotState;
//伤害数据
__DRIVER_EXT extRobotHurt_t	Judge_RobotHurt;
//实时射击信息
__DRIVER_EXT extShootData_t Judge_RobotShootData;
//实时功率热量数据
__DRIVER_EXT extPowerHeatData_t Judge_RobotPowerHeatData;
//场地交互数据
__DRIVER_EXT extRfidDetect_t Judge_RobotRfidDetect;
//buff获取数据
__DRIVER_EXT extGetBuff_t Judge_RobotGetBuff;
//机器人位置朝向信息
__DRIVER_EXT extGameRobotPos_t Judge_RobotPos;


unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);
void Analysis_Judge(UART_HandleTypeDef* huart);
void Judge_InitConfig(void);
void Transmit_Judge(uint32_t dwLength,float Data1,float Data2,float Data3,uint8_t mask);


#endif
