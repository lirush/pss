#include "scepl.h"
#include "algsep.h"

set_of_inputs ctrl_scepl_on={&PINC,PORTC5};					//разомкнуть
set_of_inputs ctrl_scepl_off={&PINC,PORTC7};					//сомкнуть

void scepl_on(void)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //если ручной режим -> управление ключом запрещено
	port_unset_out(ctrl_scepl_off);
	port_set_out(ctrl_scepl_on);
}

void scepl_off(void)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //если ручной режим -> управление ключом запрещено
	port_unset_out(ctrl_scepl_on);
	port_set_out(ctrl_scepl_off);	
}

void scepl_ctrl_ins(void)
{
	Blocks *ptrscepl = &Sysep.disel.scepl;
	ByteData *ptrbtn1 = &btn1;
	ByteData *ptrbtn2 = &btn2;
	Septimers *ptrtimer = septimers+FOURTIMER;
    
	ptrscepl->inouts.bit0 = port_state_for_outs(ctrl_scepl_off) | (ptrbtn1->bit7 & (!ptrbtn2->bit0));			//сомкнуть; состояние порта выхода или состояние переключателя SA5		
	ptrscepl->inouts.bit1 = port_state_for_outs(ctrl_scepl_on) | (ptrbtn1->bit6 & (!ptrbtn2->bit0));
	
	if(ptrscepl->inouts.bit0 | ptrscepl->inouts.bit1)
	{
		user_timer_with_step_one_sec(1,FOURTIMER,20);									//ждем 46-2 сек. и ставим аварию на открытия заслонки/закрытия
	}else
	{
		ptrtimer->InsideCntTimer=0;
	}	
	
//	user_timer_with_step_one_sec((ptrscepl->inouts.bit0 | ptrscepl->inouts.bit1),FOURTIMER,20);		//ждем 20 сек. и ставим аварию на вкл/выкл сцепления
	if(ptrtimer->FlagSetTimer) {
		ptrscepl->inouts.bit4 = ptrtimer->FlagSetTimer;
		ptrtimer->FlagSetTimer=0;
	}																																
}

void scepl_state(void)
{
	Blocks *ptrscepl = &Sysep.disel.scepl;
    uint8_t *ptr = (uint8_t *)&ptrscepl->inouts;
	static uint8_t loc_reg=0;
	Septimers *ptrtimer = septimers+FOURTIMER;
	scepl_ctrl_ins();
	switch(*ptr) {
		case 0:
		if((loc_reg==0x01) || (loc_reg==0x02))
		loc_reg=ptrscepl->status_reg=0x00;
		break;
		case OPENING:
		ptrscepl->status_reg=0x01;
		loc_reg=ptrscepl->status_reg;
		break;	
		case 0x09:
		ptrscepl->status_reg=0x01;
		loc_reg=ptrscepl->status_reg;
		break;			
		case CLOSING:
		ptrscepl->status_reg=0x02;
		loc_reg=ptrscepl->status_reg;
		break;
		case 0x06:
		ptrscepl->status_reg=0x02;
		loc_reg=ptrscepl->status_reg;
		break;		
		case ISOPEN:
		ptrscepl->status_reg=0x04;											//замкнуто сцепление
		port_unset_out(ctrl_scepl_off);
		loc_reg=ptrscepl->status_reg;
		break;	
		case ISCLOSE:
		ptrscepl->status_reg=0x08;											//разомкнуто сцепление
		port_unset_out(ctrl_scepl_on);
		loc_reg=ptrscepl->status_reg;
		break;				
		case ERRORCONNECT:
		ptrscepl->status_reg=0x0C;											//ошибка подключения
		break;
		case ERROROPEN1:
		ptrscepl->status_reg=0x0D;											//ошибка включения
		ptrtimer->FlagSetTimer=0;
		break;
		case ERRORCLOSE1:
		ptrscepl->status_reg=0x0E;											//ошибка выключения
		ptrtimer->FlagSetTimer=0;
		break;
		case ERROROPEN2:
		ptrscepl->status_reg=0x0D;											//ошибка включения
		ptrtimer->FlagSetTimer=0;
		break;
		case ERRORCLOSE2:
		ptrscepl->status_reg=0x0E;											//ошибка выключения
		ptrtimer->FlagSetTimer=0;
		break;		
		case ERROROPEN3:
		ptrscepl->status_reg=0x0D;											//ошибка включения
		ptrtimer->FlagSetTimer=0;
		break;	
		case ERRORCLOSE3:
		ptrscepl->status_reg=0x0E;											//ошибка выключения
		ptrtimer->FlagSetTimer=0;
		break;			
		default:
		ptrscepl->status_reg=loc_reg;
		ptrscepl->inouts.bit4=0;
		break;
	}	
}