/**
  ******************************************************************************
  * @file    uart.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием перечеслений, классов и функций UART интерфейса
  ******************************************************************************
*/
#ifndef __UART_H
#define __UART_H

enum {U0,U1};			//uart0,1
#define FOSC 8000000l	// Clock Speed	
//#define MYUBRR(A) ((UCSR1A & 0x02) ? (FOSC/A/8-1):(FOSC/A/16-1))	//скорость передачи по uart-у
#define MYUBRR(A) (FOSC/A/16)
#define BUFF_SIZE 50
#define SOH		0x73    /*!стартовый байт */
#define SOH1	0x74    /*!стартовый байт */
#define SOH2	0x61    /*!стартовый байт */
#define SOH3	0x72    /*!стартовый байт */
#define UART_quantity	2
#define UART_UDR_EN(n)	((n==U1)?(UCSR1B |= (1<<UDRIE1)):(UCSR0B |= (1<<UDRIE0)))

#define FRAMING_ERROR (1<<FE)
#define DATA_OVERRUN (1<<DOR)

/*!
  \class flags_state
  \brief Флаг стаус
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
  \brief UART корзина
  \details Инициализация передача/прием UART пакетов.
  Загрузка в буфер данных.
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
  Для временного хранения принятых данных 
*/
typedef struct  
{
	uint8_t FlagTmpbufFull;
	uint8_t len;					//количество байт данных
	uint8_t RX_buf[BUFF_SIZE];
}UART_RX_tmp;
//extern UART_RX_tmp UART_RX_temp[UART_quantity];

static inline int myubrr(volatile uint8_t reg, uint32_t baud)
{
	return (*(volatile uint8_t *)(&reg) & 0x02) ? (FOSC/baud/8-1):(FOSC/baud/16);
}

/*!
  Инициализация UART интерфейса 
*/
void Init_USART(uint8_t ch, uint32_t baud);

/*!
  Инициализирует передачу, передает данные в UART
  
  \param num_uart номер устройства UART
  \param *data_out указатель на передаваемые данные
  \param size_data количество байт данных
  \return 0 
*/
int USART_send(uint8_t num_uart, uint8_t *data_out, uint8_t size_data);

/*!
  Кладет по одному байту в UART передатчик
  Задумывалось как внеочередная передача данных по UART 
*/
void putuart(uint8_t ch);

//extern uint8_t uart_msg_receive;
//extern uint8_t RX_data[30];             /*!буфер принятых данных*/

#include "./comm/uart/src/uart.c"

#endif