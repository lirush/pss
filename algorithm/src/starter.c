#include "starter.h"
#include "algsep.h"

set_of_inputs ctrl_starter={&PINC,PORTC0};					//включить/выключить
set_of_inputs ctrl_starter_block={&PINC,PORTC2};			//блокировать/разблокировать

void starter_on(void)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //если ручной режим -> управление ключом запрещено
	port_set_out(ctrl_starter);
}

void starter_off(void)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //если ручной режим -> управление ключом запрещено
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
    
    if(!ptrbtn2->bit0)					//SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП) если ручной
    { 	
        if(ptrebu->read_engine_speed>=ENGINESPEED400)
        starter_block();  
    	
        if(ptrebu->read_engine_speed<ENGINESPEED400)
        starter_unblock();	                                       
    }
}