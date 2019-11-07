/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : Curve.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-03-03               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|---------------------------------declaration of end----------------------------|
 **/
#ifndef __CURVE_H 
#define __CURVE_H 

  
/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "stm32f4xx.h"

/*-------------------------G L O B A L - T Y P E S----------------------------*/



/*------------------------G L O B A L - M A C R O S --------------------------*/

#define PI 3.141592653589

/*----------------------G L O B A L - D E F I N E S---------------------------*/



/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/

void Curve_Straight(int16_t Len);
void Curve_Straight1(int16_t Len);
double Curve_Sin(double A,double w,double fi,double b,double x);
double Curve_SinWalk(double A,double w,double fi,double b,double x);
double get_alpha(int d);

#endif	// __CURVE_H
/*----------------------------------FILE OF END-------------------------------*/


 

