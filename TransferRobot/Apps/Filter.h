/**
|-------------------------------- Copyright -----------------------------------|
|                                                                              |
|                        (C) Copyright 2019, Daqo Lee,                         |
|                                                                              |
|                            By:GCU The wold of team                           |
|                         https://github.com/GCUWildwolfteam                   |
|------------------------------------------------------------------------------|
|--FileName    : Filter.h                                                
|--Version     : v1.0                                                            
|--Author      : Daqo Lee                                                       
|--Date        : 2019-03-03               
|--Libsupports : STM32F4xx_DFP ( 2.9.0)
|--Description :                                                       
|--------------------------------declaration of end----------------------------|
 **/
#ifndef __FILTER_H 
#define __FILTER_H 

  
/*--------------------- I N C L U D E - F I L E S ----------------------------*/



/*-------------------------G L O B A L - T Y P E S----------------------------*/

typedef struct 
{
  void (*p_Limit) (float *Value,float Max, float Mini);
  float (*p_ABS) (float Value);
  float (*p_Lowpass) (float Value, float *p_Value, float Rate); 
}Filter_t;


/*------------------------G L O B A L - M A C R O S --------------------------*/



/*----------------------G L O B A L - D E F I N E S---------------------------*/

extern Filter_t Filter;

/*-----------G L O B A L - F U N C T I O N S - P R O T O T Y P E S------------*/

void Filter_Init(void);


#endif	// __FILTER_H
/*----------------------------------FILE OF END-------------------------------*/


