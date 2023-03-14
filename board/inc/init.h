/**
  ******************************************************************************
  * @file    init.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл - инициализация устройств электронного блока
  ******************************************************************************
*/
#ifndef INIT_H_
#define INIT_H_

/*!
  Инициализация CAN контроллеров.  
*/
void Init_CANDevices(void);
/*!
  Инициализация дискретных входных каналов и АЦП.
  \param type_board тип платы BOARDSENSORS или BOARDCONTROLSEP  
*/
void Init_Sensors(uint8_t type_board);

/*!
  Инициализация SPI, UART0 по типу платы, инициализвция UART1.
  \param type_board тип платы MASTER или SLAVE  
*/
void Init_Interfaces(uint8_t type_board);

#include "./board/src/init.c"
#endif