#include <mega128.h>
#include "init_board.h"

#define MY_BIT (io.bits)
#define MY_MASK (1<<(io.bits))


void port_setting(set_of_inputs io)	//���������� ����������� ����� (DDR) ����������� ����
{
	(*((volatile uint8_t*)io.port+1)) &= ~(MY_MASK);			//���������� ��� ����
	(*((volatile uint8_t*)io.port+2)) |= (MY_MASK);			//������ �������� �����
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
	(*((volatile uint8_t*)io.port+1)) |= (MY_MASK);			//���������� ��� �����
}

void port_set_direct_in(set_of_inputs io)						
{
	(*((volatile uint8_t*)io.port+1)) &= ~(MY_MASK);			//���������� ��� ����
}

void port_set_out(set_of_inputs io)	
{
	(*((volatile uint8_t*)io.port+2)) |= (MY_MASK);			//������ �����
}

void port_unset_out(set_of_inputs io)
{
	(*((volatile uint8_t*)io.port+2)) &= ~(MY_MASK);			//������� �����
}						   					  							 