/**
  ******************************************************************************
  * @file    discret_input.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием функций опроса дискретных каналов
  ******************************************************************************
*/
#ifndef DISCRET_INPUT_H_
#define DISCRET_INPUT_H_

#define CH_DI_num	3

extern uint8_t DI_Out[CH_DI_num];

/*!
  Инициализация дискретных входов
*/
void Init_discret_input(void);
/*!
  Инициализация ключей управления
*/
void Init_discret_output(void);
/*!
  Сканирует дискретные входные каналы
  \param[out] DI_Out буфер для хранения состояния дискретного входа
*/
void Get_DISensors(void);

/*!
  Сканирует дискретные выходные каналы
  \param[out] DI_Out буфер для хранения состояния дискретного выхода
*/
void Get_StateOuts(void);

#include "./comm/inputs/src/discret_input.c"

#endif /* DISCRET_INPUT_H_ */