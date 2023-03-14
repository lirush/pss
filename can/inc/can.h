/**
  ******************************************************************************
  * @file    can.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� ������� CAN ����������
  ******************************************************************************
*/
#ifndef __CAN_H
#define __CAN_H

/*!
  ��������� ��������� �� CAN ����, ���������� � �������
  
  \param ch ����� can ����������
*/
void vCanReceive(uint8_t ch);

//extern uint8_t MsgReceive;

#include "./can/src/can.c"
#endif