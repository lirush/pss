/**
  ******************************************************************************
  * @file    adc.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� �������, ������������ � ������� 
  ******************************************************************************
*/
#ifndef _ADC_H
#define _ADC_H

#define ADC_CH_MASK				(0x07)						/*!����� ������� ���, ���� MUX4:0 �������� ADMUX*/
#define CH_ADC_num				(8)							/*!���������� ������� ���*/                        

/*!
    \enum adc_prescaler
    ����� ��������� ��������� ������� ���
*/
enum adc_prescaler{
	ADC_DIV2 = (0x1 << ADPS0),
	ADC_DIV4 = (0x2 << ADPS0),
	ADC_DIV8 = (0x3 << ADPS0),
	ADC_DIV16 = (0x4 << ADPS0),
	ADC_DIV32 = (0x5 << ADPS0),
	ADC_DIV64 = (0x6 << ADPS0),
	ADC_DIV128 = (0x7 << ADPS0),
};

/*!
    \class ADCBuf_TypeDef
    \brief ����� ��� ������ � ������� ���
  
    ����� �������� ������� ���.
    ���� st ����� ������������ ��� �������: 1- �����; 2-����� � ������
*/
typedef struct{
    uint8_t st;                                              //1-������; 2-����� ����� � ����� � ������
    uint16_t ADC_converter[CH_ADC_num];                      //����� ������ ��� ���� ������������ ����� ��������� ������ 
}ADCBuf_TypeDef;
extern ADCBuf_TypeDef ADCBuf[2];

/*!
������������� ��������� ������ ���
\param prescaler �������� ������ ���
*/
void Init_ADC(enum adc_prescaler prescaler);
/*!
    \brief ���������� ��� ������. ��������� � ����� ������ ADCBuf
    
    ��������������� ���������� ������ ����� ���
*/
void ADCRead(void);
/*!
���� ��������� ����� ������ ������, ������ ����� ����� ������ 
*/
uint8_t FindChannelADCBufNotBusy(void);

#include "./comm/adc/src/adc.c"

#endif