/**
  ******************************************************************************
  * @file    conf.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� �������, ��������, ������������ � ������� ������������ ����� ���
  ******************************************************************************
*/
#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

/*!
  \enum Role
  ����� ����� ����� � ������� ����� 
*/
typedef enum{SLAVE,MASTER}Role;
/*!
  \enum KindOf
  ����� ����� ����� � ������� ����� 
*/					
typedef enum{BOARDSENSORS,BOARDKEYS,BOARDCONTROLSEP} KindOf;			
enum {NO,YES};

/*!
  \class WhoAmI 
  ��������� ����������� ���� � ���� ����� � ������� ����� 
*/
typedef	struct{
	Role MasterSlave;
	KindOf SensorKeys;
}WhoAmI;

/*!
  \class Uart_part 
  ��������� ����������� ���������� � UART ������
*/
typedef	struct{
	uint8_t addr;					//����� � ���� uart
	//uint8_t SetConfig;				//�� ���� ����!!!!!
}Uart_part;

/*!
  \class Sender 
  ��������� ����������� ���������� � CAN ������
*/
typedef	struct{
	uint16_t BaseParamGroup;			//������� �����
	uint8_t SourceAddr;					//����� ��������� � j1939
	uint8_t	DestAddr;					//����������
}Sender;

/*!
  \class cFilter 
  \brief ��������� �������� CAN ���������
*/
typedef struct{
	uint8_t Extended;				//����������� ��� ����������� ������ ������
	uint32_t setFilter;
}cFilter;

/*!
  \class MaskAndFilter 
  \brief ��������� ����� � �������� CAN ���������
*/
typedef struct{
	uint8_t MaskOn_00_Off_60;					//��������� �����
	uint32_t Mask_[2];
	cFilter filter[6];
}MaskAndFilter;

/*!
  \class CAN_Part 
  ��������� ����� � �������� � ���������� ���������� � CAN ����
*/
typedef	struct{
	Sender CAN_sender;
	MaskAndFilter mask_filter[2];
}CAN_Part;

/*!
  \class Prog_polar_ios 
  \brief ��������� ���������� ���������� ������� ������� �����
*/
typedef struct  
{
	uint8_t polar_io_1_4;			//���������� ���������� ������ 1-4
	uint8_t polar_io_5_8;			//���������� ���������� ������ 5-8
	uint8_t polar_io_9_12;			//���������� ���������� ������ 9-12
	uint8_t polar_io_13_16;			//���������� ���������� ������ 13-16
	uint8_t polar_io_17_20;			//���������� ���������� ������ 17-20
}Prog_polar_ios;

/*!
  \class Prog_polar_ios 
  \brief ��������� ������������ ����� 
*/
typedef struct{
	WhoAmI whoAmI;
	CAN_Part can_part;
	Prog_polar_ios polar_ios;
	Uart_part uart_part;
	uint8_t SetConfigSlave;		//������ ��������� ��� ������
}ComConfig;
extern const ComConfig configurations[14];	

/*!
  ���������� ������ �����
  ������ ������ - ���� PG ��������� � �����
  ����� ������ - ���� PG � ������� 
*/
uint8_t FindModeBoard(void);

/*!
  ������������� ���������� ���������� ������� �������.
  ���������������� ���������� ������� ������� �� ���� ������
  � ������ ����� �� ������ ���������� ������
  ��� ���������������� ������������� ���������� � ������� ������������, 
  � ��������������� ������������ ��� ����������� ����� ���������� 0.
  ��� ���������������� ������������� ���������� - ���������� 1.   
*/
void Init_polar_ios(uint8_t type_board, Prog_polar_ios polar);

#include ".\board\src\conf.c"

#endif /* CONFIGURATOR_H_ */