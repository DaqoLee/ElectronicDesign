/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|--FileName    : Camera.c                                              
|--Version     : v1.0                                                          
|--Author      : Daqo Lee                                                     
|--Date        : 2019-03-17             
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                     
|-----------------------------declaration of end-----------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Camera.h" 
#include "DR16.h"
/*-------------------------- D E F I N E S -----------------------------------*/

Camera_t Camera;
Laser_t Laser;
/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/



/*------------------G L O B A L - F U N C T I O N S --------------------------*/

void Camera_getMassage(uint8_t *CameraBuff)
{
  for(uint8_t i=0;i<15;i++)
	{
		if(CameraBuff[i]=='$')
		{
		  if(CameraBuff[i+4] == ',' && CameraBuff[i+8] == ',')
			{
				if(CameraBuff[i+1]=='0')
				{
					Camera.Angle =  ((CameraBuff[i+2] - '0') * 10
							           + (CameraBuff[i+3] - '0'))*-1;
				}
				else
        {
					Camera.Angle = (CameraBuff[i+2] - '0') * 10
					               + (CameraBuff[i+3] - '0');
				}


				Camera.Offset =
			  	(CameraBuff[i+5] - '0') * 100
			  +	(CameraBuff[i+6] - '0') * 10
        + (CameraBuff[i+7] - '0');

				Camera.Flag = CameraBuff[i+9] - '0';
			}
			else
			{
				
			}
			
		}
	}
}


void Laser_getMessage(uint8_t *LaserBuf)
{
	static uint16_t lastX=0,lastY=0;
	/*减去字符‘0’得到实际数值*/
  Laser.X = ((LaserBuf[0]-'0')*100+(LaserBuf[1]-'0')*10+(LaserBuf[2]-'0'));
	Laser.Y = ((LaserBuf[3]-'0')*100+(LaserBuf[4]-'0')*10+(LaserBuf[5]-'0'));
	
	if(Laser.X>480||Laser.Y>480)/*限幅*/
	{
	  Laser.X = lastX;
		Laser.Y = lastY;
	}
	
	Laser.Flag = LaserBuf[6] - '0';
	Laser.Num = LaserBuf[7] - '0';
#if 1
	if(DR16.switch_left == 1)
	{
	   	Laser.TarX = ((LaserBuf[8]-'0')*100+(LaserBuf[9]-'0')*10+(LaserBuf[10]-'0'));
    	Laser.TarY = ((LaserBuf[11]-'0')*100+(LaserBuf[12]-'0')*10+(LaserBuf[13]-'0'));
		
	}else if(DR16.switch_left == 2)/*校准*/
	{
		if(DR16.switch_right == 1)
		{
				Laser.TarX=0;
				Laser.TarY=0;	
		}
		else if(DR16.switch_right == 3)
		{
			Laser.TarX=240;
			Laser.TarY=240;
		}
		else
		{
			Laser.TarX=480;
			Laser.TarY=480;
		}
	}
	else
	{
		switch(Laser.Num)/*根据环数给定坐标*/
			{
				case 5:
					Laser.TarX=0;
					Laser.TarY=240;
					break;
				case 6:
					Laser.TarX=60;
					Laser.TarY=240;			
					break;
				case 7:
					Laser.TarX=100;
					Laser.TarY=240;			
					break;
				case 8:
					Laser.TarX=140;
					Laser.TarY=240;			
					break;
				case 9:
					Laser.TarX=180;
					Laser.TarY=240;			
					break;
				case 10:
					Laser.TarX=240;
					Laser.TarY=240;			
					break;
				default:
					
					break;
			}
	}

	Laser.ON=LaserBuf[14]-'0';
#endif
	lastX = Laser.X;
	lastY = Laser.Y;
	
}
/*---------------------L O C A L - F U N C T I O N S--------------------------*/



/*-----------------------------------file of end------------------------------*/


