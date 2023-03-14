#include <string.h>
#include "spi.h"

/*
#define MCP_CS_ON(sfr,bit)	(_SFR_BYTE(sfr) &= ~_BV(bit))		//РІС‹Р±РѕСЂ SPI slave
#define MCP_CS_OFF(sfr, bit)  (_SFR_BYTE(sfr) |= _BV(bit))		//СЃРЅСЏС‚СЊ РІС‹Р±РѕСЂ SPI slave*/

//uint8_t SPI_TX_data[SPI_BUFF_SIZE];				
uint8_t SPI_data[SPI_BUFF_SIZE];				
uint8_t SPI_TX_cnt = 0;	
//uint8_t SPI_RX_cnt = 0,SPI_RX_cnt_prev=0;
volatile uint8_t SPI_busy = 0;				

static volatile uint8_t *outptr;
static volatile uint8_t ch_can=0;

selector Sel_impl[]={
	{&PORTB,PORTB0},
	{&PORTB,PORTB4}
};	

void spi_sel_on(uint8_t ch_num)
{
    selector *ptr = Sel_impl+ch_num;
    (*(volatile uint8_t *)(ptr->port)) &= ~(1<<ptr->b);                //открываем канал SPI   
}

void spi_sel_off(uint8_t ch_num)
{
    selector *ptr = Sel_impl+ch_num;
    (*(volatile uint8_t *)(ptr->port)) |= (1<<ptr->b);                //закрываем канал SPI       
}	
	
interrupt [SPI_STC] void spi_stc_isr(void)
{	
	*(outptr++) = SPDR;			
	if(SPI_TX_cnt)	{
		SPI_TX_cnt--;
		SPDR = *(outptr);		
	}
    else
    {
        SPI_busy=0;
     //spi_sel_off(ch_can);
    }
}

void Init_SPIMaster(enum DIV spi_div)
{
	volatile char IOReg;
	DDRB |= (1<<PORTB0)|(1<<PORTB4)|(1<<DD_MOSI)|(1<<DD_SCK);
    PORTB |= (1<<PORTB0) | (1<<PORTB4);		
	SPCR = (1<<SPE)|(1<<MSTR);// | (1<<SPIE);	
	
   IOReg = SPSR;							
   IOReg = SPDR;	
	
	switch(spi_div)	{
		case OSC_DIV_2:						//fosc/2									
			SPSR |= (1<<SPI2X);
			SPCR &= ~((1<<SPR0)|(1<<SPR1));
			break;
		case OSC_DIV_4:						//fosc/4
			SPSR &= ~(1<<SPI2X);
			SPCR &= ~((1<<SPR0)|(1<<SPR1));
			break;
		case OSC_DIV_8:						//fosc/8
			SPSR |= (1<<SPI2X);
			SPCR |= (1<<SPR0);
			SPCR &= ~(1<<SPR1);
			break;
		case OSC_DIV_16:					//fosc/16
			SPSR &= ~(1<<SPI2X);
			SPCR |= (1<<SPR0);
			SPCR &= ~(1<<SPR1);
			break;
		case OSC_DIV_32:					//fosc/32
			SPSR |= (1<<SPI2X);
			SPCR |= (1<<SPR1);
			SPCR &= ~(1<<SPR0);
			break;
		case OSC_DIV_64:					//fosc/64
			SPSR &= ~(1<<SPI2X);
			SPCR |= (1<<SPR1);
			SPCR &= ~(1<<SPR0);
			break;
		case OSC_DIV_128:					//fosc/128
			SPSR &= ~(1<<SPI2X);
			SPCR |= (1<<SPR1) | (1<<SPR0);			
			break;
	}
}

void putspi(void)
{
    static uint8_t locnt=0;
	if(SPI_TX_cnt)	{
		SPI_TX_cnt--;
		SPDR = SPI_data[locnt++];
        while(!(SPSR & (1<<7)));		
	}
    else
    {  
        locnt=0;
        SPI_busy=0;
    }
}

void getspi(void)
{
    static uint8_t locnt=0;
	if(SPI_TX_cnt)	{
		SPI_TX_cnt--;
		SPDR = SPI_data[locnt];		//если это не последний байт, то выводим в передатчик
        while(!(SPSR & (1<<7)));
        SPI_data[locnt++] = SPDR;
	}
	else {
		SPI_busy=0;		
	}
}

void exchange(uint8_t ch_num,uint8_t *data,uint8_t size)
{
    spi_sel_on(ch_num);
    ch_can = ch_num;
    uSPIWrite(data,size);
    //while(SPIBusy){putspi();} 
    spi_sel_off(ch_num);
    
}

void exchange_read(uint8_t ch_num,uint8_t *data,uint8_t size)
{
    spi_sel_on(ch_num);
    ch_can = ch_num;
    uSPIReadWrite(data,size);
    //while(SPI_busy){getspi();} 
    spi_sel_off(ch_num);
    
}

uint8_t uSPIWrite(uint8_t *data_in_out, uint8_t size_data)
{
    int i=0; uint8_t data=0;
	while(SPI_busy);
	SPI_busy++;
	if(size_data)
		SPI_TX_cnt = size_data;
	else return 0;
    
    for(i=0;i<SPI_TX_cnt;i++)
    {
         SPDR = data_in_out[i];
         while(!(SPSR & (1<<7)));
         data = SPDR;
    }
    SPI_busy=0;  
	return 0;
}

uint8_t uSPIReadWrite(uint8_t *data, uint8_t size_data)
{
    int i=0;
	while(SPI_busy);
	SPI_busy++;
	if(size_data)
		SPI_TX_cnt = size_data;
	else return 0; 
    for(i=0;i<SPI_TX_cnt;i++)
    {
         SPDR = data[i];
         while(!(SPSR & (1<<7)));
         data[i]=SPDR;
    }        
    SPI_busy=0; 
	return 0;
}

void SPIBusy(void)
{
	while(SPI_busy);
}