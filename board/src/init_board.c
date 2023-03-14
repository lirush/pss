#include <mega128.h>
#include "init_board.h"

#define MY_BIT (io.bits)
#define MY_MASK (1<<(io.bits))


void port_setting(set_of_inputs io)	//определяем направление порта (DDR) конкретного пина
{
	(*((volatile uint8_t*)io.port+1)) &= ~(MY_MASK);			//определяем как вход
	(*((volatile uint8_t*)io.port+2)) |= (MY_MASK);			//делаем подтяжку входа
}

uint8_t port_state(set_of_inputs io)
{
	return ((*(volatile uint8_t*)io.port) & MY_MASK)?0:1;
}

uint8_t port_state_for_outs(set_of_inputs io)
{
	return ((*(volatile uint8_t*)io.port) & MY_MASK)?1:0;
}

void port_set_direct_out(set_of_inputs io)
{
	(*((volatile uint8_t*)io.port+1)) |= (MY_MASK);			//определяем как выход
}

void port_set_direct_in(set_of_inputs io)						
{
	(*((volatile uint8_t*)io.port+1)) &= ~(MY_MASK);			//определяем как вход
}

void port_set_out(set_of_inputs io)	
{
	(*((volatile uint8_t*)io.port+2)) |= (MY_MASK);			//ставим выход
}

void port_unset_out(set_of_inputs io)
{
	(*((volatile uint8_t*)io.port+2)) &= ~(MY_MASK);			//снимаем выход
}						   					  							 