#include "pki.h"
#include "algsep.h"

Meas_TypeDef Pki1_Meas;
Meas_TypeDef Pki2_Meas;
/*
Compare_TypeDef Pki1_Comp_Struct; 
Compare_TypeDef Pki2_Comp_Struct;*/

void vPki2ComparePhase_a(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Pki *ptrpki2 = ptrgen->pki+1;	
	Meas_TypeDef *meas = &Pki2_Meas;
	measure *pki2_meas = meas->meas_struct+nPHASE_A;
	
	if((ptrpki2->phase_a>(PHASE_NORM_PKI2+LIMIT_PHASE_NORM_PKI2)) | (ptrpki2->phase_a<(PHASE_NORM_PKI2-LIMIT_PHASE_NORM_PKI2))){
		pki2_meas->qmeas=1;
	}else {
		pki2_meas->qmeas=0;
		ptrpki2->inouts.bit0=0;
		obj_septimers[FIRSTTIMER].InsideCntTimer=0;
	}
	
	user_timer_with_step_two_sec_obj(pki2_meas->qmeas,FIRSTTIMER,3);   
	if(obj_septimers[FIRSTTIMER].FlagSetTimer)
	{
		ptrpki2->inouts.bit0=1;
	}
}

void vPki2ComparePhase_b(void)
{
	
	BlockGenerators *ptrgen = &Sysep.generators;
	Pki *ptrpki2 = ptrgen->pki+1;	
	Meas_TypeDef *meas = &Pki2_Meas;
	measure *pki2_meas = meas->meas_struct+nPHASE_B;
	
	if((ptrpki2->phase_b>(PHASE_NORM_PKI2+LIMIT_PHASE_NORM_PKI2)) | (ptrpki2->phase_b<(PHASE_NORM_PKI2-LIMIT_PHASE_NORM_PKI2))){
		pki2_meas->qmeas=1;																											
	}else {
		pki2_meas->qmeas=0;
		ptrpki2->inouts.bit1=0;
		obj_septimers[SECONDTIMER].InsideCntTimer=0;
	}

	user_timer_with_step_two_sec_obj(pki2_meas->qmeas,SECONDTIMER,3);
	if(obj_septimers[SECONDTIMER].FlagSetTimer)
	{
		ptrpki2->inouts.bit1=1;
	}
}

void vPki2ComparePhase_c(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Pki *ptrpki2 = ptrgen->pki+1;
	Meas_TypeDef *meas = &Pki2_Meas;
	measure *pki2_meas = meas->meas_struct+nPHASE_C;
	
	if((ptrpki2->phase_c>(PHASE_NORM_PKI2+LIMIT_PHASE_NORM_PKI2)) | (ptrpki2->phase_c<(PHASE_NORM_PKI2-LIMIT_PHASE_NORM_PKI2))){
		pki2_meas->qmeas=1;																											
	}else {
		pki2_meas->qmeas=0;
		ptrpki2->inouts.bit2=0;
		obj_septimers[THIRDTIMER].InsideCntTimer=0;
	}
	
	user_timer_with_step_two_sec_obj(pki2_meas->qmeas,THIRDTIMER,3);
	if(obj_septimers[THIRDTIMER].FlagSetTimer)
	{
		ptrpki2->inouts.bit2=1;
	}
}

void vPki2CompareFreq(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Pki *ptrpki2 = ptrgen->pki+1;	
	Meas_TypeDef *meas = &Pki2_Meas;
	measure *pki2_meas = meas->meas_struct+nFREQ;
	
	if((ptrpki2->freq>(FREQ_NORM_PKI2+LIMIT_FREQ_NORM_PKI2)) | (ptrpki2->freq < (FREQ_NORM_PKI2-LIMIT_FREQ_NORM_PKI2))){
		pki2_meas->qmeas=1;																											
	}else {
		pki2_meas->qmeas=0;
		ptrpki2->inouts.bit3=0;
		obj_septimers[FOURTIMER].InsideCntTimer=0;
	}
	
	user_timer_with_step_two_sec_obj(pki2_meas->qmeas,FOURTIMER,3);
	if(obj_septimers[FOURTIMER].FlagSetTimer)
	{
		ptrpki2->inouts.bit3=1;
	}
}

void vPki2MCalcPhase_a(void)
{	
	
	vPki2ComparePhase_a();
	
}


void vPki2MCalcPhase_b(void)
{

	vPki2ComparePhase_b();
	
}

void vPki2MCalcPhase_c(void)
{
	
	vPki2ComparePhase_c();
	
}

void vPki2MCalcFreq(void)
{
	
	vPki2CompareFreq();
	
}

void vPki2MainCalc(void)
{
	vPki2MCalcPhase_a();
	vPki2MCalcPhase_b();
	vPki2MCalcPhase_c();
	vPki2MCalcFreq();
}

void vPki1ComparePhase_a(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Pki *ptrpki1 = &ptrgen->pki[0];	
	Meas_TypeDef *meas = &Pki1_Meas;
	measure *pki1_meas = meas->meas_struct+nPHASE_A;
	
	if((ptrpki1->phase_a>(PHASE_NORM_PKI2+LIMIT_PHASE_NORM_PKI2)) | (ptrpki1->phase_a<(PHASE_NORM_PKI2-LIMIT_PHASE_NORM_PKI2))){
		pki1_meas->qmeas=1;																											
	}else {
		pki1_meas->qmeas=0;
		ptrpki1->inouts.bit0=0;
		obj_septimers[FIFTHTIMER].InsideCntTimer=0;
	}
	
	user_timer_with_step_two_sec_obj(pki1_meas->qmeas,FIFTHTIMER,3);
	if(obj_septimers[FIFTHTIMER].FlagSetTimer)
	{
		ptrpki1->inouts.bit0=1;
	}
}


void vPki1ComparePhase_b(void)
{	
	BlockGenerators *ptrgen = &Sysep.generators;
	Pki *ptrpki1 = &ptrgen->pki[0];	
	Meas_TypeDef *meas = &Pki1_Meas;
	measure *pki1_meas = meas->meas_struct+nPHASE_B;	
	
	if((ptrpki1->phase_b>(PHASE_NORM_PKI2+LIMIT_PHASE_NORM_PKI2)) | (ptrpki1->phase_b<(PHASE_NORM_PKI2-LIMIT_PHASE_NORM_PKI2))){
		pki1_meas->qmeas=1;																											
	}else {
		pki1_meas->qmeas=0;
		ptrpki1->inouts.bit1=0;
		obj_septimers[SIXTHTIMER].InsideCntTimer=0;
	}
	
	user_timer_with_step_two_sec_obj(pki1_meas->qmeas,SIXTHTIMER,3);
	if(obj_septimers[SIXTHTIMER].FlagSetTimer)
	{
		ptrpki1->inouts.bit1=1;
	}
}

void vPki1ComparePhase_c(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Pki *ptrpki1 = &ptrgen->pki[0];	
	Meas_TypeDef *meas = &Pki1_Meas;
	measure *pki1_meas = meas->meas_struct+nPHASE_C;	
	
	if((ptrpki1->phase_c>(PHASE_NORM_PKI2+LIMIT_PHASE_NORM_PKI2)) | (ptrpki1->phase_c<(PHASE_NORM_PKI2-LIMIT_PHASE_NORM_PKI2))){
		pki1_meas->qmeas=1;																											
	}else {
		pki1_meas->qmeas=0;
		ptrpki1->inouts.bit2=0;
		obj_septimers[SEVENTHTIMER].InsideCntTimer=0;
	}
	
	user_timer_with_step_two_sec_obj(pki1_meas->qmeas,SEVENTHTIMER,3);
	if(obj_septimers[SEVENTHTIMER].FlagSetTimer)
	{
		ptrpki1->inouts.bit2=1;
	}
}

void vPki1CompareFreq(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Pki *ptrpki1 = &ptrgen->pki[0];	
	Meas_TypeDef *meas = &Pki1_Meas;
	measure *pki1_meas = meas->meas_struct+nFREQ;	
	
	if((ptrpki1->freq>(FREQ_NORM_PKI1+LIMIT_FREQ_NORM_PKI1)) | (ptrpki1->freq < (FREQ_NORM_PKI1-LIMIT_FREQ_NORM_PKI1)))
	{
		pki1_meas->qmeas=1;
	}else 
	{
		pki1_meas->qmeas=0;
		ptrpki1->inouts.bit3=0;
		obj_septimers[EIGHTTIMER].InsideCntTimer=0;
	}
	
	user_timer_with_step_two_sec_obj(pki1_meas->qmeas,EIGHTTIMER,3);
	if(obj_septimers[EIGHTTIMER].FlagSetTimer)
	{
		ptrpki1->inouts.bit3=1;
	}
}
void vPki1MCalcPhase_a(void)
{
	
	vPki1ComparePhase_a();

}


void vPki1MCalcPhase_b(void)
{
	
	vPki1ComparePhase_b();
	
}

void vPki1MCalcPhase_c(void)
{
	
	vPki1ComparePhase_c();
		
}

void vPki1MCalcFreq(void)
{
	
	vPki1CompareFreq();
	
}

void vPki1MainCalc(void)
{
	vPki1MCalcPhase_a();
	vPki1MCalcPhase_b();
	vPki1MCalcPhase_c();
	vPki1MCalcFreq();
}

void pki_reset_streg(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	//Blocks *ptrgen2 = &ptrgen->generator[1];
	Pki *ptrpki1 = &ptrgen->pki[0];
	Pki *ptrpki2 = &ptrgen->pki[1];
	uint8_t *local_streg_pki1 = (uint8_t *)&ptrpki1->status_reg;
	uint8_t *local_streg_pki2 = (uint8_t *)&ptrpki2->status_reg;
	
	//if(!ptrgen2->inouts.bit4) 
	{
		(*local_streg_pki1) = 0x00;
		(*local_streg_pki2) = 0x00;
	}
}

uint8_t status_pki_need_clear=0;
void pki_control_parametrs(void)
{
	//Sep *ptrsep = &Sysep;
	ExternalPower *ptrpower = &Extpower;
	BlockGenerators *ptrgen = &Sysep.generators;
	Pki *ptrpki2 = ptrgen->pki+1;
	Blocks *ptrgen2 = ptrgen->generator+1;
	static uint16_t cnt_cntrl_g2=0;								//задержка на контроль параметров пки
	
	if(!ptrgen2->inouts.bit4) {
		cnt_cntrl_g2=QUANTITY_OF_CYCLES_IS_0;										//сбрасываем счетчик
		return;														//контролируем по включенному Г2
	}
	
	if(++cnt_cntrl_g2<QUANTITY_OF_CYCLES10000) return;										//если Г2 -> контролируем пки2 (220) если Г2 и нет ВИП -> оба ПКИ
	cnt_cntrl_g2=QUANTITY_OF_CYCLES10000;
	if(status_pki_need_clear==NEEDCLEAR)
	{
		status_pki_need_clear=CLEANED;
		pki_reset_streg();
	}
	
	if(ptrpki2->freq<NOINMIND_FREQ_PKI2){								//если частота ПКИ2 не превышает 800 то контролируем параметры ПКИ2
		vPki2MainCalc();
	}
	
	if(ptrpower->inouts.bit2) return;							//если ВИП вкл -> то не контролируем Г1
		vPki1MainCalc();	
}

void pki2_state(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Pki *ptrpki2 = ptrgen->pki+1;
	//StatusReg *ptrpki2_cmd = &PKI2_cmd_OFF;
	
	if(ptrpki2->inouts.bit0)
	{
		//ptrpki2_cmd->bit0=1;
		ptrpki2->status_reg.bit0=1;										//фаза A за пределами 11В более 1сек. или за пределами 33В
		ptrpki2->inouts.bit0=0;
		obj_septimers[FIRSTTIMER].FlagSetTimer=0;
		obj_septimers[FIRSTTIMER].InsideCntTimer=0;
	}
	
	if(ptrpki2->inouts.bit1)
	{
		//ptrpki2_cmd->bit1=1;
		ptrpki2->status_reg.bit1=1;										//фаза B за пределами 11В более 1сек. или за пределами 33В
		ptrpki2->inouts.bit1=0;
		obj_septimers[SECONDTIMER].FlagSetTimer=0;
		obj_septimers[SECONDTIMER].InsideCntTimer=0;		
	}
	
	if(ptrpki2->inouts.bit2)
	{
		//ptrpki2_cmd->bit2=1;
		ptrpki2->status_reg.bit2=1;										//фаза C за пределами 11В более 1сек. или за пределами 33В
		ptrpki2->inouts.bit2=0;
		obj_septimers[THIRDTIMER].FlagSetTimer=0;
		obj_septimers[THIRDTIMER].InsideCntTimer=0;		
	}
	
	if(ptrpki2->inouts.bit3)
	{
		//ptrpki2_cmd->bit3=1;
		ptrpki2->status_reg.bit3=1;										//частота за пределами 16Гц более 1сек. или за пределами 32Гц
		ptrpki2->inouts.bit3=0;
		obj_septimers[FOURTIMER].FlagSetTimer=0;
		obj_septimers[FOURTIMER].InsideCntTimer=0;		
	}	
}

void pki1_state(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Pki *ptrpki1 = ptrgen->pki+0;
	//StatusReg *ptrpki1_cmd = &PKI1_cmd_OFF;
	
	if(ptrpki1->inouts.bit0)
	{
		//ptrpki1_cmd->bit0=1;
		ptrpki1->status_reg.bit0=1;										//фаза A за пределами 11В более 1сек. или за пределами 33В
		ptrpki1->inouts.bit0=0;
		obj_septimers[FIFTHTIMER].FlagSetTimer=0;
		obj_septimers[FIFTHTIMER].InsideCntTimer=0;		
	}
	
	if(ptrpki1->inouts.bit1)
	{
		//ptrpki1_cmd->bit1=1;
		ptrpki1->status_reg.bit1=1;										//фаза B за пределами 11В более 1сек. или за пределами 33В
		ptrpki1->inouts.bit1=0;
		obj_septimers[SIXTHTIMER].FlagSetTimer=0;
		obj_septimers[SIXTHTIMER].InsideCntTimer=0;		
	}
	
	if(ptrpki1->inouts.bit2)
	{
		//ptrpki1_cmd->bit2=1;
		ptrpki1->status_reg.bit2=1;										//фаза C за пределами 11В более 1сек. или за пределами 33В
		ptrpki1->inouts.bit2=0;
		obj_septimers[SEVENTHTIMER].FlagSetTimer=0;
		obj_septimers[SEVENTHTIMER].InsideCntTimer=0;		
	}
	
	if(ptrpki1->inouts.bit3)
	{
		//ptrpki1_cmd->bit3=1;
		ptrpki1->status_reg.bit3=1;										//частота за пределами 16Гц более 1сек. или за пределами 32Гц
		ptrpki1->inouts.bit3=0;
		obj_septimers[EIGHTTIMER].FlagSetTimer=0;
		obj_septimers[EIGHTTIMER].InsideCntTimer=0;		
	}
}

void pki_state(void)
{
	//pki_reset_streg();
	pki2_state();
	pki1_state();
}