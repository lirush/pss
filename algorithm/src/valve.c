#include "valve.h"
#include "algsep.h"

set_of_inputs ctrl_valve_open={&PINA,PORTA4};					//открыть
set_of_inputs ctrl_valve_close={&PINC,PORTC4};					//закрыть

void valve_open(void)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //если ручной режим -> управление ключом запрещено
	port_unset_out(ctrl_valve_close);
	port_set_out(ctrl_valve_open);
}

void valve_close(void)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //если ручной режим -> управление ключом запрещено
	port_unset_out(ctrl_valve_open);
	port_set_out(ctrl_valve_close);	
}

void valve_ctrl_ins(void)
{
	Blocks *ptrvalve = &Sysep.disel.valve;
	ByteData *ptrbtn1 = &btn1;
	//ByteData *ptrbtn2 = &btn2;
	Septimers *ptrtimer = septimers+THIRDTIMER;
	ptrvalve->inouts.bit0 = (port_state_for_outs(ctrl_valve_open) | ptrbtn1->bit4);						//состояние порта выхода или переключатель SA4 port_state_for_outs(ctrl_valve_open) | (ptrbtn1->bit4 & (!ptrbtn2->bit0));
	ptrvalve->inouts.bit1 = (port_state_for_outs(ctrl_valve_close) | ptrbtn1->bit5);	//port_state_for_outs(ctrl_valve_close) | (ptrbtn1->bit5 & (!ptrbtn2->bit0));
	if(ptrvalve->inouts.bit0 | ptrvalve->inouts.bit1)
	{
		user_timer_with_step_two_sec(1,THIRDTIMER,46);									//ждем 46-2 сек. и ставим аварию на открытия заслонки/закрытия
	}else 
	{
		ptrtimer->InsideCntTimer=0;
	}
	//user_timer_with_step_two_sec((ptrvalve->inouts.bit0 | ptrvalve->inouts.bit1),THIRDTIMER,46);		//ждем 46-2 сек. и ставим аварию на открытия заслонки/закрытия
	if(ptrtimer->FlagSetTimer) {
		ptrvalve->inouts.bit4 = ptrtimer->FlagSetTimer;
		ptrtimer->FlagSetTimer=0;
	}
}

void valve_state(void)
{	
	Blocks *ptrvalve = &Sysep.disel.valve;
    uint8_t *ptr = (uint8_t *)&ptrvalve->inouts;
	static uint8_t loc_reg=0;
	Septimers *ptrtimer = septimers+THIRDTIMER;
	valve_ctrl_ins();
	switch(*ptr) {
		case 0:
		if((loc_reg==0x01) || (loc_reg==0x02))
		loc_reg=ptrvalve->status_reg=0x00;
		break;
		case OPENING:
		ptrvalve->status_reg=0x01;
		loc_reg=ptrvalve->status_reg;
		break;
		case 0x09:
		ptrvalve->status_reg=0x01;
		loc_reg=ptrvalve->status_reg;
		break;		
		case CLOSING:
		ptrvalve->status_reg=0x02;
		loc_reg=ptrvalve->status_reg;		
		break;
		case 0x06:
		ptrvalve->status_reg=0x02;
		loc_reg=ptrvalve->status_reg;
		break;		
		case ISOPEN:
		ptrvalve->status_reg=0x04;											//открыта
		port_unset_out(ctrl_valve_open);
		loc_reg=ptrvalve->status_reg;
		break;
		case ISOPENVALVE:
		ptrvalve->status_reg=0x04;											//открыта
		port_unset_out(ctrl_valve_open);
		loc_reg=ptrvalve->status_reg;		
		break;
		case ISCLOSE:
		ptrvalve->status_reg=0x08;											//закрыта
		port_unset_out(ctrl_valve_close);
		loc_reg=ptrvalve->status_reg;
		break;
		case ISCLOSEVALVE:
		ptrvalve->status_reg=0x08;											//закрыта
		port_unset_out(ctrl_valve_close);
		loc_reg=ptrvalve->status_reg;		
		break;
		case ERRORCONNECT:
		ptrvalve->status_reg=0x0C;											//ошибка подключения
		break;
		case ERROROPEN1:
		ptrvalve->status_reg=0x0D;											//ошибка открытия
		ptrtimer->FlagSetTimer=0;
		loc_reg=ptrvalve->status_reg;
		break;
		case ERRORCLOSE1:
		ptrvalve->status_reg=0x0E;											//ошибка закрытия
		ptrtimer->FlagSetTimer=0;
		loc_reg=ptrvalve->status_reg;
		break;
		case ERROROPEN2:
		ptrvalve->status_reg=0x0D;											//ошибка открытия
		ptrtimer->FlagSetTimer=0;
		loc_reg=ptrvalve->status_reg;
		break;
		case ERRORCLOSE2:
		ptrvalve->status_reg=0x0E;											//ошибка закрытия
		ptrtimer->FlagSetTimer=0;
		loc_reg=ptrvalve->status_reg;
		break;	
		case ERROROPEN3:
		ptrvalve->status_reg=0x0D;											//ошибка открытия
		ptrtimer->FlagSetTimer=0;
		loc_reg=ptrvalve->status_reg;
		break;	
		case ERRORCLOSE3:
		ptrvalve->status_reg=0x0E;											//ошибка закрытия
		ptrtimer->FlagSetTimer=0;
		loc_reg=ptrvalve->status_reg;
		break;				
		default:
		ptrvalve->status_reg=loc_reg;
		ptrvalve->inouts.bit4=0;
		break;
	}
	
}