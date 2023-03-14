/**
  ******************************************************************************
  * @file    uart.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� ������������, ������� � ������� UART ����������
  ******************************************************************************
*/
#ifndef __UART_H
#define __UART_H

enum {U0,U1};			//uart0,1
#define FOSC 8000000l	// Clock Speed	
//#define MYUBRR(A) ((UCSR1A & 0x02) ? (FOSC/A/8-1):(FOSC/A/16-1))	//�������� �������� �� uart-�
#define MYUBRR(A) (FOSC/A/16)
#define BUFF_SIZE 50
#define SOH		0x73    /*!��������� ���� */
#define SOH1	0x74    /*!��������� ���� */
#define SOH2	0x61    /*!��������� ���� */
#define SOH3	0x72    /*!��������� ���� */
#define UART_quantity	2
#define UART_UDR_EN(n)	((n==U1)?(UCSR1B |= (1<<UDRIE1)):(UCSR0B |= (1<<UDRIE0)))

#define FRAMING_ERROR (1<<FE)
#define DATA_OVERRUN (1<<DOR)

/*!
  \class flags_state
  \brief ���� �����
*/
typedef struct  
{
	unsigned int msg_receive	:1;
	unsigned int skip			:1;
	unsigned int collision		:1;
	unsigned int busy			:1;
}flags_state;

/*!
  \class UART_basket
  \brief UART �������
  \details ������������� ��������/����� UART �������.
  �������� � ����� ������.
*/
typedef struct
{
	uint8_t status;
	flags_state fl_state;
	uint8_t buf[BUFF_SIZE];
	uint8_t size_data;
	uint8_t cnt;      
}UART_basket;
extern UART_basket UART_RX_basket[UART_quantity];
extern UART_basket UART_TX_basket[UART_quantity];

/*!
  \class UART_RX_tmp
  ��� ���������� �������� �������� ������ 
*/
typedef struct  
{
	uint8_t FlagTmpbufFull;
	uint8_t len;					//���������� ���� ������
	uint8_t RX_buf[BUFF_SIZE];
}UART_RX_tmp;
//extern UART_RX_tmp UART_RX_temp[UART_quantity];

static inline int myubrr(volatile uint8_t reg, uint32_t baud)
{
	return (*(volatile uint8_t *)(&reg) & 0x02) ? (FOSC/baud/8-1):(FOSC/baud/16);
}

/*!
  ������������� UART ���������� 
*/
void Init_USART(uint8_t ch, uint32_t baud);

/*!
  �������������� ��������, �������� ������ � UART
  
  \param num_uart ����� ���������� UART
  \param *data_out ��������� �� ������������ ������
  \param size_data ���������� ���� ������
  \return 0 
*/
int USART_send(uint8_t num_uart, uint8_t *data_out, uint8_t size_data);

/*!
  ������ �� ������ ����� � UART ����������
  ������������ ��� ������������ �������� ������ �� UART 
*/
void putuart(uint8_t ch);

//extern uint8_t uart_msg_receive;
//extern uint8_t RX_data[30];             /*!����� �������� ������*/

#include "./comm/uart/src/uart.c"

#endif