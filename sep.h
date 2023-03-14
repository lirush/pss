/**
  ******************************************************************************
  * @file    sep.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� ������� ���. �������� �������� ������������ � �������� ��������� ������
  ******************************************************************************
*/
#ifndef __SEP_H
#define __SEP_H

/*!
  ����� ������� ����, ������������ � UART ����������
*/
enum AddrBoard{NOADDR,BOARD1,BOARD2,BOARD3,BOARD4,BOARD_BRI1,BOARD_BRI2};

/*!
  ����� ������� ���� (���� ����� � �����)
*/
enum KindClassCicle{
	CLASS_KEYS,							//���� ����� ���: ������������ ���������������� ���
	CLASS_SEPCAN_1,						//������������ ����� ������ �������� ��4-� � ��������� ���,
	CLASS_SEPCAN_2,						//���������� ������� �� ����� uart-��
	CLASS_SEPNOTCAN,
	CLASS_DEF						
	};

/*!
  ����� ������ ��� ������/�������� �� ��������� u757 (���������� �������� UART)
*/
enum KindCmd{
    SEND_TO=1,
    SEND_CONFIG
}; 

/*!
  \enum
*/
enum ListPGNCanDevice{
    PGNNULL = 0x0000,
    PGNGOMPPRESS = 0xCB30,
    PGNGOMPTEMP = 0xCB31,
    PGNGOMPTELE = 0xCB32,
    PGNGOMPFRFREQ = 0xCB33,
    PGNGOMP = 0xCB34,
    PGNGOMPTIME = 0xCB35,
    PGNPRAM = 0xCB3E,    
    PGNPKI1 = 0xCB48,
    PGNPKI2 = 0xCB49,
    PGNPKI3 = 0xCB4A,
    PGNBRI1 = 0xCB52,
    PGNEBUCTRL2 = 0xF003,
    PGNEBUSPEED = 0xF004,
    PGNDFM = 0xF605,                    //����������. ������� ������ �������
    PGNDFMCOM = 0xFD09,                    //����� ������ �������
    PGNDFMU = 0xF60B,                    //���������� ���� ����
    PGNDFMA = 0xF66E,                    //�����������
    PGNEBUDIAGN = 0xFEDF,
    PGNPMVOFFSEP = 0xCB58,
    PGNDSSEP2 = 0xCB6B,                    //DS - ���������� �������
	PGNDSSEP3 = 0xCB72
};   

#include "mRTOS.h"
#include "data_types.h"
#include "input.h"
#include "discret_input.h"
#include "uart.h"
#include "mspi.h"
#include "init.h"
#include "j1939.h"
#include "mcp.h"
#include "queue.h"
#include "can.h"
#include "u757.h"
#include "timers.h"
#include "algsep.h"

#endif