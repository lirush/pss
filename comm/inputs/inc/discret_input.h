/**
  ******************************************************************************
  * @file    discret_input.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� ������� ������ ���������� �������
  ******************************************************************************
*/
#ifndef DISCRET_INPUT_H_
#define DISCRET_INPUT_H_

#define CH_DI_num	3

extern uint8_t DI_Out[CH_DI_num];

/*!
  ������������� ���������� ������
*/
void Init_discret_input(void);
/*!
  ������������� ������ ����������
*/
void Init_discret_output(void);
/*!
  ��������� ���������� ������� ������
  \param[out] DI_Out ����� ��� �������� ��������� ����������� �����
*/
void Get_DISensors(void);

/*!
  ��������� ���������� �������� ������
  \param[out] DI_Out ����� ��� �������� ��������� ����������� ������
*/
void Get_StateOuts(void);

#include "./comm/inputs/src/discret_input.c"

#endif /* DISCRET_INPUT_H_ */