#include "ebu.h"
#include "algsep.h"

set_of_inputs ctrl_ebu_900={&PINA,PORTA0};					//900 оборотов
set_of_inputs ctrl_ebu_1800={&PINA,PORTA2};				//1800 оборотов

void ebu_start(uint16_t speeed)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //если ручной режим -> управление ключом запрещено
	switch(speeed) {
		case ENGINESPEED900:
			port_set_out(ctrl_ebu_900);
		break;
		case ENGINESPEED1800:
			port_set_out(ctrl_ebu_1800);
		default:
		break;
	}
}

void ebu_stop(uint16_t speeed)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //если ручной режим -> управление ключом запрещено
	switch(speeed) {
		case ENGINESPEED900:
		port_unset_out(ctrl_ebu_900);
		break;
		case ENGINESPEED1800:
		port_unset_out(ctrl_ebu_1800);
		default:
		break;
	}	
}