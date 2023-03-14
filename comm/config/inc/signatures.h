/**
  ******************************************************************************
  * @file    signatures.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� ������������ � ������� ��������� ������ ���
  ******************************************************************************
*/
#ifndef SIGNATURES_H_
#define SIGNATURES_H_

#include "data_types.h"

/*!
    \enum config
    ����� ��������� ������������
*/
enum config{
	CONFIG0 = 0xFDB7,
	CONFIG1 = 0xFCB7,
	CONFIG2 = 0xFD33,
	CONFIG3 = 0xFC33,
	CONFIG4 = 0xF997,
	CONFIG5 = 0xFDB6,
	CONFIG6 = 0xF996,
	CONFIG7 = 0xFDA7,
	CONFIG8 = 0xF5B5,
	CONFIG9 = 0xF5A5,
	CONFIG10 = 0xF887,
	CONFIG11 = 0xFD22,
	CONFIG12 = 0xF4B4,
	CONFIG13 = 0xF111
};

/*!
  ���������� ���� G ��� ������������ ����� � �����
*/
void Init_port_combination(void);
/*!
  ������ ������ ������������
  \param[out] ����� ������������
  \param[in] num ��������� �����
*/
uint8_t GetIndex(uint16_t num);
/*!
  ������ ��������� ��� ������������ ����� 
  
  ��������� ����� ���� ������� ����� �����, � ������������ � ���� �������� ��������� 
*/
int GetSignature(void);

#include ".\comm\config\src\signatures.c"

#endif /* SIGNATURES_H_ */