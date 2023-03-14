#include <string.h>
#include "def.h"
#include "mcp2515.h"
#include "mcp.h"
#include "mspi.h"

void mcp_cs_on(volatile uint8_t *port, uint8_t b)	//перевести CS в лог. 0
{	
	#asm("cli");
	*port &= ~(1<<b);//_BV(bit);		//по прямой логике для выходов
	#asm("sei");
}

void mcp_cs_off(volatile uint8_t *port, uint8_t b)	//перевести CS в лог. 1
{
	#asm("cli");
	//(*(volatile uint8_t *)(port)) |= _BV(bit);	//по прямой логике для выходов
	*port |= (1<<b);	//по прямой логике для выходов
	#asm("sei");
}

void port_exchange(uint8_t *data,uint8_t size,uint8_t ch_num)
{
	selector *ptr = Sel_impl+ch_num;
	mcp_cs_on(ptr->port,ptr->b);
	uSPIWrite(data,size);
	//while(SPIBusy){putspi();}
    mcp_cs_off(ptr->port,ptr->b);
}

#define PORT_EXCHANGE(obj,n) port_exchange((uint8_t *)&obj,sizeof(obj),n)

//инициализация, сброс
void mcp_reset(uint8_t ch_num)
{
	uint8_t res = CANRESET;
	PORT_EXCHANGE(res,ch_num);
}

/*настройка команды изменения бита в регистре*/
void mcp_BitModify(uint8_t addr, uint8_t mask, uint8_t data,uint8_t ch_num)
{
	uint8_t msg[4];	
	msg[0] = BITMODIFY;					//команда - модификация бита
	msg[1] = addr;						//адрес регистра
	msg[2] = mask;						//байт маски
	msg[3] = data;						//байт данных	
	PORT_EXCHANGE(msg,ch_num);
}

void mcp_write(uint8_t data, uint8_t addr,uint8_t ch_num)
{
	uint8_t msg[3];
	
	msg[0] = WRITE;	//команда - запись в регистр
	msg[1] = addr;	//адрес регистра
	msg[2] = data;	//байт данных для записи	
	PORT_EXCHANGE(msg,ch_num);
}

uint8_t mcp_reg_read(uint8_t addr,uint8_t ch_num)
{
	uint8_t msg[3];
	msg[0] = READ;	//команда чтения регистра
	msg[1] = addr;	//адрес регистра
	PORT_EXCHANGE(msg,ch_num);
	return msg[2];
}

/*
8 бит данных
0 - CANINTF.RX0IF - флаг прерывания заполнения буфера приема 0
1 - CANINTF.RX1IF - флаг прерывания заполнения буфера приема 1
2 - TXB0CNTRL.TXREQ - бит запроса передачи сообщения буфера TX0
3 - CANINTF.TX0IF - флаг прерывания опустошения буфера передачи 0
4 - TXB1CNTRL.TXREQ - бит запроса передачи сообщения буфера TX1
5 - CANINTF.TX1IF - флаг прерывания опустошения буфера передачи 1
6 - TXB2CNTRL.TXREQ - бит запроса передачи сообщения буфера TX2
7 - CANINTF.TX2IF - флаг прерывания опустошения буфера передачи 2
*/
uint8_t mcp_status_read(uint8_t ch_num)
{
	uint8_t msg[2];
	msg[0] = READSTATUS;	//команда чтения регистра
	PORT_EXCHANGE(msg,ch_num);
	return msg[1];
}

/*******************************************************************************
используется для быстрого определения, какой фильтр пропустил сообщение, 
и сообщение какого типа было принято (standard, extended, remote)
********************************************************************************/
uint8_t mcp_rx_status_read(uint8_t ch_num)
{
	uint8_t msg[2];
	msg[0] = RXSTATUS;
	PORT_EXCHANGE(msg,ch_num);
	return msg[1];
}

/*******************************************************************************
загрузка буфера передачи
command		-	LOADBUFFER0DH 0x40	//буфер передачи 0 начиная с TXB0SIDH
				LOADBUFFER0D0 0x41	//буфер передачи 0 начиная с TXB0D0
				LOADBUFFER1DH 0x42	//буфер передачи 0 начиная с TXB1SIDH
				LOADBUFFER1D0 0x43	//буфер передачи 0 начиная с TXB1D0
				LOADBUFFER2DH 0x44	//буфер передачи 0 начиная с TXB2SIDH
				LOADBUFFER2D0 0x45	//буфер передачи 0 начиная с TXB2D0
********************************************************************************/
/*******************************************************************************
mcp_buf_transmit - заполняет соответствующий буфер (buf_num - TX0-2) передачи
********************************************************************************/
void mcp_buf_transmit(uint8_t ch_num, uint8_t tx_num)
{
	TX_control *ptr = CanEx[ch_num].TX_example+tx_num;
	PORT_EXCHANGE(ptr->whatToSend,ch_num);
}

void mcp_buf_load(uint8_t ch_num, uint8_t tx_num)
{
	selector *sel_ptr = Sel_impl+ch_num;
	TX_control *ptr = &CanEx[ch_num].TX_example[tx_num];
	(*(volatile uint8_t *)(sel_ptr->port)) &= ~(1<<sel_ptr->b);		//открываем канал SPI
	uSPIWrite((uint8_t *)&ptr->whatToSend,sizeof(ptr->whatToSend));						//загружаем
}

/*******************************************************************************
загрузка буфера приема
command -	READBUFFER0DH 0x90	//буфер приема 0, который начинается с RXB0SIDH
			READBUFFER0D0 0x92	//буфер приема 0, который начинается с RXB0D0
			READBUFFER0DH 0x94	//буфер приема 0, который начинается с RXB1SIDH
			READBUFFER1D0 0x96	//буфер приема 0, который начинается с RXB1D0
********************************************************************************/
/*******************************************************************************
mcp_buf_receive - чтение соответствующего буфера (buf_num - RX0-1) приема
********************************************************************************/
void mcp_buf_read(uint8_t ch_num, uint8_t rx_num)
{
	RX_control *ptr = &CanEx[ch_num].RX_example[rx_num];
	uint8_t tmp[14];
	tmp[0]=ptr->HeaderData.com_read;
	PORT_EXCHANGE(tmp,ch_num);
	memcpy(&ptr->HeaderData.Received,&tmp[1],sizeof(ptr->HeaderData.Received));
}

void mcp_rts(uint8_t ch_num, uint8_t buf_num)
{
	uint8_t cmd;
	switch(buf_num)
	{
		case 0:
			cmd = RTSTXB0;				//команда инициирование передачи сообщения TX0
			break;
		case 1:
			cmd = RTSTXB1;				//команда инициирование передачи сообщения TX1
			break;
		case 2:
			cmd = RTSTXB2;				//команда инициирование передачи сообщения TX2
			break;
	}
	PORT_EXCHANGE(cmd,ch_num);
}