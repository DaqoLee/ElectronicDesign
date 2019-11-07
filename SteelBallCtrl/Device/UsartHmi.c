#include "UsartHmi.h"

Hmi_t Hmi;


void Hmi_getMassage(uint8_t *pHmiData)
{
  if(pHmiData[0]==0xA5 && pHmiData[2]==0xA5)
	{
	  Hmi.Flag = pHmiData[1];
	}
}

