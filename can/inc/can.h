/**
  ******************************************************************************
  * @file    can.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием функций CAN интерфейса
  ******************************************************************************
*/
#ifndef __CAN_H
#define __CAN_H

/*!
  Принимает сообщения из CAN шины, укладывает в очередь
  
  \param ch номер can устройства
*/
void vCanReceive(uint8_t ch);

//extern uint8_t MsgReceive;

#include "./can/src/can.c"
#endif