#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stdlib.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "valuepack.h"
#include "timer.h"
#include "IMU.h"
#include "myiic.h"
#include "OLED_I2C.h"
#include "StepCount.h"
#include "StepDetector.h"
#include "math.h"
#include "ADS8688.h"

int main(void)
{
	u8 txt_buf[50];
	int beats=0;
	u8 x=0, k, k_flag=0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);	//延时初始化 
	uart_init(115200);	// mpu6050			0 0
	uart2_init(115200);	// stm32f1心电		1 2
	uart3_init(115200);	// 蓝牙
	uart4_init(115200);	// esp32
	LED_Init();
	KEY_Init();
	ADS8688A_INIT();
	OLED_Init();
	OLED_CLS();
	TIM3_Int_Init(100-1,8400-1);  // 计步	1 0
	TIM5_Int_Init(5000-1,8400-1); // 测温	1 3
	
	while(1){
		k = KEY_Scan(0);
		switch(k){
			case WKUP_PRES:
				k_flag = !k_flag;
				OLED_CLS();
				break;
			default:
				;
		}
		if(k_flag){
			beats=volt_diff_diff/diff_threshold*30+25+(rand()%5);
			if(beats>63)  beats=63;
			else if(beats<0)  beats=0;
			OLED_DrawWave(x,beats);//这是个画波形的函数
			x=(x+1)%128;
		}else{	
			sprintf((char *)txt_buf, "mCount :%9d", mCount);
			OLED_ShowStr(10,0,txt_buf,1);
			sprintf((char *)txt_buf, "length :%9.2f", steplength);
			OLED_ShowStr(10,2,txt_buf,1);
			sprintf((char *)txt_buf, "temp   :%9.2f", temp_A1);
			OLED_ShowStr(10,4,txt_buf,1);
			sprintf((char *)txt_buf, "hRate  :%9d", (int)(mean_bmp2+0.5));
			OLED_ShowStr(10,6,txt_buf,1);
		}
	}
}
