#include "disel_gen.h"
#include "algsep.h"

set_of_inputs ctrl_disel_gen_start={&PINC,PORTC1};		//пуск д-г (Г1)
set_of_inputs ctrl_disel_gen_stop={&PIND,PORTD6};		//стоп д-г (Г1)
set_of_inputs ctrl_disel_gen_load={&PINC,PORTC3};		//включить нагрузку д-г (Г2)
set_of_inputs ctrl_disel_gen_unload={&PINB,PORTB6};	    //отключить нагрузку д-г (Г2)
set_of_inputs ctrl_block_gens_start={&PINA,PORTA1};	    //включение генераторов в режиме дизеля
set_of_inputs ctrl_block_gens_stop={&PORTD,PORTD1};	    //выключение генераторов в режиме дизеля	
	
//long cnt_keep_stopdg=0;

void disel_gens_start(void)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //если ручной режим -> управление ключом запрещено
	port_set_out(ctrl_block_gens_start);
}

void disel_gens_stop(void)
{
    ByteData *ptrbtn2 = &btn2;
    
    if(!ptrbtn2->bit0)return;                                  //если ручной режим -> управление ключом запрещено
	port_unset_out(ctrl_block_gens_start);
}

void disel_gen_start(void)
{
	port_unset_out(ctrl_disel_gen_stop);
	port_set_out(ctrl_disel_gen_start);
}

void disel_gen_stop(void)
{
	port_unset_out(ctrl_disel_gen_start);
	port_set_out(ctrl_disel_gen_stop);
}

void disel_gen_reset_out_start(void)
{
	port_unset_out(ctrl_disel_gen_start);
}

void disel_gen_reset_out_stop(void)
{
	port_unset_out(ctrl_disel_gen_stop);
}

void disel_gen_set_load(void)
{
	port_set_out(ctrl_disel_gen_load);
}

void disel_gen_reset_load(void)
{
	port_unset_out(ctrl_disel_gen_load);
}

void disel_gen_set_unload(void)
{
	port_set_out(ctrl_disel_gen_unload);
}

void disel_gen_reset_unload(void)
{
	port_unset_out(ctrl_disel_gen_unload);
}

void block_gen_ctrl_ins(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	
	ptrgen->inouts.bit0 = port_state_for_outs(ctrl_disel_gen_start);		//старт Д-Г
	ptrgen->inouts.bit1 = port_state_for_outs(ctrl_disel_gen_stop);			//стоп Д-Г
	ptrgen->inouts.bit2 = port_state_for_outs(ctrl_disel_gen_load);			//вкл. нагрузку на Г2
	ptrgen->inouts.bit3 = port_state_for_outs(ctrl_disel_gen_unload);		//выкл. нагрузку от Г2
	user_counter(ptrgen->inouts.bit2,SECONDCNT,4);							//счетчик = 4*100
	user_counter(ptrgen->inouts.bit3,THIRDCNT,4);							//счетчик = 4*100
	ptrgen->inouts.bit6 = sepcnt[SECONDCNT].FlagSetTimer;
	ptrgen->inouts.bit7 = sepcnt[THIRDCNT].FlagSetTimer;
}

extern uint8_t flag_keep_stopdg;
void disel_gen_reset_stop(void)												//сброс ключа стоп Д-Г
{
	user_counter(port_state_for_outs(ctrl_disel_gen_stop),NINETEENTH,8);	//счетчик = 8*100
	if(sepcnt[NINETEENTH].FlagSetTimer){
		sepcnt[NINETEENTH].FlagSetTimer=0;
		disel_gen_reset_out_stop();
	}
}

void block_gen_state(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	uint8_t *ptr = (uint8_t *)&ptrgen->inouts;
	static uint8_t loc_reg=0;
	
	block_gen_ctrl_ins();
	switch(*ptr){
		case START:
			loc_reg=ptrgen->status_reg_extpw=STARTING;
		break;
		case STOPP:
			loc_reg=ptrgen->status_reg_extpw=STOPING;
		break;
		case ONLOAD:
			loc_reg=ptrgen->status_reg_extpw=LOADING;
		break;
		case OFFLOAD:
			loc_reg=ptrgen->status_reg_extpw=UNLOADING;
		break;
		case FEEDBACKONGEN:
			disel_gen_reset_out_start();
			loc_reg=ptrgen->status_reg_extpw=STARTED;
		break;
		case FEEDBACKOFFGEN:
			//disel_gen_reset_out_stop();
			loc_reg=ptrgen->status_reg_extpw=STOPED;
		break;
		case FEEDBACKONLOAD:
			disel_gen_reset_load();
			loc_reg=ptrgen->status_reg_extpw=LOADED;
			sepcnt[SECONDCNT].FlagSetTimer=0;
		break;
		case FEEDBACKONLOAD1:
			disel_gen_reset_load();
			loc_reg=ptrgen->status_reg_extpw=LOADED;
			sepcnt[SECONDCNT].FlagSetTimer=0;
		break;		
		case FEEDBACKOFFLOAD:
			disel_gen_reset_unload();
			loc_reg=ptrgen->status_reg_extpw=UNLOADED;
			sepcnt[THIRDCNT].FlagSetTimer=0;
		break;
		case FEEDBACKOFFLOAD1:
			disel_gen_reset_unload();
			loc_reg=ptrgen->status_reg_extpw=UNLOADED;
			sepcnt[THIRDCNT].FlagSetTimer=0;
		break;		
		case 0x2A:
			//disel_gen_reset_out_stop();
			loc_reg=ptrgen->status_reg_extpw=STOPED;
		break;		
		case 0xAA:
			//disel_gen_reset_out_stop();
			loc_reg=ptrgen->status_reg_extpw=STOPED;
		break;
		case 0x28:
			disel_gen_reset_unload();
			loc_reg=ptrgen->status_reg_extpw=UNLOADED;
			sepcnt[THIRDCNT].FlagSetTimer=0;
		break;
		case 0x9A:
		disel_gen_reset_unload();
		loc_reg=ptrgen->status_reg_extpw=UNLOADED;
		sepcnt[THIRDCNT].FlagSetTimer=0;
		break;		
		default:
			ptrgen->status_reg_extpw=loc_reg;
		break;
	}
	disel_gen_reset_stop();
}

void gen1_ctrl_ins(void)
 {
	BlockGenerators *ptrgen = &Sysep.generators;
	Blocks *ptrgen1 = &ptrgen->generator[0];
	
	if(port_state_for_outs(ctrl_disel_gen_start) | port_state_for_outs(ctrl_disel_gen_load))
	ptrgen1->inouts.bit2 = 1;
	if(port_state_for_outs(ctrl_disel_gen_stop) | port_state_for_outs(ctrl_disel_gen_unload))
	ptrgen1->inouts.bit3 = 1;
	user_counter(ptrgen1->inouts.bit2 | ptrgen1->inouts.bit3,FOURCNT,10);						//счетчик = 10*100
	if(sepcnt[FOURCNT].FlagSetTimer){
		ptrgen1->inouts.bit6=sepcnt[FOURCNT].FlagSetTimer;
		sepcnt[FOURCNT].FlagSetTimer=0;
	}
 }
 
void state_gen1(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Blocks *ptrgen1 = &ptrgen->generator[0];
	uint8_t *ptr = (uint8_t *)&ptrgen1->inouts;
	static uint8_t loc_reg=0;
	
	gen1_ctrl_ins();
	switch(*ptr) {	
		case FEEDBACK2ONGEN:
		ptrgen1->status_reg=loc_reg=ISOPEN;
		ptrgen1->inouts.bit2 = 0;
		break;
		case FEEDBACK0ONGEN:
		ptrgen1->status_reg=loc_reg=ISOPEN;
		ptrgen1->inouts.bit2 = 0;
		break;		
		case FEEDBACK3ONGEN:
		ptrgen1->status_reg=loc_reg=ISOPEN;
		ptrgen1->inouts.bit2 = 0;
		break;
		case FEEDBACKONLOAD:
		ptrgen1->status_reg=loc_reg=ERRORCONNECT;
		ptrgen1->inouts.bit2 = 0;
		break;
		case FEEDBACK2OFFGEN:
		ptrgen1->status_reg=loc_reg=ISCLOSE;
		ptrgen1->inouts.bit3 = 0;
		break;
		case FEEDBACK0OFFGEN:
		ptrgen1->status_reg=loc_reg=ISCLOSE;
		ptrgen1->inouts.bit3 = 0;
		break;		
		case FEEDBACK3OFFGEN:
		ptrgen1->status_reg=loc_reg=ISCLOSE;
		ptrgen1->inouts.bit3 = 0;
		break;
		case FEEDBACKERRDISCONNECT:
		ptrgen1->status_reg=loc_reg=ERRORDISCONNECT;
		ptrgen1->inouts.bit3 = 0;
		break;
		default:
		ptrgen1->status_reg=loc_reg;
		ptrgen1->inouts.bit6=0;
		break;
	}
}

void cntrl_g1_by_g2_reset_bit2(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Blocks *ptrgen1 = &ptrgen->generator[0];
	Blocks *ptrgen2 = &ptrgen->generator[1];	
	ExternalPower *ptrpower = &Extpower;
	
	if(ptrpower->inouts.bit2){
		ptrgen1->status_reg=ptrgen2->status_reg;
		ptrgen1->inouts.bit2=0;
	}	
}

void cntrl_g1_by_g2_reset_bit3(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Blocks *ptrgen1 = &ptrgen->generator[0];
	Blocks *ptrgen2 = &ptrgen->generator[1];
	ExternalPower *ptrpower = &Extpower;
	
	if(ptrpower->inouts.bit2){
		ptrgen1->status_reg=ptrgen2->status_reg;
		ptrgen1->inouts.bit3=0;
	}
}

 void gen2_ctrl_ins(void)
 {
	BlockGenerators *ptrgen = &Sysep.generators;
	Blocks *ptrgen2 = &ptrgen->generator[1];

	if(port_state_for_outs(ctrl_disel_gen_load))
	ptrgen2->inouts.bit2=1;
	if(port_state_for_outs(ctrl_disel_gen_unload))
	ptrgen2->inouts.bit3=1;
	user_counter(ptrgen2->inouts.bit2,FIFTHCNT,10);					//счетчик = 10*100
	if(sepcnt[FIFTHCNT].FlagSetTimer){
		ptrgen2->inouts.bit6=sepcnt[FIFTHCNT].FlagSetTimer;
		sepcnt[FIFTHCNT].FlagSetTimer=0;
	}
 }

void state_gen2(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Blocks *ptrgen1 = &ptrgen->generator[0];
	Blocks *ptrgen2 = &ptrgen->generator[1];
	uint8_t *ptr = (uint8_t *)&ptrgen2->inouts;
	static uint8_t loc_reg=0;
	ExternalPower *ptrpower = &Extpower;
	
	gen2_ctrl_ins();
	switch(*ptr) {
		case FEEDBACK2ONGEN:
		ptrgen2->status_reg=loc_reg=ISOPEN;
		cntrl_g1_by_g2_reset_bit2();
		ptrgen2->inouts.bit2=0;
		break;
		case FEEDBACK0ONGEN:
		ptrgen2->status_reg=loc_reg=ISOPEN;	
		cntrl_g1_by_g2_reset_bit2();
		ptrgen2->inouts.bit2=0;		
		break;		
		case FEEDBACK3ONGEN:
		ptrgen2->status_reg=loc_reg=ISOPEN;	
		cntrl_g1_by_g2_reset_bit2();
		ptrgen2->inouts.bit2=0;			
		break;	
		case FEEDBACKERRCONNECT:
		ptrgen2->status_reg=loc_reg=ERRORCONNECT;
		sepcnt[FIFTHCNT].FlagSetTimer=0;	
		cntrl_g1_by_g2_reset_bit2();
		ptrgen2->inouts.bit2=0;		
		break;
		case FEEDBACK2OFFGEN:
		ptrgen2->status_reg=loc_reg=ISCLOSE;
		cntrl_g1_by_g2_reset_bit3();
		ptrgen2->inouts.bit3=0;		
		break;
		case FEEDBACK0OFFGEN:
		ptrgen2->status_reg=loc_reg=ISCLOSE;	
		cntrl_g1_by_g2_reset_bit3();
		ptrgen2->inouts.bit3=0;		
		break;
		case FEEDBACK3OFFGEN:
		ptrgen2->status_reg=loc_reg=ISCLOSE;	
		cntrl_g1_by_g2_reset_bit3();
		ptrgen2->inouts.bit3=0;		
		break;
		case FEEDBACKERRDISCONNECT:
		ptrgen2->status_reg=loc_reg=ERRORDISCONNECT;
		sepcnt[FIFTHCNT].FlagSetTimer=0;
		cntrl_g1_by_g2_reset_bit3();
		ptrgen2->inouts.bit3=0;			
		break;
		default:
		ptrgen2->status_reg=loc_reg;
		if(ptrpower->inouts.bit2)
		ptrgen1->status_reg=ptrgen2->status_reg;
		ptrgen2->inouts.bit6=0;
		break;
	}
}

void gens_state(void)
{
	ExternalPower *ptrpower = &Extpower;
	state_gen2();
	if(ptrpower->inouts.bit2) return;
	state_gen1();
}

void gens_reset_pa1(void)
{
	user_counter(port_state_for_outs(ctrl_block_gens_start),FOURTEENTH,4);					//~200мс //счетчик = 4*100
	if(sepcnt[FOURTEENTH].FlagSetTimer){
		sepcnt[FOURTEENTH].FlagSetTimer=0;
		disel_gens_stop();																	//снимаем коммутацию (не саму коммутация, а сигнал с ключа, схематично присутствует самоподхват
	}
}