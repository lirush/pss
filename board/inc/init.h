/**
  ******************************************************************************
  * @file    init.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� - ������������� ��������� ������������ �����
  ******************************************************************************
*/
#ifndef INIT_H_
#define INIT_H_

/*!
  ������������� CAN ������������.  
*/
void Init_CANDevices(void);
/*!
  ������������� ���������� ������� ������� � ���.
  \param type_board ��� ����� BOARDSENSORS ��� BOARDCONTROLSEP  
*/
void Init_Sensors(uint8_t type_board);

/*!
  ������������� SPI, UART0 �� ���� �����, ������������� UART1.
  \param type_board ��� ����� MASTER ��� SLAVE  
*/
void Init_Interfaces(uint8_t type_board);

#include "./board/src/init.c"
#endif