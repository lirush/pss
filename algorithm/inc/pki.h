#ifndef PKI_H_
#define PKI_H_

#define LIMIT_PHASE_NORM_PKI2		(34)
#define LIMIT_PHASE_ALARM_PKI2		(34)
#define LIMIT_FREQ_NORM_PKI2		(160)
#define LIMIT_FREQ_ALARM_PKI2		(320)
#define NOINMIND_FREQ_PKI2			(8000)
#define PHASE_NORM_PKI2				(220)
#define FREQ_NORM_PKI2				(4000)
#define LIMIT_FREQ_NORM_PKI1		(25)
#define LIMIT_FREQ_ALARM_PKI1		(40)
#define FREQ_NORM_PKI1				(500)
#define MEAS_FILTER_SHIFT			(2)

#define NEEDCLEAR					(1)
#define CLEANED						(2)

enum ListStatePki{
	ALARMPKIPHASE_A = 0x11,
	ALARMPKIPHASE_B = 0x22,
	ALARMPKIPHASE_C = 0x44,
	ALARMPKIFREQ = 0x88
	};
	
enum ListPhases{
	nPHASE_A,
	nPHASE_B,
	nPHASE_C,
	nFREQ
};
	
typedef struct {
	uint8_t qmeas;
	//uint32_t accum_meas;
	uint8_t meas;
}measure;

typedef struct{
	measure meas_struct[4];
}Meas_TypeDef;

/*
typedef struct{
	measure meas_struct[4];
}Compare_TypeDef;*/

//extern Meas_TypeDef Pki2_Meas;
extern uint8_t status_pki_need_clear;

void pki_control_parametrs(void);
void pki_state(void);

#include ".\algorithm\src\pki.c"

#endif /* PKI_H_ */