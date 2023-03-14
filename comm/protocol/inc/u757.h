/**
  ******************************************************************************
  * @file    u757.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием классов и функций внутреннего протокола UART
  ******************************************************************************
*/
#ifndef __U757_H
#define __U757_H

#define LEN_HEAD_UART		(2)    //!количество байт заголовка пакета

/*!
  \class uart_ntw_level
  Фрейм пакета протокола u757
*/
typedef struct 
{
    uint8_t srcdst;                //src-источник; dst-получатель
    uint8_t what_isit;             //тип/команда сообщения
}uart_ntw_level;


/*!
  Распаковка сообщения по протоколу u757
  
  \param uint8_t *data указатель на данные
  \return фрейм пакета данных
*/
uart_ntw_level unpack_msg_uart(uint8_t *data);

#include "./comm/protocol/src/u757.c"

#endif