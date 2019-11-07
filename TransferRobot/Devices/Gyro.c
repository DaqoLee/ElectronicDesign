/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Gyro.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-02-23               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Gyro.h" 
  
/*-------------------------- D E F I N E S -----------------------------------*/

GY955_t GY955;

/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/

static void GY955_getMassage(uint8_t *pGyroData);

/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-02-23 10:45
  * @brief   陀螺仪初始化
  * @param   void
  * @retval  void
  */
  void Gyro_Init(void)
  {
    GY955.pGY955getMsg = GY955_getMassage;
  }

/*---------------------L O C A L - F U N C T I O N S--------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-02-23 10:27
  * @brief   解析GY955数据
  * @param   uint8_t *pGyroData
  * @retval  void
  */
  static void GY955_getMassage(uint8_t *pGyroData)
  {
    uint8_t sum=0,i=0;
    for(;i<(pGyroData[3]+4);i++)
    {
      sum += pGyroData[i];
    }
    if(sum == pGyroData[i])/*校验和判断*/
    {
      GY955.Gyr_X = ((pGyroData[4]<<8)|pGyroData[5]);
      GY955.Gyr_Y = ((pGyroData[6]<<8)|pGyroData[7]);
      GY955.Gyr_Z = ((pGyroData[8]<<8)|pGyroData[9]);

      GY955.Yaw = (uint16_t)((pGyroData[10]<<8)|pGyroData[11])/100.0f;
      GY955.Roll = (int16_t)((pGyroData[12]<<8)|pGyroData[13])/100.0f;
      GY955.Pitch = (int16_t)((pGyroData[14]<<8)|pGyroData[15])/100.0f;
    }  
  }

/*-----------------------------------FILE OF END------------------------------*/




