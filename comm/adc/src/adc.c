/**
  ******************************************************************************
  * @file    adc.c
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Исходный файл содержит функции инициализации, опроса АЦП каналов
  ******************************************************************************
*/

#include <string.h>
#include "adc.h"

ADCBuf_TypeDef ADCBuf[2];                   
/*!
  Старт преобразования
*/
void adc_start(void)
{
	ADCSRA |= (1<<ADSC);
}

/*!
  Выбирает канал АЦП
*/
void adc_choice_ch(uint8_t ch)
{
	ADMUX &=~ADC_CH_MASK;
	ADMUX |=ch & ADC_CH_MASK;
}

/*!
Устанавливает в качестве опорного напряжения внутренний источник (2,56V)
*/
void adc_internal_ION(void)
{
	ADMUX |= (1<<REFS0) | (1<<REFS1);									
}
/*!
  Устанавливает в качестве опорного напряжения внешний источник (5V)
*/
void adc_avcc_ION(void)													
{
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
}

void Init_ADC(enum adc_prescaler prescaler)
{
	DDRF = 0x00;														
	ADCSRA |= (1<<ADEN);									            
	ADCSRA |= (uint8_t)prescaler;										
	adc_avcc_ION();														
	PORTF = 0x00;														
}

static uint8_t num=0;
void ADCRead(void)
{
   ADCBuf_TypeDef *ptr = ADCBuf+0; 
   int i=0; uint16_t tmp;
   static uint16_t z[8];
   if(num){num=0;}                                                          
   else {num=1; ptr++;}                                                               
   for(i=0;i<8;i++)
   {                                 
        adc_choice_ch(i);
        adc_start();   
        while(ADCSRA & (1<<ADSC));                                          
        tmp = (ADCL) | (ADCH<<8); 
        z[i] += tmp - ptr->ADC_converter[i];  
        ptr->ADC_converter[i] = z[i]>>4;          
        //ptr->ADC_converter[i] = tmp;
   }                                                            
}
               
uint8_t FindChannelADCBufNotBusy(void)
{
    uint8_t loc_num=0;
    if(num)loc_num=0;                                                   
    else loc_num=1;
    return loc_num;
}