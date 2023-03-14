/**
  ******************************************************************************
  * @file    def.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� ��� ������ � ������������ mcp2515
  ******************************************************************************
*/
#ifndef __DEF_H
#define __DEF_H

#define CANCTRL	0x0F	//!CANCTRL - CAN CONTROL REGISTER address
#define CANSTAT	0x0E	//!CANSTAT – CAN STATUS REGISTER address
#define TXRTSCTRL 0x0D	//TXRTSCTRL - TXnRTS PIN CONTROL AND STATUS REGISTER address
#define BFPCTRL 0x0C	//BFPCTRL – RXnBF PIN CONTROL AND STATUS address
#define TEC 0x1C	//TEC – TRANSMIT ERROR COUNTER address
#define REC 0x1D	//REC – RECEIVER ERROR COUNTER address

//control the bit timing for the CAN bus interface.
#define CNF1 0x2A	//CNF1 - CONFIGURATION 1
#define CNF2 0x29	//CNF2 - CONFIGURATION 1
#define CNF3 0x28	//CNF3 - CONFIGURATION 1

#define CANINTE	0x2B	//CANINTE – INTERRUPT ENABLE
#define CANINTF	0x2C	//CANINTF – INTERRUPT FLAG
#define EFLG 0x2D		//EFLG – ERROR FLAG
#define TXB0CTRL 0x30	//TRANSMIT BUFFER 0 CONTROL REGISTER
#define TXB1CTRL 0x40	//TRANSMIT BUFFER 1 CONTROL REGISTER
#define TXB2CTRL 0x50	//TRANSMIT BUFFER 2 CONTROL REGISTER
#define RXB0CTRL 0x60	//RXB0CTRL – RECEIVE BUFFER 0 CONTROL
#define RXB1CTRL 0x70	//RXB1CTRL – RECEIVE BUFFER 1 CONTROL

#define TXB0SIDH 0x31	//идентификатор стандартного сообщения буфера передачи 0 - старший
#define TXB1SIDH 0x41	//идентификатор стандартного сообщения буфера передачи 1 - старший
#define TXB2SIDH 0x51	//идентификатор стандартного сообщения буфера передачи 2 - старший

#define TXB0SIDL 0x32	//идентификатор стандартного сообщения буфера передачи 0 - младший
#define TXB1SIDL 0x42	//идентификатор стандартного сообщения буфера передачи 1 - младший
#define TXB2SIDL 0x52	//идентификатор стандартного сообщения буфера передачи 2 - младший

#define TXB0EID8 0x33	//идентификатор расширенного сообщения буфера передачи 0 - старший
#define TXB1EID8 0x43	//идентификатор расширенного сообщения буфера передачи 1 - старший
#define TXB2EID8 0x53	//идентификатор расширенного сообщения буфера передачи 2 - старший

#define TXB0EID0 0x34	//идентификатор расширенного сообщения буфера передачи 0 - младший
#define TXB1EID0 0x44	//идентификатор расширенного сообщения буфера передачи 1 - младший
#define TXB2EID0 0x54	//идентификатор расширенного сообщения буфера передачи 2 - младший

#define TXB0DLC 0x35	//размер передаваемых данных
#define TXB1DLC 0x45	//размер передаваемых данных
#define TXB2DLC 0x55	//размер передаваемых данных

#define TXB0D0 0x36		//загрузка байта данных в буфер
#define TXB0D1 0x37		//загрузка байта данных в буфер
#define TXB0D2 0x38		//загрузка байта данных в буфер
#define TXB0D3 0x39		//загрузка байта данных в буфер
#define TXB0D4 0x3A		//загрузка байта данных в буфер
#define TXB0D5 0x3B		//загрузка байта данных в буфер
#define TXB0D6 0x3C		//загрузка байта данных в буфер
#define TXB0D7 0x3D		//загрузка байта данных в буфер

#define RXB0D0 0x66		//адрес буфера буфера чтения
#define RXB0D1 0x67		//адрес буфера буфера чтения
#define RXB0D2 0x68		//адрес буфера буфера чтения
#define RXB0D3 0x69		//адрес буфера буфера чтения
#define RXB0D4 0x6A		//адрес буфера буфера чтения
#define RXB0D5 0x6B		//адрес буфера буфера чтения
#define RXB0D6 0x6C		//адрес буфера буфера чтения
#define RXB0D7 0x6D		//адрес буфера буфера чтения

#define RXB1D0 0x76		//адрес буфера буфера чтения
#define RXB1D1 0x77		//адрес буфера буфера чтения
#define RXB1D2 0x78		//адрес буфера буфера чтения
#define RXB1D3 0x79		//адрес буфера буфера чтения
#define RXB1D4 0x7A		//адрес буфера буфера чтения
#define RXB1D5 0x7B		//адрес буфера буфера чтения
#define RXB1D6 0x7C		//адрес буфера буфера чтения
#define RXB1D7 0x7D		//адрес буфера буфера чтения

#define RXF0SIDH 0x00	//фильтр стандартного идентификатора буфера 0 - старший
#define RXF1SIDH 0x04	//фильтр стандартного идентификатора буфера 1 - старший
#define RXF2SIDH 0x08	//фильтр стандартного идентификатора буфера 2 - старший
#define RXF3SIDH 0x10	//фильтр стандартного идентификатора буфера 3 - старший
#define RXF4SIDH 0x14	//фильтр стандартного идентификатора буфера 3 - старший
#define RXF5SIDH 0x18	//фильтр стандартного идентификатора буфера 3 - старший

#define RXF0SIDL 0x01	//фильтр стандартного идентификатора буфера 0 - младший
#define RXF1SIDL 0x05	//фильтр стандартного идентификатора буфера 1 - младший
#define RXF2SIDL 0x09	//фильтр стандартного идентификатора буфера 2 - младший
#define RXF3SIDL 0x11	//фильтр стандартного идентификатора буфера 3 - младший
#define RXF4SIDL 0x15	//фильтр стандартного идентификатора буфера 3 - младший
#define RXF5SIDL 0x19	//фильтр стандартного идентификатора буфера 3 - младший

#define RXF0EID8 0x02	//фильтр расширенного идентификатора буфера 0 - старший
#define RXF1EID8 0x06	//фильтр расширенного идентификатора буфера 1 - старший
#define RXF2EID8 0x0A	//фильтр расширенного идентификатора буфера 2 - старший
#define RXF3EID8 0x12	//фильтр расширенного идентификатора буфера 3 - старший
#define RXF4EID8 0x16	//фильтр расширенного идентификатора буфера 3 - старший
#define RXF5EID8 0x1A	//фильтр расширенного идентификатора буфера 3 - старший

#define RXF0EID0 0x03	//фильтр расширенного идентификатора буфера 0 - младший
#define RXF1EID0 0x07	//фильтр расширенного идентификатора буфера 1 - младший
#define RXF2EID0 0x0B	//фильтр расширенного идентификатора буфера 2 - младший
#define RXF3EID0 0x13	//фильтр расширенного идентификатора буфера 3 - младший
#define RXF4EID0 0x17	//фильтр расширенного идентификатора буфера 3 - младший
#define RXF5EID0 0x1B	//фильтр расширенного идентификатора буфера 3 - младший

#define RXM0SIDH 0x20	//маска стандартного идентификатора буфера 0 - старший
#define RXM1SIDH 0x24	//маска стандартного идентификатора буфера 1 - старший

#define RXM0SIDL 0x21	//маска стандартного идентификатора буфера 0 - младший
#define RXM1SIDL 0x25	//маска стандартного идентификатора буфера 1 - младший

#define RXM0EID8 0x22	//маска расширенного идентификатора буфера 0 - старший
#define RXM1EID8 0x26	//маска расширенного идентификатора буфера 1 - старший

#define RXM0EID0 0x23	//маска расширенного идентификатора буфера 0 - младший
#define RXM1EID0 0x27	//маска расширенного идентификатора буфера 1 - младший

//команды
#define CANRESET 0xC0				//инициализация внутренних регистров
#define READ 0x03				//чтение из регистра
#define READSTATUS	0xA0			//чтение из регистра	
#define READRX0DH	0x90	//чтение буфера. старт с RXB0SIDH
#define READRX0D0	0x92	//чтение буфера. старт с RXB0SID0
#define READRX1DH	0x94	//чтение буфера. старт с RXB1SIDH
#define READRX1D0	0x96	//чтение буфера. старт с RXB1SID0
#define WRITE 0x02				//запись данных в регистр
#define LOADBUFFER		0x40
#define LOADTX0DH	0x40	//загрузка в передающий буфер 0 старт с TXB0SIDH
#define LOADTX0D0	0x41	//загрузка в передающий буфер 0 старт с TXB0SID0
#define LOADTX1DH	0x42	//загрузка в передающий буфер 1 старт с TXB1SIDH
#define LOADTX1D0	0x43	//загрузка в передающий буфер 1 старт с TXB1SID0
#define LOADTX2DH	0x44	//загрузка в передающий буфер 2 старт с TXB2SIDH
#define LOADTX2D0	0x45	//загрузка в передающий буфер 2 старт с TXB2SID0			

#define RXSTATUS	0xB0	//состояние RX
#define BITMODIFY	0x05	//изменение бита в конкретном регистре
#define RTSTXB0		0x81	//инициирования передачи сообщения для 1-го буфера TXB0
#define RTSTXB1		0x82	//инициирования передачи сообщения для 2-го буфера TXB1
#define RTSTXB2		0x84	//инициирования передачи сообщения для 3-го буфера TXB2
#define RTSTX		0x80	//инициирования передачи сообщения - просто команда

//управление битами регистров
#define MODE_MASK	0xE0	//маска для установки режимов: 000 - Normal; 001 - Sleep; 010 - Loopback; 011 - Listen-Only; 100 - Configuration
#define MODE_CONF	0x80	//установка режима конфигурирования MCP
#define MODE_NORM	0x00	//установка режима нормал MCP
#define BIT_OSM		0x08	//бит однократного режима передачи (One-Shot mode).

#define CNF1_MASK_BRP		0x1F	//биты прескалера регистра CNF1
#define CNF2_MASK_PHSEG1	0x38	//биты сегмента фазы 1 регистра CNF1
#define CNF2_MASK_PRSEG		0x07	//биты сегмента распространения регистра CNF2
#define CNF3_MASK_PHSEG2	0x07	//биты сегмента фазы 2 регистра CNF3
#define CNF2_MASK_BTLMODE	0x80	//бит управления длиной PS2, определяется в CNF3
#define CNF2_SET_BTLMODE_0	0x00	//бит управления длиной PS2, определяется в CNF3
#define CNF2_SET_BTLMODE_1	0x80
#define CNF1_MASK_SJW		0xC0	//биты синхронизации
#define CNF1_SET_SJW_1		0x00
#define CNF1_SET_SJW_2		0x40
#define CNF1_SET_SJW_3		0x80
#define CNF1_SET_SJW_4		0xC0

#define MASK_LENGHT_DATA		0x07	//биты - управление количеством байт данных сообщения (TXBnDLC)

#define CANINTF_MASK_TXCLR		0x04

#define MASK_PRIORITY_MSG		0x03
#define MASK_RMX_ON_OFF			0x60	//биты - управления режимом работы буфера приема - включить/отключить маски/фильтры
#define MASK_BUKT				0x04	//бит разрешения пролонгации буфера
#define MASK_OSM				0x08	//бит однократного режима передачи
#define MASK_OFF				0x60	//отключены маски приема, принимать все сообщения
//#define MASK_ON					0x20	//включить маски приема, принимать сообщения с использованием стандартных или расширенных ID, которые удовлетворяют критерию фильтра
#define MASK_ON_00				0x00	//включить маски приема, принимать сообщения с использованием стандартных или расширенных ID, которые удовлетворяют критерию фильтра
#define MASK_ON_01				0x40	//включить маски только со стандартным id
#define MASK_RX0IF_RESET		0x01	//маска сброса прерывания по заполнению буфера приема RX0
#endif