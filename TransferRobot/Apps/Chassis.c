/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Chassis.c                                                
|  Version     : v1.0                                                            
|  Author      : Daqo Lee                                                       
|  Date        : 2019-02-20               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/
#include "Chassis.h"
#include "RoboModule.h"
#include "math.h"
#include "DR16.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Encoder.h"
#include "Curve.h"
#include "Filter.h"
#include "Motor.h"
#include "Camera.h" 
#include "Cylinder.h"
/*-------------------------- D E F I N E S -----------------------------------*/
const float Radian=PI/6;

Chassis_t Chassis;

int16_t Speed=0;
int16_t Xcoord,Ycoord;
uint8_t Flag=0;
static float Slope[]=
{
	0.000000, 0.085480, 0.170623, 0.255093, 0.338556, 
	0.420682, 0.501149, 0.579638, 0.655839, 0.729451, 
	0.800185, 0.867761, 0.931913, 0.992386, 1.048943, 
	1.101361, 1.149432, 1.192966, 1.231792, 1.265758, 
	1.294727, 1.318587, 1.337243, 1.350622, 1.358670, 
	1.361357, 1.358670, 1.350622, 1.337243, 1.318587, 
	1.294727, 1.265758, 1.231792, 1.192966, 1.149432, 
	1.101361, 1.048943, 0.992386, 0.931913, 0.867761, 
	0.800185, 0.729451, 0.655839, 0.579638, 0.501149, 
	0.420682, 0.338556, 0.255093, 0.170623, 0.085480, 
	0.000000, -0.085480, -0.170623, -0.255093, -0.338556, 
	-0.420682, -0.501149, -0.579638, -0.655839, -0.729451, 
	-0.800185, -0.867761, -0.931913, -0.992386, -1.048943, 
	-1.101361, -1.149432, -1.192966, -1.231792, -1.265758, 
	-1.294727, -1.318587, -1.337243, -1.350622, -1.358670, 
	-1.361357, -1.358670, -1.350622, -1.337243, -1.318587, 
	-1.294727, -1.265758, -1.231792, -1.192966, -1.149432, 
	-1.101361, -1.048943, -0.992386, -0.931913, -0.867761, 
	-0.800185, -0.729451, -0.655839, -0.579638, -0.501149, 
	-0.420682, -0.338556, -0.255093, -0.170623, -0.085480, 
	-0.000000, 0.085480, 0.170623, 0.255093, 0.338556, 
	0.420682, 0.501149, 0.579638, 0.655839, 0.729451, 
	0.800185, 0.867761, 0.931913, 0.992386, 1.048943, 
	1.101361, 1.149432, 1.192966, 1.231792, 1.265758, 
	1.294727, 1.318587, 1.337243, 1.350622, 1.358670, 
	1.361357, 1.358670, 1.350622, 1.337243, 1.318587, 
	1.294727, 1.265758, 1.231792, 1.192966, 1.149432, 
	1.101361, 1.048943, 0.992386, 0.931913, 0.867761, 
	0.800185, 0.729451, 0.655839, 0.579638, 0.501149, 
	0.420682, 0.338556, 0.255093, 0.170623, 0.085480, 
	0.000000, -0.085480, -0.170623, -0.255093, -0.338556, 
	-0.420682, -0.501149, -0.579638, -0.655839, -0.729451, 
	-0.800185, -0.867761, -0.931913, -0.992386, -1.048943, 
	-1.101361, -1.149432, -1.192966, -1.231792, -1.265758, 
	-1.294727, -1.318587, -1.337243, -1.350622, -1.358670
};
/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/
void SpinRuning(void);
void SetCoord(int16_t X,int16_t Y);
/*------------------G L O B A L - F U N C T I O N S --------------------------*/

/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-02-20 16:59
  * @brief  底盘运动模型分解
  * @param  [in] Vx     X 轴方向速度
  *         [in] Vy     Y 轴方向速度
  *         [in] Omega  自旋速度
  *         [in] *Speed 速度分解缓存 
  * @retval None
  */
void Chassis_MotionModel(float Vx, float Vy, float Omega, int16_t *Speed)								
{
#if   MECANUM == 1
  Speed[0] = -Vx - Vy + Omega;
  Speed[1] = -Vx + Vy + Omega;
  Speed[2] =  Vx + Vy + Omega;
  Speed[3] =  Vx - Vy + Omega;
#elif	THROMNI == 1
	Speed[0] = -Vx + Omega;
  Speed[1] =  Vx*sin(Radian) - Vy*cos(Radian) + Omega;
  Speed[2] =  Vx*sin(Radian) + Vy*cos(Radian) + Omega;
#elif FOROMNI == 1
	Speed[0] = -Vx - Vy + Omega;
  Speed[1] = -Vx + Vy + Omega;
  Speed[2] =  Vx + Vy + Omega;
  Speed[3] =  Vx - Vy + Omega;
	
#endif

}

/*---------------------L O C A L - F U N C T I O N S--------------------------*/

/**
  * @Data    2019-03-19 10:56
  * @brief   底盘参数初始化
  * @param   void
  * @retval  void
  */
  void Chassis_Init(void)
  {
		
    PID_StructInit(&Chassis.PID_X, POSITION_PID, 2000, 500, 10, 0, 0);
    PID_StructInit(&Chassis.PID_Y, POSITION_PID, 2000, 500, 10, 0, 0);
		
    PID_StructInit(&Gripper.xPID, POSITION_PID, 2000, 1000, 10, 0.01, 25);
    PID_StructInit(&Gripper.yPID, POSITION_PID, 2000, 1000, 10, 0.01, 25);
		
    PID_StructInit(&Chassis.PID_Spin, POSITION_PID, 2000, 500, 50, 0, 0);
		for(uint8_t i=0;i<4;i++)
		{
		  PID_StructInit(&Motor.M3508[i].OutPID,POSITION_PID,2000,0 ,0.15 ,0 ,0);//2.5 , 1
			PID_StructInit(&Motor.M3508[i].InPID,POSITION_PID,4000,3000 ,2.5f ,1.0f ,0);
		}
		
    Posture.targetZ_Angle=0;
		Chassis.Vy=0;
		Chassis.Vx=0;
		
  }
/**
  * @Data    2019-03-19 11:08
  * @brief   
  * @param   void
  * @retval  void
  */
  void Chassis_Ctrl(void)
  {
    int16_t velBuff[4];
    int16_t temp;
#if 0
		if(DR16.switch_right==3)
		{
		  Posture.targetX_Coords= -DR16.ch1 * 5;
			Posture.targetY_Coords= -DR16.ch2 * 5;
			Posture.targetZ_Angle = -DR16.ch3 / 6;

//		  Chassis.Vx=PID_Calc(&Chassis.PID_X,
//                        Posture.realX_Coords,Posture.targetX_Coords);
//      Chassis.Vy=PID_Calc(&Chassis.PID_Y,
//                        Posture.realY_Coords,Posture.targetY_Coords);
			Chassis.Vspin=PID_Calc(&Chassis.PID_Spin,
										Posture.realZ_Angle,Posture.targetZ_Angle);
			SpinRuning();
			
		}
		else
		{
			Chassis.Vx = -10*DR16.ch3;
			Chassis.Vy = -10*(DR16.ch2+DR16.ch4);
			Chassis.Vspin=5*DR16.ch1;
		}

#endif

#if 0
    Posture.targetX_Coords=DR16.ch1;
		Posture.targetY_Coords=DR16.ch2;

//		
    Chassis.Vx=PID_Calc(&Chassis.PID_X,
                        Posture.realX_Coords,Posture.targetX_Coords);
    Chassis.Vy=PID_Calc(&Chassis.PID_Y,
                        Posture.realY_Coords,Posture.targetY_Coords);
    Chassis.Vspin=PID_Calc(&Chassis.PID_Spin,
                        Posture.realZ_Angle,Posture.targetZ_Angle);
//		
#endif	
#if 0
//		if(temp<160&&DR16.switch_right==3)
//		{
//			Chassis.Vy=-1500;
//			Chassis.Vx=-Chassis.Vy*Cure[temp];
//			Chassis.Vspin=PID_Calc(&Chassis.PID_Spin,
//											 Posture.realZ_Angle,Posture.targetZ_Angle);
//		if(++temp1>3)
//		{
//			temp1=0;
//			temp++;
//		}
//		}
//		else
//		{
//			Chassis.Vx=-10*DR16.ch3;
//      Chassis.Vy=-10*(DR16.ch2+DR16.ch4);
//			Chassis.Vspin=10*DR16.ch1;
//		//	Chassis.Vspin=PID_Calc(&Chassis.PID_Spin,
//      //       Posture.realZ_Angle,Posture.targetZ_Angle);
//		}
#endif

#if 0 /*全场定位切线跑曲线*/

  if(temp<162 && DR16.switch_right == 3)/*一共取了170个点*/
	{
		//Chassis.Vy = Chassis.Vy < -1500 ? -1500 : Chassis.Vy - 10; /*起步缓慢加速*/
    Posture.targetZ_Angle=0;
		Chassis.Vy = -1000 - 500*ABS(Slope[temp]); /*直线提速，拐弯减速*/
//		Filter.p_Limit(&Chassis.Vy,-2500,-1000); /*限幅滤波*/
		if((30*(temp+1)) + Posture.realY_Coords < 5) /*每隔30mm取一个点，误差接近5mm更新*/
		{
			Chassis.Vx = -Chassis.Vy*Slope[temp]; /*计算Vx*/
			temp++;
		}
			Chassis.Vspin = PID_Calc(&Chassis.PID_Spin,
			Posture.realZ_Angle,Posture.targetZ_Angle); /*陀螺仪伺服*/
	}
	else if(DR16.switch_right == 3&&temp1<3)
	{
		switch(temp1)
		{
			case 1:
//				Posture.targetX_Coords= 0;
//				Posture.targetY_Coords= -8500;
			  Curve_Straight(1000);
			
		  	Posture.targetZ_Angle=0;
				Chassis.Vspin = PID_Calc(&Chassis.PID_Spin,
				         Posture.realZ_Angle,Posture.targetZ_Angle);
			
				Chassis.Vx=PID_Calc(&Chassis.PID_X,
                        Posture.realX_Coords,Posture.targetX_Coords);
        Chassis.Vy=PID_Calc(&Chassis.PID_Y,
                        Posture.realY_Coords,Posture.targetY_Coords);
			  
			  if(-8000-Posture.realY_Coords>-100
					&& Posture.realX_Coords-725<100)
				{
					temp1=2;
				}
				break;
			case 2:
			  Curve_Straight1(1500);
		  	Posture.targetZ_Angle=Posture.targetZ_Angle>90?90:Posture.targetZ_Angle+0.9f;

				Chassis.Vspin = PID_Calc(&Chassis.PID_Spin,
				         Posture.realZ_Angle,Posture.targetZ_Angle);
			  SpinRuning();
				if(-8500-Posture.realY_Coords>-100
					&& 2500-Posture.realX_Coords<100)
				{
					temp1=3;
				}
				break;
		}
	}
	else/*跑完点切回遥控*/
	{
		Chassis.Vx = -10*DR16.ch3;
		Chassis.Vy = -10*(DR16.ch2+DR16.ch4);
		Chassis.Vspin=5*DR16.ch1;
//			Chassis.Vspin = PID_Calc(&Chassis.PID_Spin,
//			Posture.realZ_Angle,Posture.targetZ_Angle);
	}  
#endif	

 
// if(DR16.switch_left==1)
// {
//	  GAS8_resetBits(0,GAS_POS_2);
//    GAS8_resetBits(0,GAS_POS_3);

// }
// else if(DR16.switch_left==3)
// {
//    GAS8_setBits(0,GAS_POS_3);
// }
// else
// {
//   GAS8_setBits(0,GAS_POS_2);
// }
 
 if(DR16.switch_right==1)
 {
//   GAS8_resetBits(0,GAS_POS_3);
	 
	 for(uint8_t i=0;i<3;i++)
	 {
		 Motor.M3508[i].totalAngle=0;
		 Motor.M3508[i].turnsNum=0;
		 Motor.M3508[i].targetAngle=0;
		 Motor.M3508[i].targetCurrent=0;
	 
	 }
	 Xcoord=0;
	 Ycoord=0;
	 Flag=0;
	 
	  GAS8_resetBits(0,GAS_POS_2);
		GAS8_resetBits(0,GAS_POS_3);
	 
	 	Motor.M3508[0].targetCurrent = PID_Calc(&Motor.M3508[0].InPID,
																				Motor.M3508[0].realSpeed,0);
	 
	  Motor.M3508[1].targetCurrent=-Motor.M3508[0].targetCurrent;
	 
    Motor.M3508[2].targetCurrent = PID_Calc(&Motor.M3508[2].InPID,
																				Motor.M3508[2].realSpeed,0);
	 
 }
 
 else if(DR16.switch_right==3)
 {
	  Motor.M3508[0].targetCurrent = PID_Calc(&Motor.M3508[0].InPID,
																				Motor.M3508[0].realSpeed,DR16.ch2*5);
	 
	  Motor.M3508[1].targetCurrent=-Motor.M3508[0].targetCurrent;
	 
    Motor.M3508[2].targetCurrent = PID_Calc(&Motor.M3508[2].InPID,
																				Motor.M3508[2].realSpeed,DR16.ch1*5);
 }
  else if(DR16.switch_right==2)
	{
			{
				switch(Flag)
				{
					case 0:
						if(DR16.switch_left==1||DR16.switch_left==3)
						{
							
							if(Gripper.colour != 'b')//||(Xcoord>100 && Ycoord>100))
							{
									if(Motor.M3508[2].turnsNum==-Ycoord)
									{
										if((Xcoord/10)%2==0)
										{
											Ycoord+=20;
										}
										else
										{
											Ycoord-=20;
										}
										
									}
									
//									if(Xcoord<100)
									{
								  	if(Ycoord>100||Ycoord==0)
										{
											Xcoord=Xcoord>90?0:Xcoord+30;
										}
									}

									SetCoord(Xcoord,Ycoord);
								
									temp = PID_Calc(&Motor.M3508[0].OutPID,
																											Motor.M3508[0].totalAngle,
																											Motor.M3508[0].targetAngle);
									Motor.M3508[0].targetCurrent = PID_Calc(&Motor.M3508[0].InPID,
																													Motor.M3508[0].realSpeed,
																													temp);
									Motor.M3508[1].targetCurrent=-Motor.M3508[0].targetCurrent;
									
									temp = PID_Calc(&Motor.M3508[2].OutPID,
																													Motor.M3508[2].totalAngle,
																													Motor.M3508[2].targetAngle);
									Motor.M3508[2].targetCurrent = PID_Calc(&Motor.M3508[2].InPID,
																													Motor.M3508[2].realSpeed,temp);
									
									Motor.p_M3508setCur(CAN_1);
							}	
							else 
							{
                 Flag=1;
								
							}
						
						}
						else if(DR16.switch_left==2)
						{
							if(Gripper.colour != 'y')//||(Xcoord>100 && Ycoord>100))
							{
									if(Motor.M3508[2].turnsNum==-Ycoord)
									{
										if((Xcoord/10)%2==0)
										{
											Ycoord+=20;
										}
										else
										{
											Ycoord-=20;
										}
										
									}
									
									{
								  	if(Ycoord>100||Ycoord==0)
										{
											Xcoord=Xcoord>90?0:Xcoord+30;
										}
									}

									SetCoord(Xcoord,Ycoord);
								
									temp = PID_Calc(&Motor.M3508[0].OutPID,
																											Motor.M3508[0].totalAngle,
																											Motor.M3508[0].targetAngle);
									Motor.M3508[0].targetCurrent = PID_Calc(&Motor.M3508[0].InPID,
																													Motor.M3508[0].realSpeed,
																													temp);
									Motor.M3508[1].targetCurrent=-Motor.M3508[0].targetCurrent;
									
									temp = PID_Calc(&Motor.M3508[2].OutPID,
																													Motor.M3508[2].totalAngle,
																													Motor.M3508[2].targetAngle);
									Motor.M3508[2].targetCurrent = PID_Calc(&Motor.M3508[2].InPID,
																													Motor.M3508[2].realSpeed,temp);
									
									Motor.p_M3508setCur(CAN_1);
							}	
							else 
							{
                 Flag=2;
								
							}
						
						}
					  	
						break;
					case 1:
						
					   		if(ABS(Gripper.xPID.err[0])<20)
								{
									Gripper.xPID.f_PID_Reset(&Gripper.xPID,20,0.05,0);
								}
								else if(ABS(Gripper.xPID.err[0])>20&&ABS(Gripper.xPID.err[0])<50)
								{
									Gripper.xPID.f_PID_Reset(&Gripper.xPID,20,0.01,0);
								}
								else
								{
									Gripper.xPID.f_PID_Reset(&Gripper.xPID,20,0.005,0);
								}
								
								if(ABS(Gripper.yPID.err[0])<10)
								{
									Gripper.yPID.f_PID_Reset(&Gripper.yPID,10,0.05,0);
								}
								else if(ABS(Gripper.yPID.err[0])>10&&ABS(Gripper.yPID.err[0])<20)
								{
									Gripper.yPID.f_PID_Reset(&Gripper.yPID,10,0.08,0);
								}
								else if(ABS(Gripper.yPID.err[0])>20&&ABS(Gripper.yPID.err[0])<50)
								{
									Gripper.yPID.f_PID_Reset(&Gripper.yPID,10,0.01,0);
								}
								else
								{
									Gripper.yPID.f_PID_Reset(&Gripper.yPID,10,0.005,0);
								}

								temp=PID_Calc(&Gripper.xPID,Gripper.bRealX,Gripper.TargetX);
								
								Motor.M3508[0].targetCurrent = PID_Calc(&Motor.M3508[0].InPID,
																												 Motor.M3508[0].realSpeed,temp);
								Motor.M3508[1].targetCurrent=-Motor.M3508[0].targetCurrent;
										
								temp=PID_Calc(&Gripper.yPID,Gripper.bRealY,Gripper.TargetY);
								

								Motor.M3508[2].targetCurrent = PID_Calc(&Motor.M3508[2].InPID,
																									Motor.M3508[2].realSpeed,temp);
								Motor.p_M3508setCur(CAN_1);
							
								if(ABS(Gripper.xPID.err[0])<2 && ABS(Gripper.yPID.err[2])<2)
								{
									Flag=3;
								}
						break;
					case 2:
						
						 		if(ABS(Gripper.xPID.err[0])<20)
								{
									Gripper.xPID.f_PID_Reset(&Gripper.xPID,20,0.05,0);
								}
								else if(ABS(Gripper.xPID.err[0])>20&&ABS(Gripper.xPID.err[0])<50)
								{
									Gripper.xPID.f_PID_Reset(&Gripper.xPID,20,0.01,0);
								}
								else
								{
									Gripper.xPID.f_PID_Reset(&Gripper.xPID,20,0.005,0);
								}
								
								if(ABS(Gripper.yPID.err[0])<10)
								{
									Gripper.yPID.f_PID_Reset(&Gripper.yPID,10,0.05,0);
								}
								else if(ABS(Gripper.yPID.err[0])>10&&ABS(Gripper.yPID.err[0])<20)
								{
									Gripper.yPID.f_PID_Reset(&Gripper.yPID,10,0.08,0);
								}
								else if(ABS(Gripper.yPID.err[0])>20&&ABS(Gripper.yPID.err[0])<50)
								{
									Gripper.yPID.f_PID_Reset(&Gripper.yPID,10,0.01,0);
								}
								else
								{
									Gripper.yPID.f_PID_Reset(&Gripper.yPID,10,0.005,0);
								}
								
								temp=PID_Calc(&Gripper.xPID,Gripper.yRealX,Gripper.TargetX);
								
								Motor.M3508[0].targetCurrent = PID_Calc(&Motor.M3508[0].InPID,
																												 Motor.M3508[0].realSpeed,temp);
								Motor.M3508[1].targetCurrent=-Motor.M3508[0].targetCurrent;
										
								temp=PID_Calc(&Gripper.yPID,Gripper.yRealY,Gripper.TargetY);
								

								Motor.M3508[2].targetCurrent = PID_Calc(&Motor.M3508[2].InPID,
																									Motor.M3508[2].realSpeed,temp);
								Motor.p_M3508setCur(CAN_1);
								
															
								if(ABS(Gripper.xPID.err[0])<2 && ABS(Gripper.yPID.err[2])<2)
								{
									Flag=3;
								}
					
						break;
					
					case 3:
						
					      Motor.M3508[0].targetCurrent=0;
					      Motor.M3508[1].targetCurrent=0;
					      Motor.M3508[2].targetCurrent=0;
					
	              Motor.p_M3508setCur(CAN_1);
					     vTaskDelay(200);
								GAS8_resetBits(0,GAS_POS_2);
								GAS8_resetBits(0,GAS_POS_3);
                
					      vTaskDelay(500);
							   
								GAS8_setBits(0,GAS_POS_3);
					      vTaskDelay(500);
							  GAS8_setBits(0,GAS_POS_2);
					      vTaskDelay(500);
					      GAS8_resetBits(0,GAS_POS_3);
					      vTaskDelay(500);
                
					      Flag=4;
												
						break;
					case 4:
						if(DR16.switch_left==1)
						{
							SetCoord(130,150);
							
						  if(ABS(Motor.M3508[0].turnsNum)>120 && ABS(Motor.M3508[2].turnsNum)>140)
							{
							   Flag=5;
							}
						}
						else 
						{
						  SetCoord(160,0);
						  if(ABS(Motor.M3508[0].turnsNum)==160 && ABS(Motor.M3508[2].turnsNum)==0)
							{
							   Flag=5;
							}
						}
			      		
					
								temp = PID_Calc(&Motor.M3508[0].OutPID,
																					Motor.M3508[0].totalAngle,
																					Motor.M3508[0].targetAngle);
								Motor.M3508[0].targetCurrent = PID_Calc(&Motor.M3508[0].InPID,
																							Motor.M3508[0].realSpeed,
																							temp);
								Motor.M3508[1].targetCurrent=-Motor.M3508[0].targetCurrent;

								temp = PID_Calc(&Motor.M3508[2].OutPID,
																							Motor.M3508[2].totalAngle,
																							Motor.M3508[2].targetAngle);
								Motor.M3508[2].targetCurrent = PID_Calc(&Motor.M3508[2].InPID,
																							Motor.M3508[2].realSpeed,temp);
			      		Motor.p_M3508setCur(CAN_1);

						break;
							
					case 5:
					  	GAS8_resetBits(0,GAS_POS_2);
					    SetCoord(Xcoord,Ycoord);
							temp = PID_Calc(&Motor.M3508[0].OutPID,
																				Motor.M3508[0].totalAngle,
																				Motor.M3508[0].targetAngle);
							Motor.M3508[0].targetCurrent = PID_Calc(&Motor.M3508[0].InPID,
																						Motor.M3508[0].realSpeed,
																						temp);
							Motor.M3508[1].targetCurrent=-Motor.M3508[0].targetCurrent;

							temp = PID_Calc(&Motor.M3508[2].OutPID,
																						Motor.M3508[2].totalAngle,
																						Motor.M3508[2].targetAngle);
							Motor.M3508[2].targetCurrent = PID_Calc(&Motor.M3508[2].InPID,
																						Motor.M3508[2].realSpeed,temp);
							Motor.p_M3508setCur(CAN_1);
					
					    if(ABS(Motor.M3508[0].turnsNum)==Xcoord && ABS(Motor.M3508[2].turnsNum)==Ycoord)
							{
							   Flag=0;
							}

						break;
							
					default:
						break;
				
				}
				
					

				
			
			
//			
//			Motor.M3508[0].targetCurrent = PID_Calc(&Gripper.xPID,Gripper.bRealX,Gripper.TargetX);
//		  Motor.M3508[1].targetCurrent=-Motor.M3508[0].targetCurrent;

//			Motor.M3508[2].targetCurrent = PID_Calc(&Gripper.yPID,Gripper.bRealY,Gripper.TargetY);//PID_Calc(&Motor.M3508[2].InPID,
//																				//Motor.M3508[2].realSpeed,
//																							//temp)+
			
		}
			
	}
//	else
//	{
//	  for(uint8_t i=0;i<4;i++)
//		{
//			Motor.M3508[i].targetSpeed=0;
//			Motor.M3508[i].targetCurrent=0;// PID_Calc(&Motor.M3508[i].OutPID,
////																							Motor.M3508[i].realSpeed,
////																							Motor.M3508[i].targetSpeed);
//		}
//	}
//	
	Motor.p_M3508setCur(CAN_1);
	

//    RoboModule_DRV_Velocity_Mode(0,1,3000,Speed);
//    RoboModule_DRV_Velocity_Mode(0,2,3000,velBuff[1]);
//    RoboModule_DRV_Velocity_Mode(0,3,3000,velBuff[2]);
  }

/**
* @Data    2019-03-24 12:53
* @brief   
* @param   void
* @retval  void
*/
void SpinRuning(void)
{
	static float Vx=0.0f,Vy=0.0f,Vxy=0.0f;
	static float Length=0.0f,CoordsErrX=0.0f,CoordsErrY=0.0f;

	Vx=PID_Calc(&Chassis.PID_X,
					Posture.realX_Coords,Posture.targetX_Coords);
    Vy=PID_Calc(&Chassis.PID_Y,
					Posture.realY_Coords,Posture.targetY_Coords);

	Vxy=sqrt(pow(Vx,2)+pow(Vy,2));

    CoordsErrX = Posture.targetX_Coords - Posture.realX_Coords;
    CoordsErrY = Posture.targetY_Coords - Posture.realY_Coords;
		
	Length=sqrt(pow(CoordsErrX,2) + pow(CoordsErrY,2));
	
	Chassis.Vx = ((Vxy * CoordsErrX)/Length)*cos(Posture.realZ_Angle*(PI/180))
							+((Vxy * CoordsErrY)/Length)*sin(Posture.realZ_Angle*(PI/180));

	Chassis.Vy = -((Vxy * CoordsErrX)/Length)*sin(Posture.realZ_Angle*(PI/180))
							+((Vxy * CoordsErrY)/Length)*cos(Posture.realZ_Angle*(PI/180));
}

void SetCoord(int16_t X,int16_t Y)
{
	Motor.M3508[0].targetAngle=-X*8192;
	Motor.M3508[2].targetAngle=-Y*8192;

}
/*-----------------------------------FILE OF END------------------------------*/

