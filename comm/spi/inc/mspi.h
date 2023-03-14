/**
  ******************************************************************************
  * @file    mspi.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием классов и функций SPI интерфейса
  ******************************************************************************
*/
#ifndef __SPI_H
#define __SPI_H

#define DD_SCK 1
#define DD_MOSI 2
#define SPI_BUFF_SIZE	50		/*!глубина буфера данных*/

/*!
   \class selector
   Переключатель устройств SPI интерфейса
*/
typedef struct{
	volatile uint8_t *port;
	uint8_t b;
}selector;
selector Sel_impl[2];

extern uint8_t SPI_data[SPI_BUFF_SIZE];		/*!буфер принимаемых данных*/
extern volatile uint8_t SPI_busy;

/*!
   \enum DIV
   Набор делителей частоты SPI
*/
enum DIV {OSC_DIV_2=1, OSC_DIV_4,OSC_DIV_8,OSC_DIV_16,OSC_DIV_32,OSC_DIV_64,OSC_DIV_128};

/*!
   Инициализация SPI мастера
*/
void Init_SPIMaster(enum DIV);

/*!
   Обмен по SPI
   
   \param *data_in_out указатель на данные
   \param size_data количество байт данных
   \return 0
*/
uint8_t uSPIWrite(uint8_t *data_in_out, uint8_t size_data);
/*!
   Обмен по SPI
   Разом передает все количество байт. Пока не передаст не выйдет
   
   \param[in] *data указатель на данные
   \param[in] size_data количество байт данных
   \param[out] *data указатель на данные
   \return 0 
*/
uint8_t uSPIReadWrite(uint8_t *data, uint8_t size_data);

/*!
   Включает/выключает устройство для обмена по SPI интерфейсу
   Вызывает функцию обмена
   Разом передает все количество байт. Пока не передаст не выйдет
   Используется исключительно для записи данных в устройство SPI
   
   \param ch_num номер канала устройства
   \param *data указатель на данные
   \param size количество байт данных 
*/
void exchange(uint8_t ch_num,uint8_t *data,uint8_t size);
/*!
   Включает/выключает устройство для обмена по SPI интерфейсу
   Вызывает функцию обмена
   Используется для чтения/записи данных устройства SPI
   
   \param ch_num номер канала устройства
   \param *data указатель на данные
   \param size количество байт данных 
*/
void exchange_read(uint8_t ch_num,uint8_t *data,uint8_t size);

/*!
    Флаг занятости SPI устройства 
*/
void SPIBusy(void);

/*!
    Кладет по одному байту в SPI интерфес для передачи/приема
    Задумывалось как функция безочередного обмена по SPI. 
*/
void putspi(void);

#include "./comm/spi/src/mspi.c"

#endif