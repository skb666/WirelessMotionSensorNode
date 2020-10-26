#include "ADS8688.h"

uint16_t ads8688_value[8];

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ֵת���ɵ�ѹ
//  @param      value       ���ֵ
//  @param      range       ͨ���ķ�Χ
//  @return     float       ��ȷ��ѹ����λ��V��
//  Sample usage:           ADS8688A_showVoltage(value,0x05);��Χ��+2.5*ref
//-------------------------------------------------------------------------------------------------------------------
float ADS8688A_showVoltage(u16 value,uint8_t range)
{
	if(range == 0x00) return 2.5*4.096/32768*(value-32768);
	if(range == 0X01) return 1.25*4.096/32768*(value-32768);
	if(range == 0X02) return 0.625*4.096/32768*(value-32768);
	if(range == 0X03) return 0.3125*4.096/32768*(value-32768);
	if(range == 0X0B) return 0.15625*4.096/32768*(value-32768);
	if(range == 0X05) return 2.5*4.096/65536*value;
	if(range == 0X06) return 1.25*4.096/65536*value;
	if(range == 0X07) return 0.625*4.096/65536*value;
	if(range == 0X0F) return 0.3125*4.096/65536*value;
}


void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /***********************************************************************///ADC INTERFACE
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//SDO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void AUTO_RST_Mode(void)//�����Զ�ɨ��ģʽ
{
  ADS8688A_WriteCommandReg(AUTO_RST);
}


void Get_AUTO_RST_Mode_Data(uint16_t* outputdata, uint8_t chnum)
{
  //��ȡɨ��ͨ�����е�ADת������code������������
  u8 i=0,datal=0,datah=0;
  u16 data=0;
  for (i=0; i<chnum; i++)
  {
    nCS_L;
    ADS8688A_SPI_WB(0X00);
    ADS8688A_SPI_WB(0X00);
    datah = ADS8688A_SPI_RB();;
    datal = ADS8688A_SPI_RB();;
    nCS_H;
    data = datah<<8 | datal;	//��λ��ǰ����λ�ں�
    *(outputdata+i) = data;
  }
}

uint16_t Get_MAN_Ch_n_Mode_Data(void)
{
  u8 datah=0,datal=0;
  nCS_L;
  ADS8688A_SPI_WB(0X00);
  ADS8688A_SPI_WB(0X00);
  datah = ADS8688A_SPI_RB();
  datal = ADS8688A_SPI_RB();
  nCS_H;
  return (datah<<8 | datal);
}

void MAN_Ch_n_Mode(uint16_t ch)//�ֶ�ģʽ
{
  ADS8688A_WriteCommandReg(ch);	
}


void ADS8688A_Write_Program_Register(uint8_t Addr,uint8_t data)
{
  nCS_L;
  ADS8688A_SPI_WB(Addr<<1| 0X01);
  ADS8688A_SPI_WB(data);
  nCS_H;
}

void Set_CH_Range_Select(uint8_t ch,uint8_t range) //���ø���ͨ���ķ�Χ
{
  ADS8688A_Write_Program_Register(ch,range);
}



u8 ADS8688A_READ_Program_Register(uint8_t Addr)
{
  u8 data = 0;
  nCS_L;
  ADS8688A_SPI_WB(Addr<<1);
  data = ADS8688A_SPI_RB();
  data = ADS8688A_SPI_RB();
  nCS_H;
  return data;
}

void Enter_RESET_MODE(void)//�����λģʽ����λ program registers
{
  ADS8688A_WriteCommandReg(RST);
}

void ADS8688A_WriteCommandReg(uint16_t command)//дADS8688����Ĵ���
{
  nCS_L;
  ADS8688A_SPI_WB(command>>8 & 0XFF);
  ADS8688A_SPI_WB(command & 0XFF);
  nCS_H;
}

//uint8_t ADS8688A_INIT(void)
//{ 
//  uint8_t i;
//  Enter_RESET_MODE();
//  ADS8688A_Write_Program_Register(0X01,0XFF);
//  i = ADS8688A_READ_Program_Register(0X01);
//  return i;
//}


uint8_t ADS8688A_SPI_RB(void)
{
  uint8_t Rdata=0;
  //nCS_L;
  for(uint8_t s=0;s<8;s++)
  {
    Rdata<<=1;
    SCLK_H;
    if(SDO)
    {
      Rdata|=0x01;
    }
    else
    {
      Rdata&=0xFE;
    }
    SCLK_L;
  }
  return Rdata;
}


void ADS8688A_SPI_WB(uint8_t com)
{
  uint8_t com_temp=com;
  nCS_L;
  for(uint8_t s=0;s<8;s++)	
  {
    if(com_temp&0x80)
    {
      SDI_H;
    }
    else
    {
      SDI_L;
    }
    SCLK_H;
    com_temp<<=1;
    SCLK_L;
  }
}


void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
uint8_t ADS8688A_INIT(void)
{ 
    uint8_t i;
    GPIO_Configuration();
    RST_PD_H;
    DAISY_IN_L;
    Delay(0x1fff);
    Enter_RESET_MODE();
    ADS8688A_Write_Program_Register(0X01,0XFF);
    i = ADS8688A_READ_Program_Register(0X01);

    Delay(0x1fff);
    ADS8688A_Write_Program_Register(0x02,0x00);//����ͨ���˳��͹���״̬
    ADS8688A_Write_Program_Register(0x01,0xff);//ʹ������ͨ��

    Set_CH_Range_Select(0X05,0X00);//����ͨ��0�����뷶Χ��0.625*Vref
    Set_CH_Range_Select(0X06,0X00);//����ͨ��1�����뷶Χ��0.625*Vref  
    Set_CH_Range_Select(0X07,0X00);//����ͨ��2�����뷶Χ��0.625*Vref
    Set_CH_Range_Select(0X08,0X00);//����ͨ��3�����뷶Χ��0.625*Vref
    Set_CH_Range_Select(0X09,0X00);//����ͨ��4�����뷶Χ��0.625*Vref
    Set_CH_Range_Select(0X0a,0X00);//����ͨ��5�����뷶Χ��0.625*Vref
    Set_CH_Range_Select(0X0b,0X00);//����ͨ��6�����뷶Χ��0.625*Vref
    Set_CH_Range_Select(0X0c,0X00);//����ͨ��7�����뷶Χ��0.625*Vref

    //0x00 -> +-2.5*ref
    //0x01 -> +-1.25*ref
    //0x02 -> +-0.625*ref
    //0x03 -> +-0.3125*ref
    //0x0B -> +-0.15625*ref
    //0x05 -> +2.5*ref
    //0x06 -> +1.25*ref
    //0x07 -> +0.625*ref
    //0x0F -> +0.3125*ref
    
    AUTO_RST_Mode();//�����Զ�ɨ��ģʽ
  //  MAN_Ch_n_Mode(MAN_Ch_1);//�ֶ�ģʽ
//    MAN_Ch_n_Mode(MAN_AUX);//��ȡ����ͨ������
    return i;
}








