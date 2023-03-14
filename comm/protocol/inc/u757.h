/**
  ******************************************************************************
  * @file    u757.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� ������� � ������� ����������� ��������� UART
  ******************************************************************************
*/
#ifndef __U757_H
#define __U757_H

#define LEN_HEAD_UART		(2)    //!���������� ���� ��������� ������

/*!
  \class uart_ntw_level
  ����� ������ ��������� u757
*/
typedef struct 
{
    uint8_t srcdst;                //src-��������; dst-����������
    uint8_t what_isit;             //���/������� ���������
}uart_ntw_level;


/*!
  ���������� ��������� �� ��������� u757
  
  \param uint8_t *data ��������� �� ������
  \return ����� ������ ������
*/
uart_ntw_level unpack_msg_uart(uint8_t *data);

#include "./comm/protocol/src/u757.c"

#endif