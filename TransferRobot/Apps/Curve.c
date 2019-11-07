/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                     (C) Copyright 2019, Daqo Lee                             |
|                                                                              |
|                          By:GCU The wold of team                             |
|                     https://github.com/GCUWildwolfteam                       |
|------------------------------------------------------------------------------|
|  FileName    : Curve.c                                                
|  Version     : v1.0                                                           
|  Author      : Daqo Lee                                                       
|  Date        : 2019-03-03               
|  Libsupports : STM32F4xx_DFP ( 2.9.0)
|  Description :                                                       
|------------------------------declaration of end------------------------------|
 **/
/*--------------------- I N C L U D E - F I L E S ----------------------------*/

#include "Curve.h"
#include "Encoder.h"
#include <math.h>
/*-------------------------- D E F I N E S -----------------------------------*/



/*-----------L O C A L - F U N C T I O N S - P R O T O T Y P E S--------------*/



/*------------------G L O B A L - F U N C T I O N S --------------------------*/





/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-03-03 13:05
  * @brief   
  * @param   void
  * @retval  void
  */
void Curve_Straight(int16_t Len)
{
	static float xCoords=725.0f,yCoords=0.0f;
	
	yCoords=Posture.realY_Coords-\
	        sqrt(pow(Len,2)-pow(xCoords-Posture.realX_Coords,2));
	
	Posture.targetX_Coords=xCoords;
	Posture.targetY_Coords=yCoords;
	
}

  /**
  * @Data    2019-03-03 13:05
  * @brief   
  * @param   void
  * @retval  void
  */
void Curve_Straight1(int16_t Len)
{
	static float xCoords=0.0f,yCoords=-8500.0f;
	
	xCoords=Posture.realX_Coords+\
	        sqrt(pow(Len,2)-pow(yCoords-Posture.realY_Coords,2));
	
	Posture.targetX_Coords=xCoords;
	Posture.targetY_Coords=yCoords;
	
}
/*------------------------------80 Chars Limit--------------------------------*/
  /**
  * @Data    2019-03-03 13:05
  * @brief   
  * @param   void
  * @retval  void
  */
float Curve_Bezier(uint8_t *x,uint8_t n,float t)
{
  for(;n>0;n--)
	{
		for(uint8_t i=0;i<n;i++)
		{
			x[i]=((1-t)*x[i]+t*x[i+1]);
		}

	}
  return x[0];
}
  /**
  * @Data    2019-03-03 13:05
  * @brief   
  * @param   void
  * @retval  void
  */
double Curve_Sin(double A,double w,double fi,double b,double x)
{
  return (A*sin(w*x+fi*PI)+b);
}

  /**
  * @Data    2019-03-03 13:05
  * @brief   
  * @param   void
  * @retval  void
  */
double Curve_SinWalk(double A,double w,double fi,double b,double x)
{
  return (A*sin(w*(x+fi*PI))+b);
}
/*---------------------L O C A L - F U N C T I O N S--------------------------*/

#if 0
/*
f(x) =  a0 + a1*cos(x*w) + b1*sin(x*w) +
		a2*cos(2*x*w) + b2*sin(2*x*w)
Coefficients (with 95% confidence bounds):
       a0 =       35.83  (-10.58, 82.24)
       a1 =       30.72  (5.986, 55.46)
       b1 =      -10.88  (-305.9, 284.1)
       a2 =       8.177  (-20.33, 36.68)
       b2 =     -0.2359  (-141.6, 141.1)
       w =     0.01598  (-0.01716, 0.04912)


     f(x) =  a0 + a1*cos(x*w) + b1*sin(x*w) + 
               a2*cos(2*x*w) + b2*sin(2*x*w)
Coefficients (with 95% confidence bounds):
       a0 =   1.604e+10  (-2.262e+15, 2.262e+15)
       a1 =  -2.138e+10  (-3.015e+15, 3.015e+15)
       b1 =  -5.679e+08  (-6.005e+13, 6.005e+13)
       a2 =    5.34e+09  (-7.534e+14, 7.534e+14)
       b2 =   2.839e+08  (-3.002e+13, 3.002e+13)
       w =   0.0001187  (-4.182, 4.182)



	double x = (double)d;
	double a0 = 1.604e+10;
	double a1 = -2.138e+10;
	double b1 = -5.679e+08;
	double a2 = 5.34e+09;
	double b2 = 2.839e+08;
	double w = 0.0001187;
 */
double get_alpha(int d)
{
	double x = (double)d;
	double a0 = 35.83;
	double a1 = 30.72;
	double b1 = -10.88;
	double a2 = 8.177;
	double b2 = -0.2359;
	double w = 0.01598;
	double ret;

	ret = a0 + a1*cos(x*w) + b1*sin(x*w) + a2*cos(2 * x*w) + b2*sin(2 * x*w);

	
	
	return ret;
}
#endif

#if 1
/*
General model Fourier3:
f(x) =  a0 + a1*cos(x*w) + b1*sin(x*w) +
a2*cos(2*x*w) + b2*sin(2*x*w) + a3*cos(3*x*w) + b3*sin(3*x*w)
Coefficients (with 95% confidence bounds):
a0 =        27.3  (15.75, 38.85)
a1 =       -12.5  (-86.54, 61.55)
b1 =      -8.147  (-90.57, 74.27)
a2 =      -5.156  (-77.49, 67.18)
b2 =       5.955  (-75.35, 87.26)
a3 =        1.85  (-40.96, 44.66)
b3 =       2.038  (-30.99, 35.07)
w =     0.03341  (0.00882, 0.058)
 */
double get_alpha(int d)
{
 double x = (double)d;
 double a0 = 27.3;
 double a1 = -12.5;
 double b1 = -8.147;
 double a2 = -5.156;
 double b2 = 5.955;
 double a3 = 1.85;
 double b3 = 2.038;
 double w = 0.03341;
 double ret;

 ret = a0 + a1*cos(x*w) + b1*sin(x*w) + a2*cos(2 * x*w) + b2*sin(2 * x*w) + a3*cos(3 * x*w) + b3*sin(3 * x*w);
 return ret;
}
#endif

#if 0
/*
f(x) = a*x^b+c
Coefficients (with 95% confidence bounds):
a =   1.745e-11  (-4.111e-11, 7.601e-11)
b =       4.945  (4.361, 5.528)
c =       9.898  (8.357, 11.44)
*/
double get_alpha(int d)
{
	double a = 1.745e-11;
	double b = 4.945;
	double c = 9.898;

	return a*powl(d, b) + c;
}
#endif


/*-----------------------------------FILE OF END------------------------------*/



