/**
  ******************************************************************************
  * @file    init.c
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   �������� ���� - ������������� ��������� ������������ �����
  ******************************************************************************
*/
#include "init.h"
#include "mcp.h"

/*!
  ������������� ������� ���������� -INT4, INT5.
  ��� ���������� ������������ ������ ������ �� can ����.
  ������������ �� ������� ������.    
*/
void ExternalInt_Init(void)
{
    DDRE &= ~((1<<PORTE4) | (1<<PORTE5));                                    //���� E INT4,5 - ���������� ��� �����
    PORTE |= (1<<PORTE4) | (1<<PORTE5);                                    //�����������
    EIFR |= (1<<INT4) | (1<<INT5);            //����� ����������
    EICRB &= ~(1<<ISC41);                                            //���������� �� ������� ������
    EICRB &= ~(1<<ISC51);
//    EICRB |= //(1<<ISC41) |                                            //���������� ������������ �� ��������� ������ ��� INT6,7 {� �� ������� ��� INT4,5}
//            //(1<<ISC51) |;        
    EIMSK |= (1<<INT4) | (1<<INT5);            //���������� ����������
}

void Init_CANDevices(void)
{
    CanEx[CAN1].inter_rupt.inter_rupt.RX0_en = 0;
    CanEx[CAN2].inter_rupt.inter_rupt.RX0_en = 0;
    CanEx[CAN1].inter_rupt.inter_rupt.RX1_en = 0;        //��������� ���������� �� ���������� ������ ������
    CanEx[CAN2].inter_rupt.inter_rupt.RX1_en = 0;        //��������� ���������� �� ���������� ������ ������    
    //ExternalInt_Init();                                //������������� ������� ��������� INT6,7? ������������ ��� ������������ ������/�������� �� SPI    
    Init_CAN(CAN1);                                    //�����; �����, ����������� �������; �������� - 125 ����/�
	Init_CAN(CAN2);
}

void Init_Sensors(uint8_t type_board)
{
	if(type_board==BOARDCONTROLSEP)
	Init_discret_output();
	else {
		Init_discret_input();
	}
}

void Init_Interfaces(uint8_t type_board)
{
    if(type_board==MASTER)
        Init_SPIMaster(OSC_DIV_8); 
    Init_USART(U1,57600);
//    if(type_board==SLAVE)         
//        Init_USART(U0,57600);
}