/**
  ******************************************************************************
  * @file    j1939.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������������, �������� � ��������� ��������� J1939
  ******************************************************************************
*/
#ifndef TRADE_H_
#define TRADE_H_

/*!
    \enum KindOfMsg
    \brief ����� ����� ��������� ������������ � can ����
*/
typedef enum{AN1FROM1TO4,					//!<������ �����: � 1 �� 4 ����������
			AN1FROM5TO8,					//!<������ �����: � 5 �� 8 ����������
			DI1,							//!<������ �����: ���������� �������
			AN2FROM1TO4,					//!<������ �����: � 1 �� 4 ����������
			AN2FROM5TO8,					//!<������ �����: � 5 �� 8 ����������
			DI2								//!<������ �����: ���������� �������
}KindOfMsg;

/*!
    \class J1939_standart
    \brief ��������� ��������� J1939
*/
typedef struct{								//
	uint8_t FromWhom			:4;			//����� ���������
	uint8_t ToWhom				:4;			//����� ����������
	uint16_t ParamGroupNum		:16;
	uint8_t reserv				:2;	
	uint8_t priority			:3;	
	uint8_t zero				:3;
}J1939_standart;

/*!
    \class J1939_params
    \brief ��������� ��������� J1939
*/
typedef struct{
	uint16_t base_id;
	uint8_t addr_source;
	uint8_t addr_dest;
}J1939_params;

/*!
    ��������� � ������� ����� (�� ��������� J1939) � ������� ��� �������� � CAN ����
    
    \param ch_num ����� can �����������
    \param params ��������� ���������
    \param priority ���������
    \param kind_of_msg ��� ������������� ���������
    \param uint8_t *data ��������� �� ������������ ������
    \param size ���������� ������������ ������ ������
*/
void vJ1939(uint8_t ch_num, J1939_params params, uint8_t priority, uint8_t kind_of_msg, uint8_t *data, uint8_t size);

#include "./comm/protocol/src/j1939.c"

#endif /* TRADE_H_ */