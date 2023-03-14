#include <string.h>
#include "uart.h"
#include "input.h" 

UART_basket UART_RX_basket[UART_quantity];
UART_basket UART_TX_basket[UART_quantity];
//UART_RX_tmp UART_RX_temp[UART_quantity];		

set_of_inputs Set_status_reg_uart[2]={
	{&UCSR0A,UDRE0},
    {&UCSR1A,UDRE1}
};
set_of_inputs Set_data_reg_uart[2]={
	{&UDR0,0},
    {&UDR1,0}
};

//uint8_t uart_msg_receive=0;
//static uint8_t cnt=0;
//static uint8_t status=0;
//static volatile uint8_t *inptr;
//static uint8_t collision;
//uint8_t RX_data[30];
//static uint8_t len_data_uart=0;

enum stat{START_BYTE_NOT_RECEIVED=0,	//старт байт не принят
			START_BYTE_RECEIVED0,		//старт байт принят
			START_BYTE_RECEIVED1,
			START_BYTE_RECEIVED2,
			START_BYTE_RECEIVED3,
			CNT_RUNNIG,					//копируем данные и включаем счетчик
			CNT_END};					//выключаем счетчик    
            
void uart_handler_receive(uint8_t ch, uint8_t data)
{
    
    UART_basket *ptr = UART_RX_basket+ch;
    uint8_t cnt = ptr->cnt;
    uint8_t status = ptr->status;
    if(ptr->fl_state.msg_receive)return; 
    switch(status)
    {
        case START_BYTE_NOT_RECEIVED:                    //старт байт не принят
            if(data==SOH){                                           
                status = START_BYTE_RECEIVED0;        //старт байт принят
            }
            break;
        case START_BYTE_RECEIVED0:                    //старт байт не принят
        if(data==SOH1){                                              
            status = START_BYTE_RECEIVED1;        //старт байт принят
        }
        else {
            ptr->fl_state.skip = 1;
            status = CNT_END;
        }
        break;
        case START_BYTE_RECEIVED1:                    //старт байт не принят
        if(data==SOH2){
            status = START_BYTE_RECEIVED2;        //старт байт принят
        }
        else {
            ptr->fl_state.skip = 1;
            status = CNT_END;
        }        
        break;
        case START_BYTE_RECEIVED2:                    //старт байт не принят
        if(data==SOH3){
            status = START_BYTE_RECEIVED3;        //старт байт принят
        }
        else {
            ptr->fl_state.skip = 1;
            status = CNT_END;
        }        
        break;                                    
        case START_BYTE_RECEIVED3:
            if(data==0){                                        //если размер пакета нулевой
                status = START_BYTE_NOT_RECEIVED;            //не принимаем пакет
            }
            else{
                if(ptr->fl_state.msg_receive == 1){                        //если в это время сообщение уже принимается
            		ptr->fl_state.collision=1;							//устанавливаем коллизию
					ptr->fl_state.skip = 1;								//просматриваем пакет
					}
					ptr->size_data = data;					//копируем размер, устанавливаем счетчик
				}
				if(ptr->size_data>BUFF_SIZE)
				status = START_BYTE_NOT_RECEIVED;			//флаг, чтобы не копировать сообщения, в случае превышения размера принятых данных размера буфера RX_data
				else status = CNT_RUNNIG;
				break;				
		case CNT_RUNNIG:
				if(ptr->fl_state.skip==0){
					ptr->buf[cnt]=data;
				}
				if(++cnt<ptr->size_data){
					break;
				}		
		case CNT_END:
				if(ptr->fl_state.skip==1)
				ptr->fl_state.skip=0;
				else ptr->fl_state.msg_receive = 1;							//приняли пакет данных
				ptr->fl_state.collision = 0;
                status = START_BYTE_NOT_RECEIVED;
				cnt=0;
				break;
	}
	ptr->cnt=cnt;
	ptr->status = status;
}            

interrupt [USART1_RXC] void usart1_rx_isr(void)
{
	uint8_t tmp = 0;
	tmp=UDR1;
	uart_handler_receive(U1,tmp);    	
}

interrupt [USART0_RXC] void usart0_rx_isr(void)
{
	uint8_t tmp = 0; 
	tmp=UDR0;
	uart_handler_receive(U0,tmp);
}

void Init_USART(uint8_t ch, uint32_t baud)
{
//    UART_basket *ptr = UART_TX_basket+U1;
    //int ubrr = myubrr(UCSR1A,baud);
	int ubrr = MYUBRR(baud);     
    switch(ch)
    {
     case U0:
        DDRE |= (1<<PORTE1);			//линия TX выход
        PORTE |= (1<<PORTE1);
        UCSR0A = 0x00;
        //UCSR0A |= (1<<U2X0);		//удвоенная скорость передачи
        //UCSR0A &= ~(1<<U2X0);
        /* Set baud rate */
        UBRR0H = (unsigned char)(ubrr>>8);
        UBRR0L = (unsigned char)ubrr;
        /* Enable receiver and transmitter */
        UCSR0B = (1<<RXEN0)|(1<<TXEN0);	//
        /* Set frame format: 8data, 1stop bit */
        UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);     
     break;
     case U1:
        DDRD |= (1<<PORTD3);			//линия TX выход
        PORTD |= (1<<PORTD3);
        UCSR1A = 0x00;
        //UCSR1A |= (1<<U2X1);		//удвоенная скорость передачи
        //UCSR1A &= ~(1<<U2X1);
        /* Set baud rate */
        UBRR1H = (unsigned char)(ubrr>>8);
        UBRR1L = (unsigned char)ubrr;
        /* Enable receiver and transmitter */
        UCSR1B = (1<<RXEN1)|(1<<TXEN1);
        /* Set frame format: 8data, 1stop bit */
        UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);     
     break;
    }
}


void putuart(uint8_t ch)
{
	UART_basket *ptr = UART_TX_basket+ch;
    set_of_inputs *pSReg = Set_status_reg_uart+ch;
    set_of_inputs *pDReg = Set_data_reg_uart+ch;
	static uint8_t locnt=0;
    if(ptr->fl_state.busy==0)return;
	while(!(*pSReg->port & (1<<pSReg->b)));
    if(ptr->size_data--)
	{   
		*pDReg->port = ptr->buf[locnt++];					//передача данных в выходной буфер uart-а
	} 
	if(ptr->size_data==0)
	{
		locnt=0;
		ptr->fl_state.busy=0;						//передатчик свободен
	}
}

int USART_send(uint8_t num_uart, uint8_t *data_out, uint8_t size_data)
{	
	UART_basket *ptr = UART_TX_basket+num_uart;	
    set_of_inputs *pSReg = Set_status_reg_uart+num_uart;
    set_of_inputs *pDReg = Set_data_reg_uart+num_uart;
    uint8_t i=0;    
	if(sizeof(ptr->buf) < size_data+5)				//проверка размера буфера передаваемых данных с размером желаемым
		return -1;		
//	if(ptr->fl_state.busy)return 0;
//    ptr->fl_state.busy=1;
        						
    ptr->buf[0] = SOH;								//стартовый байт
    ptr->buf[1] = SOH1;								//стартовый байт
    ptr->buf[2] = SOH2;								//стартовый байт
    ptr->buf[3] = SOH3;								//стартовый байт
    ptr->buf[4] = size_data;						//размер данных
    memcpy(ptr->buf+5,data_out,size_data);			//копировать в выходной буфер
    ptr->size_data=size_data+5;
    for(i=0;i<ptr->size_data;i++)
    {
        while(!(*pSReg->port & (1<<pSReg->b)));
       *pDReg->port = ptr->buf[i];					//передача данных в выходной буфер uart-а
    }
	return 1;
}