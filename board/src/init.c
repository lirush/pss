/**
  ******************************************************************************
  * @file    init.c
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Исходный файл - инициализация устройств электронного блока
  ******************************************************************************
*/
#include "init.h"
#include "mcp.h"

/*!
  Инициализация внешних прерываний -INT4, INT5.
  Эти прерывания используются приема данных из can шины.
  Генерируются по заднему фронту.    
*/
void ExternalInt_Init(void)
{
    DDRE &= ~((1<<PORTE4) | (1<<PORTE5));                                    //порт E INT4,5 - используем как входы
    PORTE |= (1<<PORTE4) | (1<<PORTE5);                                    //подтягиваем
    EIFR |= (1<<INT4) | (1<<INT5);            //сброс прерываний
    EICRB &= ~(1<<ISC41);                                            //прерывания по низкому уровню
    EICRB &= ~(1<<ISC51);
//    EICRB |= //(1<<ISC41) |                                            //прерывания генерируются по переднему фронту для INT6,7 {и по заднему для INT4,5}
//            //(1<<ISC51) |;        
    EIMSK |= (1<<INT4) | (1<<INT5);            //разрешение прерываний
}

void Init_CANDevices(void)
{
    CanEx[CAN1].inter_rupt.inter_rupt.RX0_en = 0;
    CanEx[CAN2].inter_rupt.inter_rupt.RX0_en = 0;
    CanEx[CAN1].inter_rupt.inter_rupt.RX1_en = 0;        //разрешаем прерывание по заполнению буфера приема
    CanEx[CAN2].inter_rupt.inter_rupt.RX1_en = 0;        //разрешаем прерывания по заполнению буфера приема    
    //ExternalInt_Init();                                //инициализация внешних перываний INT6,7? используемых для отслеживания приема/передачи по SPI    
    Init_CAN(CAN1);                                    //сброс; маски, настраиваем фильтры; скорость - 125 кбит/с
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