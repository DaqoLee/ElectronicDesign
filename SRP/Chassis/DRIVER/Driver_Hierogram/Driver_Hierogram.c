#include "Driver_Hierogram.h"
#include "Ctrl_Rammer.h"
uint8_t Hierogram[20],Hierogram_flag=0;
uint16_t Hierogram_Yaw[3][3],Hierogram_Pitch[3][3];
int16_t Hierogram_Y=300,Hierogram_X=400;
pid_t Auto_Yaw_PID,Auto_Pitch_PID;
FrameRate_Struct PC_Data;
float Target_Auto_Yaw,Target_Auto_Pitch;
 /*
  * @brief 神符打击函数，手动取3点确定九宫格位置
  * @param None
  * @retval None
  */
void Hierogram_Get()//神符
{
	static uint8_t flag1=1,flag2=6,flag3=0,flag4=0,i=0;
	
	

	if(Hierogram_flag<3)
	{
		flag2=6;
  
		Cloud_Target_Angle(DBUS_ReceiveData.mouse.y,DBUS_ReceiveData.mouse.x);


		if(DBUS_ReceiveData.mouse.press_right)
		{
			if(flag1&&Hierogram_flag<3)
			{
				flag1=0;
				for(i=0;i<3;i++)
				{
					Hierogram_Pitch[Hierogram_flag][i]=CloudParam.Pitch.Target_Angle;
					Hierogram_Yaw[i][Hierogram_flag]=CloudParam.Yaw.Target_Angle;
				}
				Hierogram_flag++;
			}
			if(Hierogram_flag==3)
			{
				Cloud_Target_Angle(Hierogram_Pitch[1][1],Hierogram_Yaw[1][1]);
				flag2=0;
			}
		}
		else
		{
			flag1=1;
		}
	}
	else if(flag2<6)
	{
//		switch(Hierogram[0])
//		{
//			case '1':
//				Cloud_Target_Angle(Hierogram_Pitch[0][0],Hierogram_Yaw[0][0]);			
//				HAL_UART_Transmit_DMA(&huart6,&Hierogram[0],1);
//				flag3=1;
//				flag2++; 
//				break;
//			case '2':
//				Cloud_Target_Angle(Hierogram_Pitch[0][1],Hierogram_Yaw[0][1]);
//				HAL_UART_Transmit_DMA(&huart6,&Hierogram[0],1);
//				flag3=1;
//				flag2++;
//				break;
//			case '3':
//				Cloud_Target_Angle(Hierogram_Pitch[0][2],Hierogram_Yaw[0][2]);
//				HAL_UART_Transmit_DMA(&huart6,&Hierogram[0],1);
//				flag3=1;
//				flag2++;
//				break;
//			case '4':
//				Cloud_Target_Angle(Hierogram_Pitch[1][0],Hierogram_Yaw[1][0]);
//				HAL_UART_Transmit_DMA(&huart6,&Hierogram[0],1);
//				flag3=1;
//				flag2++;
//				break;
//			case '5':
//				Cloud_Target_Angle(Hierogram_Pitch[1][1],Hierogram_Yaw[1][1]);
//				HAL_UART_Transmit_DMA(&huart6,&Hierogram[0],1);
//				flag3=1;
//				flag2++;
//				break;
//			case '6':
//				Cloud_Target_Angle(Hierogram_Pitch[1][2],Hierogram_Yaw[1][2]);
//				HAL_UART_Transmit_DMA(&huart6,&Hierogram[0],1);
//				flag3=1;
//				flag2++;
//				break;
//			case '7':
//				Cloud_Target_Angle(Hierogram_Pitch[2][0],Hierogram_Yaw[2][0]);
//				HAL_UART_Transmit_DMA(&huart6,&Hierogram[0],1);
//				flag3=1;
//				flag2++;
//				break;
//			case '8':
//				Cloud_Target_Angle(Hierogram_Pitch[2][1],Hierogram_Yaw[2][1]);
//				HAL_UART_Transmit_DMA(&huart6,&Hierogram[0],1);
//				flag3=1;
//				flag2++;
//				break;
//			case '9':
//				Cloud_Target_Angle(Hierogram_Pitch[2][2],Hierogram_Yaw[2][2]);
//				HAL_UART_Transmit_DMA(&huart6,&Hierogram[0],1);
//				flag3=1;
//				flag2++;
//				break;
//			
//			default:
//				break;
//			
//		}

//		Hierogram[0]='0';



	switch(DBUS_ReceiveData.keyBoard.key_code)
		{
			case KEY_Q:
				Cloud_Target_Angle(Hierogram_Pitch[0][0],Hierogram_Yaw[0][0]);			
				flag3=1;
				break;
			case KEY_W:
				Cloud_Target_Angle(Hierogram_Pitch[0][1],Hierogram_Yaw[0][1]);
				flag3=1;
				break;
			case KEY_E:
				Cloud_Target_Angle(Hierogram_Pitch[0][2],Hierogram_Yaw[0][2]);
				flag3=1;
				break;
			case KEY_A:
				Cloud_Target_Angle(Hierogram_Pitch[1][0],Hierogram_Yaw[1][0]);
				flag3=1;
				break;
			case KEY_S:
				Cloud_Target_Angle(Hierogram_Pitch[1][1],Hierogram_Yaw[1][1]);
				flag3=1;
				break;
			case KEY_D:
				Cloud_Target_Angle(Hierogram_Pitch[1][2],Hierogram_Yaw[1][2]);
				flag3=1;
				break;
			case KEY_Z:
				Cloud_Target_Angle(Hierogram_Pitch[2][0],Hierogram_Yaw[2][0]);
				flag3=1;
				break;
			case KEY_X:
				Cloud_Target_Angle(Hierogram_Pitch[2][1],Hierogram_Yaw[2][1]);
				flag3=1;
				break;
			case KEY_C:
				Cloud_Target_Angle(Hierogram_Pitch[2][2],Hierogram_Yaw[2][2]);
				flag3=1;
				break;
			
			default:
				break;
			
		}
	}
	if(flag3==1)
		{
			if(flag4++>=120)
			{
				flag3=0;
				flag4=0;
				Hierogram_Shoot=1;
				flag2++;
			}
		}
		else
		{
			Hierogram_Shoot=0;
		}
	Hierogram[0]='0';
		if(flag2==6&&DBUS_CheckPush(KEY_S))
		{
			flag2=0;
		}
}

 /*
  * @brief 神符打击函数，手动取3点确定九宫格位置
  * @param None
  * @retval None
  */

void PC_Data_Analysis()
{
	for(uint16_t n = 0; n < 20; ){
		//匹配帧头
		if(Hierogram[n]=='S'&&Hierogram[n+7]=='E' ){
			PC_Data.FrameRate++;
			Hierogram_X=(Hierogram[n+1]-'0')*100+(Hierogram[n+2]-'0')*10+(Hierogram[n+3]-'0')-400;
			Hierogram_Y=(Hierogram[n+4]-'0')*100+(Hierogram[n+5]-'0')*10+(Hierogram[n+6]-'0')-300;
			Target_Auto_Yaw=Hierogram_X+CloudParam.Yaw.Real_Angle;
	    Target_Auto_Pitch=CloudParam.Pitch.Real_Angle+Hierogram_Y;
			return;
		}
		else{
			n++;
		}
	}
	
	
	
//	if(Hierogram[0]=='S'&&Hierogram[7]=='E')
//	{
//		PC_Data.FrameRate++;
//		Hierogram_X=(Hierogram[1]-'0')*100+(Hierogram[2]-'0')*10+(Hierogram[3]-'0');
//		Hierogram_Y=(Hierogram[4]-'0')*100+(Hierogram[5]-'0')*10+(Hierogram[6]-'0');
//	}


}

