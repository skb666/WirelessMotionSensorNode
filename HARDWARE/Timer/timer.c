#include "timer.h"
#include "StepDetector.h"
#include "ADS8688.h"

float ads8688_ch1, temp_A1, temp_A2;

void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
 	TIM3->ARR=arr;  	//设定计数器自动重装值 
	TIM3->PSC=psc;  	//预分频器	  
	TIM3->DIER|=1<<0;   //允许更新中断	  
	TIM3->CR1|=0x01;    //使能定时器3
  	MY_NVIC_Init(1,0,TIM3_IRQn,2);	//抢占1，子优先级1，组2
}

void TIM5_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<3;	//TIM5时钟使能    
 	TIM5->ARR=arr;  	//设定计数器自动重装值 
	TIM5->PSC=psc;  	//预分频器	  
	TIM5->DIER|=1<<0;   //允许更新中断	  
	TIM5->CR1|=0x01;    //使能定时器5
  	MY_NVIC_Init(1,3,TIM5_IRQn,2);	//抢占3，子优先级3，组2									 
}

void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		Run_Step();	    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}

void TIM5_IRQHandler(void)
{ 		    		  			    
	if(TIM5->SR&0X0001)//溢出中断
	{
		Get_AUTO_RST_Mode_Data(ads8688_value,8);//自动扫描模式，自动扫描并转换8通道。转换数据存与Value数组中
		ads8688_ch1 = ADS8688A_showVoltage(ads8688_value[1],0x00);
		temp_A2 = ads8688_ch1*1000;
		temp_A1 = temp_A2*temp_A2*temp_A2*-0.0000000010642+temp_A2*temp_A2*-0.000005759725+temp_A2*-0.1789883+204.857;
	}
	TIM5->SR&=~(1<<0);//清除中断标志位 	    
}
