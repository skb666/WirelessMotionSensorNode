#include "IMU.h"	

//陀螺仪相关变量
unsigned char Re_buf[100],counter;
unsigned char ucStra[6],ucStrw[6],ucStrAngle[6];
float Angle[3];
float AccX,AccY,AccZ,X,Y,Z;
  	
u8 IMU_BUF[200];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 IMU_STA=0;       //接收状态标记	

void USART1_IRQHandler(void)                	//陀螺仪串口1中断服务程序
{
	char ch;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		ch=USART_ReceiveData(USART1);
		Re_buf[counter]=ch;
		
		if(Re_buf[0]!=0x55) 
		{
			counter=0;
			return;      //第零号数据不是帧头
		}
				
		counter++;
		
		if(counter==11)             
		{
			counter=0; 
			switch(Re_buf [1])
			{
				case 0x51:
				ucStra[0]=Re_buf[2];
				ucStra[1]=Re_buf[3];
				ucStra[2]=Re_buf[4];
				ucStra[3]=Re_buf[5];
				ucStra[4]=Re_buf[6];
				ucStra[5]=Re_buf[7];
				break;
				case 0x52:	 
				ucStrw[0]=Re_buf[2];
				ucStrw[1]=Re_buf[3];
				ucStrw[2]=Re_buf[4];
				ucStrw[3]=Re_buf[5];
				ucStrw[4]=Re_buf[6];
				ucStrw[5]=Re_buf[7];
				break;
				case 0x53: 
				ucStrAngle[0]=Re_buf[2];
				ucStrAngle[1]=Re_buf[3];
				ucStrAngle[2]=Re_buf[4];
				ucStrAngle[3]=Re_buf[5];
				ucStrAngle[4]=Re_buf[6];
				ucStrAngle[5]=Re_buf[7];	
				break;
				default:break;
			}
		}
		Get_IMU_data();
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
} 

void Get_IMU_data(void)//获取陀螺仪数据
{
  Angle[0] = ((ucStrAngle[1]<<8| ucStrAngle[0]))/32768.0*180.0;
  Angle[1] = ((ucStrAngle[3]<<8| ucStrAngle[2]))/32768.0*180.0;
  Angle[2] = ((ucStrAngle[5]<<8| ucStrAngle[4]))/32768.0*180.0;
	
	AccX=(((short)((short)ucStra[1]<<8)|ucStra[0])/32768.000)*16*9.8;
	AccY=(((short)((short)ucStra[3]<<8)|ucStra[2])/32768.000)*16*9.8;
	AccZ=(((short)((short)ucStra[5]<<8)|ucStra[4])/32768.000)*16*9.8;
	if((Angle[0]>=0)&&(Angle[0]<=180))
		X=Angle[0]+180;
	else 
		X=Angle[0]-180;
	
	if((Angle[1]>=0)&&(Angle[1]<=180))
		Y=Angle[1]+178;
	else 
		Y=Angle[1]-182;
	
		Z=Angle[2];
}
