#ifndef __ADS8688_H
#define __ADS8688_H
#include "sys.h"        //间接引用"stm32f4xx.h"

#define nCS_H               GPIO_SetBits(GPIOE,GPIO_Pin_14)
#define nCS_L               GPIO_ResetBits(GPIOE,GPIO_Pin_14)
#define SCLK_H              GPIO_SetBits(GPIOE,GPIO_Pin_13)
#define SCLK_L              GPIO_ResetBits(GPIOE,GPIO_Pin_13)
#define SDO                 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)
#define SDI_H               GPIO_SetBits(GPIOD,GPIO_Pin_10)
#define SDI_L               GPIO_ResetBits(GPIOD,GPIO_Pin_10)
#define DAISY_IN_H          GPIO_SetBits(GPIOE,GPIO_Pin_12)
#define DAISY_IN_L          GPIO_ResetBits(GPIOE,GPIO_Pin_12)
#define RST_PD_H            GPIO_SetBits(GPIOD,GPIO_Pin_8)
#define RST_PD_L            GPIO_ResetBits(GPIOD,GPIO_Pin_8)



//Command Register 
#define NO_OP		0X0000//datasheet P43."Table 6. Command Register Map"
#define STDBY		0X8200//datasheet P43."Table 6. Command Register Map"
#define PWR_DN  	0X8300//datasheet P43."Table 6. Command Register Map"
#define RST	        0X8500//datasheet P43."Table 6. Command Register Map"
#define AUTO_RST	0XA000//datasheet P43."Table 6. Command Register Map"
#define MAN_Ch_0	0XC000//datasheet P43."Table 6. Command Register Map"
#define MAN_Ch_1	0XC400//datasheet P43."Table 6. Command Register Map"
#define MAN_Ch_2	0XC800//datasheet P43."Table 6. Command Register Map"
#define MAN_Ch_3	0XCC00//datasheet P43."Table 6. Command Register Map"
#define MAN_Ch_4	0XD000//datasheet P43."Table 6. Command Register Map"
#define MAN_Ch_5	0XD400//datasheet P43."Table 6. Command Register Map"
#define MAN_Ch_6	0XD800//datasheet P43."Table 6. Command Register Map"
#define MAN_Ch_7	0XDC00//datasheet P43."Table 6. Command Register Map"
#define MAN_AUX		0XE000//datasheet P43."Table 6. Command Register Map"


float ADS8688A_showVoltage(u16 value,uint8_t range);
void GPIO_Configuration(void);
//void RCC_Configuration(void);
void Delay(uint32_t nCount);
void ADS8688A_SPI_WB(uint8_t com);
uint8_t ADS8688A_SPI_RB(void);
uint8_t ADS8688A_INIT(void);
void ADS8688A_WriteCommandReg(uint16_t command);//写ADS8688命令寄存器
u16 Get_MAN_Ch_n_Mode_Data(void);
void MAN_Ch_n_Mode(uint16_t ch);//选择输入通道
void Set_CH_Range_Select(uint8_t ch,uint8_t range); //设置各个通道的范围
void Get_AUTO_RST_Mode_Data(uint16_t* outputdata, uint8_t chnum);
void ADS8688A_Write_Program_Register(uint8_t Addr,uint8_t data);
void AUTO_RST_Mode(void);

extern uint16_t ads8688_value[8];

		    
#endif







