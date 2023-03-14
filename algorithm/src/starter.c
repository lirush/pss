#include "starter.h"
#include "algsep.h"

set_of_inputs ctrl_starter={&PINC,PORTC0};					//��������/���������
set_of_inputs ctrl_starter_block={&PINC,PORTC2};			//�����������/��������������

void starter_on(void)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //���� ������ ����� -> ���������� ������ ���������
	port_set_out(ctrl_starter);
}

void starter_off(void)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //���� ������ ����� -> ���������� ������ ���������
	port_unset_out(ctrl_starter);
}

void starter_block(void)
{
	port_set_out(ctrl_starter_block);
}

void starter_unblock(void)
{
	port_unset_out(ctrl_starter_block);
}

void starter_state(void)
{
	Disel *ptrdisel = &Sysep.disel;
	Ebu *ptrebu = &ptrdisel->ebu;
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)					//SA12(���������� ������ ���) ���� ������
    { 	
        if(ptrebu->read_engine_speed>=ENGINESPEED400)
        starter_block();  
    	
        if(ptrebu->read_engine_speed<ENGINESPEED400)
        starter_unblock();	                                       
    }
}