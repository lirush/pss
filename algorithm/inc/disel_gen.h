#ifndef DISEL_GEN_H_
#define DISEL_GEN_H_

enum ListInsBlockGen{
	START=0x01,
	STOPP=0x02,
	ONLOAD=0x04,
	OFFLOAD=0x08,
	FEEDBACKONGEN=0x11,
	FEEDBACK0ONGEN=0x10,
	FEEDBACK2ONGEN=0x14,	
	FEEDBACK3ONGEN=0x54,
	FEEDBACK4ONGEN=0x50,
	FEEDBACKOFFGEN=0x22,
	FEEDBACK0OFFGEN=0x20,
	FEEDBACK2OFFGEN=0x28,
	FEEDBACK3OFFGEN=0x68,
	FEEDBACKONLOAD=0x44,
	FEEDBACKONLOAD1=0x54,
	FEEDBACKOFFLOAD=0x88,
	FEEDBACKOFFLOAD1=0xA8,
	FEEDBACKERRCONNECT=0x64,
	FEEDBACKERRDISCONNECT=0x58
};

enum ListStateBlockGen{
	STARTING = 0x01,
	STOPING = 0x02,
	STARTED = 0x11,
	STOPED = 0x22,
	LOADING=0x04,
	UNLOADING=0x08,
	LOADED=0x44,
	UNLOADED=0x88
};

void disel_gen_start(void);
void disel_gen_stop(void);
void disel_gen_set_load(void);
void disel_gen_reset_load(void);
void disel_gen_set_unload(void);
void disel_gen_reset_out_start(void);
void disel_gen_reset_out_stop(void);
void block_gen_state(void);
/*
void state_gen1(void);
void state_gen2(void);*/
void disel_gens_stop(void);
void disel_gens_start(void);
void gens_state(void);
void gens_reset_pa1(void);

#include ".\algorithm\src\disel_gen.c"

#endif