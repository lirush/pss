#include "conf.h"
#include "discret_input.h"

set_of_inputs IO_impl_new[24]={
	{&PINA,PORTA0},
	{&PINA,PORTA1},
	{&PINA,PORTA2},
	{&PINA,PORTA3},
	{&PINA,PORTA4},
	{&PINA,PORTA5},
	{&PINA,PORTA6},
	{&PINA,PORTA7},
	{&PINC,PORTC0},
	{&PINC,PORTC1},
	{&PINC,PORTC2},
	{&PINC,PORTC3},
	{&PINC,PORTC4},
	{&PINC,PORTC5},
	{&PINC,PORTC6},
	{&PINC,PORTC7},
	{&PIND,PORTD7},
	{&PIND,PORTD6},
	{&PIND,PORTD5},
	{&PIND,PORTD4},
	{&PINB,PORTB5},
	{&PINB,PORTB6},
	{&PIND,PORTD0},
	{&PIND,PORTD1}
};
set_of_inputs IO_impl_old[24]={
	{&PINA,PORTA0},
	{&PINA,PORTA1},
	{&PINA,PORTA2},
	{&PINA,PORTA3},
	{&PINA,PORTA4},
	{&PINA,PORTA5},
	{&PINA,PORTA6},
	{&PINA,PORTA7},
	{&PINC,PORTC0},
	{&PINC,PORTC1},
	{&PINC,PORTC2},
	{&PINC,PORTC3},
	{&PINC,PORTC4},
	{&PINC,PORTC5},
	{&PINC,PORTC6},
	{&PINC,PORTC7},
	{&PINF,PORTF6},
	{&PINF,PORTF7},
	{&PINA,PORTA7},
	{&PINA,PORTA6},
	{&PINA,PORTA5},
	{&PINC,PORTC0},
	{&PINC,PORTC1},
	{&PINC,PORTC2}
};

#define IN_quantity	sizeof(IO_impl_old)/sizeof(set_of_inputs)-4
#define OUT_quantity sizeof(IO_impl_old)/sizeof(set_of_inputs)
void Init_discret_input(void)
{	
    int i =0;
	set_of_inputs *IO_impl = IO_impl_old;
	if(FindModeBoard())IO_impl = IO_impl_new;
	for(i=0;i<IN_quantity;i++){
		if (IO_impl->port == &PINF){
				DDRF &= ~(1<<IO_impl->b);
				PORTF |= (1<<IO_impl->b);
		}
		else port_setting(*IO_impl);
		IO_impl++;
	}
}

void Init_discret_output(void)
{
    int i =0;
	set_of_inputs *IO_impl = IO_impl_new;
	for(i=0;i<OUT_quantity;i++){
		port_set_direct_out(*IO_impl);
		port_unset_out(*IO_impl);					//убираем напряжение с выхода
		IO_impl++;
	}
}

static char resultString[IN_quantity+1];

char* GetResult(void){
    int i =0;		
	set_of_inputs *IO_impl = IO_impl_old;
	if(FindModeBoard())IO_impl = IO_impl_new;	
	for(i=0;i<IN_quantity;i++){
		resultString[i]= port_state(*IO_impl++)? '1':'0';
	}	
	resultString[IN_quantity]=0;
	return resultString;
}

uint8_t k=0;
uint32_t DITemp[3]={0,0,0};
uint32_t Get2WordResult(void){	
    int i =0;
    uint32_t tmp=0;
	set_of_inputs *IO_impl = IO_impl_old;
	if(FindModeBoard())IO_impl = IO_impl_new;
	for(i=0;i<IN_quantity;i++){
		tmp |= ((uint32_t)(port_state(*IO_impl++)?1:0)<<i);
	}
	return tmp;
}

uint32_t GetStateOuts(void){
    int i =0;
	set_of_inputs *IO_impl = IO_impl_new;
	uint32_t tmp=0;
	for(i=0;i<OUT_quantity;i++){
		tmp |= ((uint32_t)(port_state_for_outs(*IO_impl++)?1:0)<<i);
	}                                                                 
	return tmp;
}

uint8_t DI_Out[CH_DI_num]; //__attribute__ ((section (".noinit")));
//long cnt_init_port=1000L;
void Get_DISensors(void)
{
    int i =0;
	uint32_t tmp=0;
    DITemp[k] = Get2WordResult(); 
    if(k)
    {   
        tmp=DITemp[0] & DITemp[1];
        for(i =0;i<3;i++){
            DI_Out[i]=(uint8_t)(tmp>>(i*8));
        }
    }
    if(k)k=0;
    else k=1;

//	if(cnt_init_port<=0)
//		DI_Out[3] =0xAA;									//диагностическая заплатка
//	else cnt_init_port--;
//if(PORTG & (1<<PORTG4))DI_Out[3] =0xAA;
}

void Get_StateOuts(void){
    int i=0;
	uint32_t tmp = GetStateOuts();
	for(i =0;i<3;i++){
		DI_Out[i]=(uint8_t)(tmp>>(i*8));
	}
}

