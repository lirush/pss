/**
  ******************************************************************************
  * @file    adc.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием классов, перечислений и функций 
  ******************************************************************************
*/
#ifndef _ADC_H
#define _ADC_H

#define ADC_CH_MASK				(0x07)						/*!маска каналов АЦП, биты MUX4:0 регистра ADMUX*/
#define CH_ADC_num				(8)							/*!количество каналов АЦП*/                        

/*!
    \enum adc_prescaler
    Набор возможных делителей частоты АЦП
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
    \brief Класс для работы с буфером АЦП
  
    Буфер содержит двойное дно.
    Поле st можно использовать как команду: 1- занят; 2-готов к чтению
*/
typedef struct{
    uint8_t st;                                              //1-чтение; 2-буфер полон и готов к чтению
    uint16_t ADC_converter[CH_ADC_num];                      //буфер данных АЦП куда записываются вновь считанные данные 
}ADCBuf_TypeDef;
extern ADCBuf_TypeDef ADCBuf[2];

/*!
Устанавливает параметры работы АЦП
\param prescaler делитель работы АЦП
*/
void Init_ADC(enum adc_prescaler prescaler);
/*!
    \brief Опрашивает АЦП каналы. Сохраняет в буфер данных ADCBuf
    
    Последовательно опрашивает каждый канал АЦП
*/
void ADCRead(void);
/*!
Ищет свободный номер канала буфера, откуда можно брать данные 
*/
uint8_t FindChannelADCBufNotBusy(void);

#include "./comm/adc/src/adc.c"

#endif