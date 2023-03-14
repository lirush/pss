#include "algsep.h"
#include "starter.h"
//#include "timers.h"
#include "pki.h"
#include "ebu.h"
#include "valve.h"
#include "scepl.h"
#include "disel_gen.h"
#include "fms.h"

BoardsNormal BoardNormal[4];
Sep Sysep;
ExternalPower Extpower;
ByteData btn1;
ByteData btn2;
ByteData btn3;
ByteData sensors1;
ByteData sensors2;
ByteData sensors3;
uint8_t OutDataForCantmp[8];

uint16_t press_oil_sep=0;
uint8_t pki1_norma=0;
uint8_t pki2_norma=0;
static uint8_t recharging=0;
uint8_t flag_keep_stopdg=0;
static uint8_t cnt_start_starter=0;
uint8_t pmv_remote_off=0;                       //отключение по аварии, команда от ПМВ
//uint8_t alarm=0;

void StartSep(void);
void OpenValveScepl(void);
void OnEBU(void);
void AlgStarter(void);
void DiselIs900(void);
void DiselIs1800(void);
void DiselOnGens(void);
void DiselGensIsOn(void);
void StopSep(void);
void DiselOffGens(void);
void DiselWorking5Min(void);
void DiselStop(void);
void ExtPwdCheckIndEP(void);
void ExtPwdOnGen(void);
void ExtPwdCheckSpeedUpWork(void);
void ExtPwdOnLoad(void);
void ExtPwdCheckG2(void);
void ExtPwdUnLoad(void);
void ExtPwdGenStop(void);
void ExtPwdGenIsStop(void);
void ExtPwdWorking30Sec(void);
void RechargingTimerStart(void);
void RechargingTimerStop(void);
void RechargingIsRecharge(void);
void RechargingErrorStart(void);
void RechargingIsStop(void);
void RechargingErrorStop(void);
void RechargingStopByKey(void);
void RechargingIsStopByKey(void);

static Transition transitionTable[28]={
    {AUTO_DIS_OPEN_VALVE_SCEPL,OpenValveScepl},
    {AUTO_DIS_ON_EBU,OnEBU},
    {AUTO_DIS_ALG_STARTER,AlgStarter},
    {AUTO_DIS_IS_SPEED900,DiselIs900},
    {AUTO_DIS_IS_SPEED1800,DiselIs1800},
    {AUTO_DIS_ON_GENS,DiselOnGens},
    {AUTO_DIS_GENS_IS_ON,DiselGensIsOn},
    {AUTO_DIS_OFF_GENS,DiselOffGens},
    {AUTO_DIS_WORKING_5MIN,DiselWorking5Min},
    {AUTO_DIS_STOP,DiselStop},
    {AUTO_EXTPWD_CHECK_INDEP,ExtPwdCheckIndEP},
    {AUTO_EXTPWD_OPEN_VALVE_SCEPL,OpenValveScepl},
    {AUTO_EXTPWD_ON_GEN,ExtPwdOnGen},
    {AUTO_EXTPWD_CHECK_SPEEDUP_WORK,ExtPwdCheckSpeedUpWork},
    {AUTO_EXTPWD_ON_LOAD,ExtPwdOnLoad},
    {AUTO_EXTPWD_CHECK_G2,ExtPwdCheckG2},
    {AUTO_EXTPWD_UNLOAD,ExtPwdUnLoad},
    {AUTO_EXTPWD_GEN_STOP,ExtPwdGenStop},
    {AUTO_EXTPWD_GEN_IS_STOP,ExtPwdGenIsStop},
    {AUTO_EXTPWD_WORKING_30SEC,ExtPwdWorking30Sec},
    {RECHARGING_TIMER_START,RechargingTimerStart},
    {RECHARGING_IS_RECHARGE,RechargingIsRecharge},
    {RECHARGING_TIMER_STOP,RechargingTimerStop},
    {RECHARGING_ERROR_START,RechargingErrorStart},
    {RECHARGING_IS_STOP,RechargingIsStop},
    {RECHARGING_STOP_BY_KEY,RechargingStopByKey},
    {RECHARGING_ERROR_STOP,RechargingErrorStop},
    {RECHARGING_IS_STOP_BY_KEY,RechargingIsStopByKey}
};
static uint8_t current_state = START_SEP_;
static uint8_t prev_state = START_SEP_;

set_of_inputs ctrl_sep_stop = {&PIND,PORTD1};

void set_ins(void)
{
    Disel *ptr = &Sysep.disel;
    BlockGenerators *ptrgen = &Sysep.generators;
    ExternalPower *ptrpower = &Extpower;
    Ebu *ptrebu = &ptr->ebu;
    Pgd *ptrpgd = &ptr->pgd;
    Pki *ptrpki1 = &ptrgen->pki[0];
	Pki *ptrpki2 = &ptrgen->pki[1];
    Blocks *ptrscepl = &ptr->scepl;
    Blocks *ptrstarter = &ptr->starter;
	Blocks *ptrvalve = &ptr->valve;
	Blocks *ptrgen1 = &ptrgen->generator[0];	
	Blocks *ptrgen2 = &ptrgen->generator[1];
	ByteData *ptrbtn1 = &btn1;
	ByteData *ptrbtn2 = &btn2;
	ByteData *ptrbtn3 = &btn3;
	ByteData *ptrsens1 = &sensors1;
	ByteData *ptrsens2 = &sensors2;
	ByteData *ptrsens3 = &sensors3;    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //                                          Uart пакет от PN1                                               //
    // _______________________________________________________________________________________________________  //
    // |    ПН1    |    ПН2    |    ПН3    |    ЭБУ    |    ПКИ1   |    ПКИ2   | БРИ (T,P) |ПМВ (remote cntl)|  //
    // | 3-и байта | 3-и байта | 3-и байта | 2-а байта | 8-мь байт | 8-мь байт | 4-е байта |    2-а байта    |  //
    // | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |  9  |  10 |  11 - 18  |  19 - 26  |  27 - 30  |   31   |   32   |  //
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ptrebu->read_engine_speed = (BoardNormal[PN1].In[9]) | (BoardNormal[PN1].In[10]<<8);    
    ptrpgd->temp_liq = (BoardNormal[PN1].In[27]) | (BoardNormal[PN1].In[28]<<8);       
    press_oil_sep = (BoardNormal[PN1].In[29]) | (BoardNormal[PN1].In[30]<<8);
    
    ptrpki1->phase_a = BoardNormal[PN1].In[11];
    ptrpki1->phase_b = BoardNormal[PN1].In[12];
	ptrpki1->phase_c = BoardNormal[PN1].In[13];
	ptrpki1->freq = (BoardNormal[PN1].In[14]) | (BoardNormal[PN1].In[15]<<8);
	ptrpki1->resist = (BoardNormal[PN1].In[16]) | (BoardNormal[PN1].In[17]<<8);
	
    ptrpki2->phase_a = BoardNormal[PN1].In[19];
    ptrpki2->phase_b = BoardNormal[PN1].In[20];
	ptrpki2->phase_c = BoardNormal[PN1].In[21];
	ptrpki2->freq = (BoardNormal[PN1].In[22]) | (BoardNormal[PN1].In[23]<<8);
	ptrpki2->resist = (BoardNormal[PN1].In[24]) | (BoardNormal[PN1].In[25]<<8);
    	
//	off_sep_from_pmv = (InDataFromCan[PMVOFFSEP].Data.bytezero);
                          
    ptrebu->alarm = (BoardNormal[PN1].In[6] & (1<<0))>>0;                           //АВАРИЯ ЭБУ
    ptrscepl->inouts.bit3 = (BoardNormal[PN1].In[1] & (1<<5))>>5;                    //СРАБАТЫВАНИЕ МЕХАНИЗМА СЦЕПЛЕНИЕ МП-10 ВКЛЮЧЕНО (разомкнуто)
    ptrscepl->inouts.bit2 = (BoardNormal[PN1].In[1] & (1<<4))>>4;                    //СРАБАТЫВАНИЕ МЕХАНИЗМА СЦЕПЛЕНИЕ МП-10 ВЫКЛЮЧЕНО (замкнуто)
    ptrstarter->inouts.bit2 = (BoardNormal[PN1].In[4] & (1<<0))>>0;                //Вход включения СТАРТЕРА диз. СЭП    
    ptrvalve->inouts.bit2 = (BoardNormal[PN1].In[4] & (1<<6))>>6;                    //СРАБАТЫВАНИЕ МЕХАНИЗМА ЗАСЛОНКИ ВОЗДУХООТВОДА МП-100 ЛЕВО - открыта
    ptrvalve->inouts.bit3 = (BoardNormal[PN1].In[4] & (1<<7))>>7;                    //СРАБАТЫВАНИЕ МЕХАНИЗМА ЗАСЛОНКИ ВОЗДУХООТВОДА МП-100 ПРАВО - закрыта
    ptrgen1->inouts.bit4 = (BoardNormal[PN1].In[7] & (1<<1))>>1;                    //ИНДИКАЦИЯ ВКЛЮЧЕНИЯ Г1
    ptrgen1->inouts.bit5 = !((BoardNormal[PN1].In[7] & (1<<1))>>1);                    //не ИНДИКАЦИЯ ВКЛЮЧЕНИЯ Г1
    ptrgen2->inouts.bit4 = (BoardNormal[PN1].In[7] & (1<<2))>>2;                    //ИНДИКАЦИЯ ВКЛЮЧЕНИЯ Г2
    ptrgen2->inouts.bit5 = !((BoardNormal[PN1].In[7] & (1<<2))>>2);                   //не ИНДИКАЦИЯ ВКЛЮЧЕНИЯ Г2
    ptrpower->inouts.bit2 = (BoardNormal[PN1].In[7] & (1<<3))>>3;                    //ИНДИКАЦИЯ ВКЛЮЧЕНИЯ ВИП
    ptrgen->inouts.bit4 = (BoardNormal[PN1].In[7] & (1<<2))>>2;                    //ИНДИКАЦИЯ ВКЛЮЧЕНИЯ Г2
    ptrgen->inouts.bit5 = !((BoardNormal[PN1].In[7] & (1<<2))>>2);                //не ИНДИКАЦИЯ ВКЛЮЧЕНИЯ Г2
    
    ptrbtn1->bit0 = (BoardNormal[PN1].In[0] & (1<<0))>>0;             //Вход состояния положения переключателя SA1(СТАРТЕР/ХХ/СТОП) СТОП
    ptrbtn1->bit1 = (BoardNormal[PN1].In[0] & (1<<1))>>1;             //Вход состояния положения переключателя SA1(СТАРТЕР/ХХ/СТОП) ХХ
    ptrbtn1->bit2 = (BoardNormal[PN1].In[0] & (1<<2))>>2;             //Вход состояния положения переключателя SA1(СТАРТЕР/ХХ/СТОП) СТАРТЕР
    ptrbtn1->bit3 = (BoardNormal[PN1].In[0] & (1<<3))>>3;             //Вход состояния положения тумблера SA2(РАБОТА ДИЗ. СЭП)
    ptrbtn1->bit4 = (BoardNormal[PN1].In[0] & (1<<4))>>4;             //Вход состояния положения переключателя SA4(заслонку открыть)
    ptrbtn1->bit5 = (BoardNormal[PN1].In[0] & (1<<5))>>5;             //Вход состояния положения переключателя SA4(заслонку закрыть)
    ptrbtn1->bit6 = (BoardNormal[PN1].In[0] & (1<<6))>>6;             //Вход состояния положения переключателя SA5(ВКЛЮЧЕНИЕ СЦЕПЛЕНИЯ/разомкнуть)
    ptrbtn1->bit7 = (BoardNormal[PN1].In[0] & (1<<7))>>7;             //Вход состояния положения переключателя SA5(ВЫКЛЮЧЕНИЕ СЦЕПЛЕНИЯ/замкнуть)
    ptrbtn2->bit0 = (BoardNormal[PN1].In[1] & (1<<0))>>0;             //Вход состояния положения тумблера SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП)
    ptrbtn2->bit1 = (BoardNormal[PN1].In[1] & (1<<1))>>1;             //Вход состояния нажатия кнопки SB1(ОТКЛ. СЭП) Н.З.
    
    if((BoardNormal[PN1].In[1] & (1<<2))) ptrbtn2->bit2=1;
    ptrbtn2->bit3 = (BoardNormal[PN1].In[1] & (1<<3))>>3;             //Вход состояния нажатия кнопки SB3(ВКЛ. ВИП) Н.Р.
    ptrbtn2->bit4 = (BoardNormal[PN1].In[1] & (1<<6))>>6;             //Вход состояния положения кнопки SB11(АВТ. СТАРТ СЭП)
    ptrbtn2->bit5 = (BoardNormal[PN1].In[1] & (1<<7))>>7;             //Вход состояния положения кнопки SB9(АВАРИЙНЫЙ СТОП  СЭП)
    ptrbtn2->bit6 = (BoardNormal[PN1].In[2] & (1<<0))>>0;             //Вход состояния нажатия кнопки SB7(СТОП Д-Г) Н.З.
    ptrbtn2->bit7 = (BoardNormal[PN1].In[2] & (1<<1))>>1;             //Вход состояния нажатия кнопки SB6(ПУСК Д-Г) Н.Р.
    ptrbtn3->bit0 = (BoardNormal[PN1].In[5] & (1<<1))>>1;             //откл ВИП
    ptrbtn3->bit1 = (BoardNormal[PN1].In[8] & (1<<1))>>1;             //КОНТРОЛЬНЫЙ СИГНАЛ -ВХОД РАБОТА
    ptrbtn3->bit2 = (BoardNormal[PN1].In[8] & (1<<0))>>0;             //КОНТРОЛЬНЫЙ СИГНАЛ -ВХОД РАЗГОН
    ptrbtn3->bit3 = !((BoardNormal[PN1].In[7] & (1<<7))>>7);         //СЕТЬ ВИП ПОДКЛЮЧЕНА
    ptrbtn3->bit4 = (BoardNormal[PN1].In[5] & (1<<0))>>0;  			//(СТОП Д-Г)
	
	ptrsens1->bit0 = (BoardNormal[PN1].In[3] & (1<<0))>>0;     		//авария давления дизеля дад
	ptrsens1->bit1 = (BoardNormal[PN1].In[3] & (1<<1))>>1;     		//нет перегрева охлаждающей жидкости
	ptrsens1->bit2 = (BoardNormal[PN1].In[3] & (1<<2))>>2;				//нез загрязнения дзф
	ptrsens1->bit3 = (BoardNormal[PN1].In[3] & (1<<3))>>3;				//авария давления редуктора дад
	ptrsens1->bit4 = (BoardNormal[PN1].In[3] & (1<<4))>>4;				//нет масла в поддоне дум
	ptrsens1->bit5 = (BoardNormal[PN1].In[3] & (1<<5))>>5;				//нет охлаждающей жидкости дуож
	ptrsens1->bit6 = (BoardNormal[PN1].In[6] & (1<<1))>>1;				//БЛОКИРОВКА АВАРИЙНАЯ ГС2-40
	ptrsens1->bit7 = (BoardNormal[PN1].In[6] & (1<<2))>>2;				//температура переднего подшибника выше нормы
	ptrsens2->bit0 = (BoardNormal[PN1].In[6] & (1<<3))>>3;				//температура заднего подшибника выше нормы
	ptrsens2->bit1 = (BoardNormal[PN1].In[6] & (1<<4))>>4;				//температура статора генератора выше нормы
	ptrsens2->bit2 = (BoardNormal[PN1].In[6] & (1<<5))>>5;				//короткое замыкание ГС2-40
	ptrsens2->bit3 = (BoardNormal[PN1].In[6] & (1<<6))>>6;				//напряжение генератора выше допустимого ГС2-40
	ptrsens2->bit4 = (BoardNormal[PN1].In[6] & (1<<7))>>7;				//возбужение включено
	ptrsens2->bit5 = (BoardNormal[PN1].In[7] & (1<<4))>>4;				//возбужение выключено
	ptrsens2->bit6 = (BoardNormal[PN1].In[7] & (1<<5))>>5;				//неисправность БА5-12Т27
	ptrsens2->bit7 = (BoardNormal[PN1].In[3] & (1<<6))>>6;				//авария ВИП
	ptrsens3->bit0 = (BoardNormal[PN1].In[4] & (1<<3))>>3;				//опасная нейтраль
	ptrsens3->bit1 = (BoardNormal[PN1].In[5] & (1<<3))>>3;				//сопротивление изоляции Г1 ниже нормы
	ptrsens3->bit2 = (BoardNormal[PN1].In[5] & (1<<4))>>4;				//сопротивление изоляции Г2 ниже нормы       
    
    pmv_remote_off = (BoardNormal[PN1].In[31] & (1<<0));                     
}

void sep_stop(void)
{
	port_set_out(ctrl_sep_stop);
}

void sep_reset_stop(void)
{
	port_unset_out(ctrl_sep_stop);
}

/*!
  Алгоритм контроля кнопки включения блоков генераторов (ВКЛ.БГ)
*/
void alg_control_button_bg(void)
{
	ExternalPower *ptrpower = &Extpower;
    BlockGenerators *ptrgen = &Sysep.generators;
    Ebu *ptrebu = &Sysep.disel.ebu;
    Pki *ptrpki1 = &ptrgen->pki[0];
	Pki *ptrpki2 = &ptrgen->pki[1];    
	ByteData *ptrbtn3 = &btn3;
	uint8_t pki1_phase_a_norma=0;
	uint8_t pki1_phase_b_norma=0;
	uint8_t pki1_phase_c_norma=0;
	uint8_t pki1_resist_norma=0;
	uint8_t pki1_freq_norma=0;

	uint8_t pki2_phase_a_norma=0;
	uint8_t pki2_phase_b_norma=0;
	uint8_t pki2_phase_c_norma=0;
	uint8_t pki2_resist_norma=0;
	uint8_t pki2_freq_norma=0;
	//uint16_t ebu_speed=0;
	uint8_t ebu_speed_norma=0; 

    pki1_phase_a_norma = ((ptrpki1->phase_a>=198) && (ptrpki1->phase_a<=242));
    pki1_phase_b_norma = ((ptrpki1->phase_b>=198) && (ptrpki1->phase_b<=242));
    pki1_phase_c_norma = ((ptrpki1->phase_c>=198) && (ptrpki1->phase_c<=242)); 
    pki1_freq_norma = ((ptrpki1->freq>=480) && (ptrpki1->freq<=520)); 
    pki1_resist_norma = (ptrpki1->resist>20);
		
    pki2_phase_a_norma = ((ptrpki2->phase_a>=209) && (ptrpki2->phase_a<=231));
    pki2_phase_b_norma = ((ptrpki2->phase_b>=209) && (ptrpki2->phase_b<=231));
    pki2_phase_c_norma = ((ptrpki2->phase_c>=209) && (ptrpki2->phase_c<=231));
    pki2_freq_norma = ((ptrpki2->freq>=3840) && (ptrpki2->freq<=4160));
    pki2_resist_norma = (ptrpki2->resist>20);
		
    if((ptrebu->read_engine_speed>=ENGINESPEED1746) & (ptrebu->read_engine_speed<=ENGINESPEED1854)) ebu_speed_norma=1;
    else ebu_speed_norma=0;

    if((pki1_phase_a_norma) & (pki1_phase_b_norma) & (pki1_phase_c_norma) & (pki1_resist_norma) & (pki1_freq_norma)) pki1_norma=1;
    else pki1_norma=0;
    if((pki2_phase_a_norma) & (pki2_phase_b_norma) & (pki2_phase_c_norma) & (pki2_resist_norma) & (pki2_freq_norma)) pki2_norma=1;
    else pki2_norma=0;

    if((pki2_norma & pki1_norma & ebu_speed_norma) 
    | ((!ptrbtn3->bit2) & ptrpower->inouts.bit2 & ptrbtn3->bit1))	//разгон; индикация ВИП; работа 
    PORTC |= (1<<PORTC6);												//снимаем блокировку вкл БГ
    else {
        PORTC &= ~(1<<PORTC6);											//устанавливаем блокировку
    }
}

/*!
  Алгоритм контроля индикатора - РАБОТА СЭП
*/
void alg_control_x12(void)
{
	Disel *ptrdisel = &Sysep.disel;
	Ebu *ptrebu = &ptrdisel->ebu;
	
	if(ptrebu->read_engine_speed>=ENGINESPEED800) {
		PORTD |= (1<<PORTD5);
	}
	
	if(ptrebu->read_engine_speed<ENGINESPEED100)
	{
		PORTD &= ~(1<<PORTD5);	
	}
	
}

///*!
//  Состояние STATE_MACHINE для ручного режима
//  \param[out] speaker
//*/
//void state_manual_mode(void)
//{
//	Disel *ptrdisel = &Sysep.disel;
//	Ebu *ptrebu = &ptrdisel->ebu;	
//	ByteData *ptrbtn1 = &btn1;
//	ByteData *ptrbtn2 = &btn2;	
//	
//	if((!ptrbtn2->bit0)										//не SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП)
//	& (!ptrbtn1->bit3)										//SA2(РАБОТА ДИЗ. СЭП)
//	& (!ptrbtn1->bit1)) {									//SA1(СТАРТЕР/ХХ/СТОП) ХХ
//		user_counter(1,SEVENTEENTH,QUANTITY_OF_CYCLES800);	//фильтр на срабатывание счетчик = 8*100
//		if(sepcnt[SEVENTEENTH].FlagSetTimer){
//			sepcnt[SEVENTEENTH].FlagSetTimer=0;
//			speaker = START_SEP;
//		}
//	}else sepcnt[SEVENTEENTH].FlagSetTimer=0;
//		
//	if((!ptrbtn2->bit0)										//SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП)
//	& (!ptrbtn1->bit3)										//SA2(РАБОТА ДИЗ. СЭП)
//	& (ptrbtn1->bit1)) {									//SA1(СТАРТЕР/ХХ/СТОП) ХХ
//		user_counter(1,EIGHTEENTH,QUANTITY_OF_CYCLES800);	//фильтр на срабатывание счетчик = 8*100
//		if(sepcnt[EIGHTEENTH].FlagSetTimer){
//			sepcnt[EIGHTEENTH].FlagSetTimer=0;
//			speaker = START_SEP;	
//		}
//	}else sepcnt[EIGHTEENTH].FlagSetTimer=0;
//	
//	if((!ptrbtn2->bit0)										//SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП)
//	& (ptrbtn1->bit3)										//SA2(РАБОТА ДИЗ. СЭП)
//	& (ptrbtn1->bit1)) {									//SA1(СТАРТЕР/ХХ/СТОП) ХХ
//		if(ptrebu->read_engine_speed>=ENGINESPEED1790)
//		speaker = MANUAL_ENG_RUN_SPEED1800;
//		else speaker = MANUAL_ENG_RUN_SPEED900;
//	}	
//	
//}

///*!
//  Диспетчер входных параметров
//*/
//void disp_input_param(void)
//{
//	//Blocks *ptrgen1 = &Sysep.generators.generator[0];
////	Blocks *ptrgen2 = &Sysep.generators.generator[1];
//	ByteData *ptrbtn2 = &btn2;
//	ByteData *ptrbtn3 = &btn3;
//	ExternalPower *ptrpower = &Extpower;
//	static uint16_t cnt_check_button_stop=0;
//    static uint8_t cnt_recharging=0;
//	
////	if(!ptrbtn2->bit1) {								//при появлении сигнала ОТКЛ. СЭП переходит в ветку штатного останова
////		cnt_check_button_stop++;
////		if(cnt_check_button_stop>=QUANTITY_OF_CYCLES700) {			//фильтр на включения кнопки откл СЭП
////			speaker = STOP_SEP;
////			cnt_check_button_stop=0;		
////			if(!ptrpower->inouts.bit2){					//выкл режима подзарядки
////				ptrpower->status_reg=NOTRECHARGING;		//выход из режима подзарядки
////				recharging=0;						
////			}
////		}
////	}
//	
//	//автоматический стоп в режиме ВИП, для того чтобы из любого места в режиме ВИП можно было перейти в останов
//	if((ptrbtn3->bit4)									//стоп Д-Г					
//	& (ptrbtn2->bit0)									//SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП)
//	& (!(PORTB & (1<<PORTB6)))                          //отключить нагрузку от Г2
//	& (!flag_keep_stopdg))	
//	{
//		speaker = STOP_EXTPWD;
//	}
//	
//	//автоматический режим
//	if((ptrbtn2->bit4)								//SB11(АВТ. СТАРТ СЭП)
//		& (ptrbtn2->bit0)							//SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП)
//		& (!alarm)) {				
//			speaker = START_SEP;
//		}
//	
////	//режим подзаряда батареи
////	if((ptrbtn2->bit3)								//кнопка вкл ВИП
////	& (!ptrbtn2->bit0)								//SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП)
////	& (ptrpower->inouts.bit2))                //индикация вип
////    {					
////		if(cnt_recharging++>=QUANTITY_OF_CYCLES200)
////		{
////			ptrpower->status_reg=RECHARGING;
////			recharging=1;								//подзарядка аккумуляторов
////			cnt_recharging=QUANTITY_OF_CYCLES_IS_0;			
////		}
////	}
//		
//	state_manual_mode();
//		
//	if(speaker==START_SEP) {
//		//PORTC |= (1<<PC2);						//блокировка стартера
//		starter_block();		
//	}
//}

/*!
  Алгоритм блокировки стартера
*/
void alg_starter_block(void)
{
    ByteData *ptrbtn2 = &btn2;

	if(ptrbtn2->bit0)					//SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП)    
    {
    	if(current_state==START_SEP_) 
        {
		    starter_block();
        }          
    }
}

/*!
  Диспетчер аварийных отключений СЭП.
  По факту - отключение СЭП по контролю параметров ПКИ и от ПМВ
*/
void alg_alarm_disable(void)
{
	Disel *ptrdisel = &Sysep.disel;
	ExternalPower *ptrpower = &Extpower;	
	Blocks *ptrgen1 = &Sysep.generators.generator[0];
	Blocks *ptrgen2 = &Sysep.generators.generator[1];
	ByteData *ptrbtn2 = &btn2;	
	StatusReg *ptrpki2 = &Sysep.generators.pki[1].status_reg;
	StatusReg *ptrpki1 = &Sysep.generators.pki[0].status_reg;
	uint8_t *loc_pki2 = (uint8_t *)ptrpki2;
	uint8_t *loc_pki1 = (uint8_t *)ptrpki1;
	
	if(((ptrbtn2->bit5)	& (ptrgen1->inouts.bit4	| ptrgen2->inouts.bit4 | ptrpower->inouts.bit2))		//если нажата кнопка SB9 - аварийный стоп	//включен Г1 или Г2 или работает ВИП //ИЛИ
		| ptrpki2->bit0											//контроль параметров ПКИ1 (в проекте МК он pki2)
		| ptrpki2->bit1											//если значение выходят за пределы +/-11 и +/-33В -> СЭП отключается
		| ptrpki2->bit2
		| ptrpki2->bit3
		| ptrpki1->bit0											//контроль параметров ПКИ2 (в проекте МК он pki)
		| ptrpki1->bit1
		| ptrpki1->bit2
		| ptrpki1->bit3 
        | pmv_remote_off                                       
		) {
			user_counter(1,SIXTEENTH,35);									//фильтр на срабатывание ~3сек счетчик = 25*100 (100 период цикла входа в алгоритм сэп)
			if(sepcnt[SIXTEENTH].FlagSetTimer | pmv_remote_off)
			{
				sepcnt[SIXTEENTH].FlagSetTimer=0;
				sepcnt[SIXTEENTH].InsideCntTimer=0;
				sep_stop();									//откл СЭП
				//disel_gens_stop();							//снимаем коммутацию (в режиме дизеля только нужно, но делается всегда)
				(*loc_pki1)=0x00;
				(*loc_pki2)=0x00;
				ptrpower->status_reg=EXTPW_IS_OFF;			//вип выключен
				ptrdisel->reg_status=0;
                if(ptrbtn2->bit0)
				    current_state=AUTO_DIS_STOP;
                else current_state=START_SEP_;				
			}
		}else sepcnt[SIXTEENTH].FlagSetTimer=0;
}

/*!
    STATE_MACHINE автоматического режима запуска/останова дизеля СЭП, ВИП
*/
void alg_sep_auto(void)
{
    ByteData *ptrbtn2 = &btn2;
        
    int elementsCount = sizeof(transitionTable) / sizeof(transitionTable[0]);
    
    if(!ptrbtn2->bit0)current_state=START_SEP_;
    StartSep();
    StopSep();
    FMSFunc(elementsCount,transitionTable,current_state);    
}

/*!
    Сбрасывает ключ - ОТКЛЮЧИТЬ СЭП
*/
void reset_key_disable_sep(void)
{
	Sep *ptrsep = &Sysep;
	static uint16_t cnt_stop_sep=0;
	
	ptrsep->inouts.bit1=port_state_for_outs(ctrl_sep_stop);
	
	if(ptrsep->inouts.bit1) {									//сброс X13 -> PD1
		cnt_stop_sep++;
		if(cnt_stop_sep>=QUANTITY_OF_CYCLES400){
			cnt_stop_sep=QUANTITY_OF_CYCLES_IS_0;
			sep_reset_stop();
		}
	}
}

/*!
    Записывает статусы состояния СЭП
*/
void sep_status_reg(void)
{
	Sep *ptrsep = &Sysep;
	Disel *ptrdisel = &ptrsep->disel;
	ExternalPower *ptrpower = &Extpower;
	Blocks *ptrvalve = &ptrdisel->valve;
	Blocks *ptrscepl = &ptrdisel->scepl;
    ByteData *ptrbtn2 = &btn2;
    static uint16_t cnt_push_dg=0;	
	
	if(current_state==START_SEP_) {
		ptrsep->status_reg.bit0=1;																//статус регистр - сэп выключен
		ptrsep->status_reg.bit1=0;																//сброс сэп включен
		ptrsep->status_reg.bit4=0;																//сброс работает сэп от ВИП
		ptrsep->status_reg.alarm &= ~(1<<0);													//сброс отключение сэп
		ptrdisel->alarm_status.bit1=0;				                                            //нет масла в двигателе (при запуске в атоматич. режиме)
		ptrdisel->alarm_status.bit3=0;				                                            //попытка запустить двиг. в ручном режиме без сцепления        
	}
	else ptrsep->status_reg.bit0=0;
		
//	if(ptrdisel->ebu.read_engine_speed==0x00)
//	    ptrdisel->reg_status=0x00;
		
	if((ptrdisel->reg_status==ENGIN_SEP_STOPING) | (ptrpower->status_reg==EXTPW_STOPING))
		ptrsep->status_reg.alarm |= (1<<0);														//отключение сэп
	
	if(ptrbtn2->bit7 & !ptrbtn2->bit0)						//если нажата кнопка ПУСК-ДГ и АВТ.ДИЗ.СЭП в ручном
	{ 
		 if(cnt_push_dg++>=QUANTITY_OF_CYCLES300)								//фильтр на срабатывания
		 {
			status_pki_need_clear=NEEDCLEAR;				//очищаем статус пки от предыдущих флагов
			cnt_push_dg=QUANTITY_OF_CYCLES_IS_0;
		 }
	}         		
}


union copy_status{
	StatusReg Status_bits;
	unsigned int biuld_status;
};
uint8_t convert_status_reg(StatusReg sta)
{
	union copy_status localstatus;
	localstatus.Status_bits = sta;
	return localstatus.biuld_status;
}

void set_data_for_can(void)
{
	Sep *ptrsep = &Sysep;
	Disel *ptrdis = &Sysep.disel;
	Blocks *ptrvalve = &ptrdis->valve;
	Blocks *ptrscepl = &ptrdis->scepl;
//	ExternalPower *ptrpower = &Extpower;
	BlockGenerators *ptrgens = &ptrsep->generators;
	Blocks *ptrgen1 = &Sysep.generators.generator[0];
	Blocks *ptrgen2 = &Sysep.generators.generator[1];
	Pki *ptrpki1 = &ptrgens->pki[0];
	Pki *ptrpki2 = &ptrgens->pki[1];
/*
	uint8_t *tmp1 = (uint8_t *)&ptrpki1->inouts;
	uint8_t *tmp2 = (uint8_t *)&ptrpki2->inouts;*/

	OutDataForCantmp[0] = current_state;											//статус алгоритма 
	OutDataForCantmp[1] = Sysep.disel.reg_status | Extpower.status_reg;    //ptrdis->reg_status
	OutDataForCantmp[2] = convert_status_reg(ptrsep->status_reg);					//convert_status_reg(ptrdis->alarm_status);
	OutDataForCantmp[3] = (ptrscepl->status_reg<<4) | ptrvalve->status_reg;		//младшая тетрада - состояние заслонки, старшая - состояние сцепления
	OutDataForCantmp[4] = (ptrgen2->status_reg<<4) | ptrgen1->status_reg;			//convert_status_reg(ptrsep->alarm_status);
	OutDataForCantmp[5] = ptrgens->status_reg_extpw; 
	OutDataForCantmp[6] = convert_status_reg(ptrdis->alarm_status);
	OutDataForCantmp[7] = convert_status_reg(ptrpki1->status_reg) | (convert_status_reg(ptrpki2->status_reg)<<4);
}

/*!
    Запускает Алгоритмы работы СЭП
*/
void alg_sep(void)
{	
	set_ins();
    
    reset_key_disable_sep();
	
	sep_status_reg();
    
    alg_control_button_bg();
    
    alg_alarm_disable();
    
    alg_starter_block();
	
	pki_state();
	
	pki_control_parametrs();
	
	gens_reset_pa1();						//снять коммутация (не саму коммутацию, а сигнал с ключа)
	
	starter_state();	
    
    alg_sep_auto();
	
	valve_state();
	
	scepl_state();
    
    alg_control_x12();
	
	block_gen_state();	
	
	gens_state();
	
	set_data_for_can();
}


void SetState(uint8_t state)
{
	prev_state = current_state;
    current_state = state;	
}

void StartSep(void)
{
	ByteData *ptrbtn1 = &btn1;
    ByteData *ptrbtn2 = &btn2;
    ExternalPower *ptrpower = &Extpower;

	if((ptrbtn2->bit4) 					//SB11(АВТ. СТАРТ СЭП)
	&(ptrbtn2->bit0)					//SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП)
	& (!ptrpower->inouts.bit2)          //ВИП не подключен
    & (!pmv_remote_off))                //флаг аварийного отключения, команда от ПМВ
	{		
        if(current_state==START_SEP_)
            SetState(AUTO_DIS_OPEN_VALVE_SCEPL);
	}

	if((ptrbtn2->bit0) & (ptrbtn2->bit3))								//SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП) //кнопка вкл ВИП //			& (ptrbtn2->bit3)
    {	
        if(recharging==0)											//не режим подзарядки
        {						
            ptrpower->status_reg=EXTPW_IS_DISCONNECT;				//!!!!!!!!!нет сети ВИП 
            sepcnt[FIRSTCNT].FlagSetTimer=0;
			if(current_state==START_SEP_)
                SetState(AUTO_EXTPWD_CHECK_INDEP);
        }
	}
    
	if((ptrbtn2->bit3)								//кнопка вкл ВИП
	& (!ptrbtn2->bit0))								//не SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП)
    {
        SetState(RECHARGING_TIMER_START);
    }        	
}

void StopSep(void)
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Blocks *ptrgen1 = &ptrgen->generator[0];
	Blocks *ptrgen2 = &ptrgen->generator[1];
	ByteData *ptrbtn1 = &btn1;
    ByteData *ptrbtn2 = &btn2;
	ByteData *ptrbtn3 = &btn3;
    static uint8_t filter_btn_off_sep=0;
    static uint8_t cnt_stop=0;
	
		if((!ptrbtn2->bit1) & (ptrbtn2->bit0)) 					//нажата кнопка ОТКЛ СЭП и система управления в автоматич. режиме
		{ 
            PORTC &= ~(1<<PORTC0);									//выключаем стартер на всякий случай
            if((current_state==AUTO_DIS_OPEN_VALVE_SCEPL) 
            || (current_state==AUTO_DIS_ON_EBU)
            || (current_state==AUTO_DIS_ALG_STARTER)
            || (current_state==AUTO_DIS_IS_SPEED900))
                SetState(AUTO_DIS_STOP);
            if(current_state==STOP_DIS)
            {   
                if((!ptrgen1->inouts.bit4) & (!ptrgen2->inouts.bit4))            
                SetState(AUTO_DIS_OFF_GENS);                
            }
		}
	
		if((ptrbtn3->bit4)									//(СТОП Д-Г)
		& (ptrbtn2->bit0)									//SA12(АВТОМАТИКА ДИЗЕЛЯ СЭП)
		& (!(PINB & (1<<PORTB6)))
		& (!flag_keep_stopdg)) 
		{							
			SetState(AUTO_EXTPWD_UNLOAD);
			flag_keep_stopdg=1;
		}
        
        if((!ptrbtn2->bit1)                                  //ОТКЛ СЭП
        && (recharging==1))
        {
           if(filter_btn_off_sep++>=QUANTITY_OF_CYCLES300)
           {
             filter_btn_off_sep=0;
             SetState(RECHARGING_TIMER_STOP);
           }
        }	          
}

void OpenValveScepl(void)
{
	Disel *ptrdisel = &Sysep.disel;
	Blocks *ptrscepl = &ptrdisel->scepl;
	Blocks *ptrvalve = &ptrdisel->valve;
    ExternalPower *ptrpower = &Extpower;	
	
	if(ptrvalve->status_reg !=0x04)						//заслонка не открыта? -> открываем 0x04 
	{
		valve_open();
	}
	if(ptrscepl->status_reg!=0x04) 						//сцепление не выключено? -> выключаем 0x04 
	{						
		scepl_off();
	}

	if((ptrvalve->status_reg==0x04) && (ptrscepl->status_reg==0x04))
	{   
        if(current_state==AUTO_DIS_OPEN_VALVE_SCEPL)
		    SetState(AUTO_DIS_ON_EBU);
        if(current_state==AUTO_EXTPWD_OPEN_VALVE_SCEPL)
            SetState(AUTO_EXTPWD_ON_GEN);
	}
    if((ptrvalve->status_reg==0x0D) || (ptrscepl->status_reg==0x0D))            //если заслонка И сцепления не сработали->
    {
        if(current_state==AUTO_DIS_OPEN_VALVE_SCEPL)                            //идем в этот статус в случае запуска дизеля                                            
           SetState(AUTO_DIS_STOP);
            
        if(current_state==AUTO_EXTPWD_OPEN_VALVE_SCEPL)                         //идем в этот статус в случае запуска ВИП
        {
            ptrpower->status_reg=ALARM_EXTPW_NOTSTART;		                    //авария включения ВИП
            SetState(AUTO_EXTPWD_WORKING_30SEC);
        }    
    }
}

void OnEBU(void)
{
	Disel *ptrdisel = &Sysep.disel;
	
	ebu_start(ENGINESPEED900);							//УПРАВЛЕНИЕ РАБОТОЙ/ОСТАНОВОМ ДИЗЕЛЯ СЭП ЧЕРЕЗ ЭБУ (900 об.) 
	ptrdisel->reg_status=ENGIN_SEP_READY_TO_START;		//двигатель СЭП к пуску готов
	septimers[THIRDTIMER].FlagSetTimer=0;				//очищаем таймер на открытия заслонки
	septimers[FOURTIMER].FlagSetTimer=0;				//очищаем таймер на включения сцепления
	septimers[SECONDTIMER].FlagSetTimer=1;				//для того чтобы включить стартер в следующем состоянии
	starter_unblock();									//разблокировка стартера    
	SetState(AUTO_DIS_ALG_STARTER);	
}

void AlgStarter(void)
{
	Disel *ptrdisel = &Sysep.disel;
	Ebu *ptrebu = &ptrdisel->ebu;
	
	if((cnt_start_starter<QUANTITY_STARTER_START)
	& (!(PORTC & (1<<PORTC0)))									//нет СТАРТЕРА
	& (ptrebu->read_engine_speed==0)
	& septimers[SECONDTIMER].FlagSetTimer) 
	{							
		starter_on();										//ФОРМИРОВАНИЕ СИГНАЛА ЗАПУСКА СТАРТЕРА ПРИ АВТОСТАРТЕ
		septimers[SECONDTIMER].FlagSetTimer=0;
	}
		
	if(ptrebu->read_engine_speed >= ENGINESPEED400) 
	{							
		starter_off();										//выключаем стартер
		ptrdisel->reg_status=ENGIN_SEP_STARTED;				//двигатель запустился
	}		
		
	if(ptrebu->read_engine_speed >= ENGINESPEED800) 
	{
		starter_off();										//выключаем стартер
		septimers[FIRSTTIMER].FlagSetTimer=0;
		septimers[SECONDTIMER].FlagSetTimer=0;
		cnt_start_starter=0;
		SetState(AUTO_DIS_IS_SPEED900);
	}
	if((septimers[FIRSTTIMER].FlagSetTimer)
	& (cnt_start_starter<QUANTITY_STARTER_START)
	& (ptrebu->read_engine_speed==0)) 
	{
		cnt_start_starter++;
		starter_off();									//выключаем стартер
		septimers[FIRSTTIMER].FlagSetTimer=0;						
	}
	//if((septimers[FIRSTTIMER].FlagSetTimer) 
	if(cnt_start_starter>=QUANTITY_STARTER_START) 
	{
		starter_off();
        cnt_start_starter=0;									//выключаем стартер
		//septimers[FIRSTTIMER].FlagSetTimer=0;				
		ptrdisel->alarm_status.bit0=1;					//аларм - двигатель не запустился
		ptrdisel->reg_status=ALARM_ENGIN_SEP_NOTSTART;	//аларм - двигатель не запустился
        SetState(AUTO_DIS_STOP);
	}
	user_timer_with_step_two_sec((PINC & (1<<PORTC0)),FIRSTTIMER,12);					//стартер выключается через 12-2 сек.
	user_timer_with_step_one_sec((!(PINC & (1<<PORTC0))),SECONDTIMER,5);				//повторяем запуск стартера через 5 сек.	
}

void DiselIs900(void)
{
	Disel *ptrdisel = &Sysep.disel;
	Pgd *ptrpgd = &ptrdisel->pgd;
    uint8_t press_norma=0, liq_norma=0;
	
	starter_off();										    //выключаем стартер
	starter_block();
	user_timer_with_step_two_sec(1,FIRSTTIMER,32);			//30-2 сек. на 900 оборотах работаем 30 сек.
	press_norma = (press_oil_sep>=PRESS175);                // & (press_oil_sep<=PRESS180));
	liq_norma = (ptrpgd->temp_liq>=TEMPCOOLIQ450);          // | (ptrpgd->temp_liq<=TEMPCOOLIQ558));
	if(septimers[FIRSTTIMER].FlagSetTimer){                 //& press_norma & liq_norma
		septimers[FIRSTTIMER].FlagSetTimer=0;
		SetState(AUTO_DIS_IS_SPEED1800);
	}
//	if(septimers[FIRSTTIMER].FlagSetTimer) {                //& (!press_norma) & (!liq_norma)
//		septimers[FIRSTTIMER].FlagSetTimer=0;
//		ptrdisel->reg_status=ALARM_ENGIN_SEP_NOTSTART;		//аларм - нет давления масла в двигателе
//		ptrdisel->alarm_status.bit1=1;						//аларм - нет давления масла в двигателе 
//        SetState(AUTO_DIS_STOP);
//	}	
}

void DiselIs1800(void)
{
	Sep *ptrsep = &Sysep;
//	Disel *ptrdisel = &Sysep.disel;
	
	ebu_start(ENGINESPEED1800);								//установить ОБОРОТЫ 1800
	user_timer_with_step_two_sec(1,FIRSTTIMER,32);
	ptrsep->status_reg.bit2=1;								//двигатель не нагружать
	if((septimers[FIRSTTIMER].FlagSetTimer))
	{
		SetState(AUTO_DIS_ON_GENS);
	}
}

void DiselOnGens(void)	
{	
	Sep *ptrsep = &Sysep;
	Disel *ptrdisel = &Sysep.disel;
	Blocks *ptrgen1 = &ptrsep->generators.generator[0];
    Blocks *ptrgen2 = &ptrsep->generators.generator[1];

	septimers[FIRSTTIMER].FlagSetTimer=0;
	ptrsep->status_reg.bit2=0;							//двигатель не нагружать
	ptrdisel->reg_status=ENGIN_SEP_READY_TO_LOAD;		//двигатель к нагрузке готов									
	disel_gens_start();									//коммутация
	status_pki_need_clear=NEEDCLEAR;					//очищаем статус пки от предыдущих флагов
	ptrgen2->inouts.bit2=ptrgen1->inouts.bit2=1;		//к механизму контроля  генераторов 
	SetState(AUTO_DIS_GENS_IS_ON);
}

void DiselGensIsOn(void)
{
	Sep *ptrsep = &Sysep; 
    Disel *ptrdisel = &Sysep.disel;
	Blocks *ptrgen1 = &ptrsep->generators.generator[0];
	Blocks *ptrgen2 = &ptrsep->generators.generator[1];
    static uint8_t cnt=0;
	
	if(ptrgen1->inouts.bit4 && ptrgen2->inouts.bit4)		//если генераторы включены значит считаем, что сэп включен и можем включать нагрузку
	{
		ptrdisel->reg_status=ENGIN_SEP_LOADED;				//двигатель нагружен
        ptrsep->status_reg.bit1=1;							//СЭП включен
		SetState(STOP_DIS);  
        cnt=0;
	}
        if(cnt++>=20)
        {     
            cnt=0;
            SetState(STOP_DIS);
        }
}

void DiselOffGens(void)	
{
	BlockGenerators *ptrgen = &Sysep.generators;
	Blocks *ptrgen1 = &ptrgen->generator[0];
	Blocks *ptrgen2 = &ptrgen->generator[1];	
	//PORTA &= ~(1<<PA1);										
	disel_gens_stop();										//сигнал на выключение коммутации
	ptrgen2->inouts.bit3=ptrgen1->inouts.bit3=1;			//к механизму контроля генераторов
	//ebu_stop(ENGINESPEED1800);							//снимаем 1800 оборотов
//	if((!ptrgen1->inouts.bit4)                              //перенесено в StopSep()
//	& (!ptrgen2->inouts.bit4))
    {	
		//ptrdisel->reg_status.alarm &=(1<<0);				//!!!не хватило статусных битов, взят из alarm-ов!!! генераторы откл.
		//ptrdisel->reg_status.bit4=0;						//двигатель СЭП к нагрузке готов
		septimers[FIRSTTIMER].FlagSetTimer=0;
		SetState(AUTO_DIS_WORKING_5MIN);
	}		
}

void DiselWorking5Min(void)
{
    Disel *ptrdisel = &Sysep.disel;
    
	user_timer_with_step_two_sec(1,FIRSTTIMER,300);
	ptrdisel->reg_status=ENGIN_SEP_STOPING;						//двигатель перешел к процессу остановки
	if((septimers[FIRSTTIMER].FlagSetTimer)) 
	{						
		septimers[FIRSTTIMER].FlagSetTimer=0;
		SetState(AUTO_DIS_STOP);	
	}	
}

void DiselStop(void)
{
    Sep *ptrsep = &Sysep;
	Disel *ptrdisel = &Sysep.disel;
//    ExternalPower *ptrpower = &Extpower;
	
	valve_close();
	ebu_stop(ENGINESPEED1800);							//снимаем 1800 оборотов
	ebu_stop(ENGINESPEED900);							//снимаем 900 оборотов
    ptrsep->status_reg.bit1=0;                          //СЭП отключен
    if(prev_state==AUTO_DIS_WORKING_5MIN)
        ptrdisel->reg_status=START_SEP_;					
	SetState(START_SEP_);
}

void ExtPwdCheckIndEP(void)
{
    ExternalPower *ptrpower = &Extpower;
    
    user_counter(1,FIRSTCNT,7);						//счетчик = 5*T_MAIN
	valve_open();
	scepl_off();									//замыкаем сцепление
	if((sepcnt[FIRSTCNT].FlagSetTimer)				//прошло ~500ms сеть ВИП не появилась
	& (!ptrpower->inouts.bit2))                     //индикация ВИП
    {					
		ptrpower->status_reg=ALARM_EXTPW_NOTSTART;		//авария включения ВИП
		SetState(AUTO_EXTPWD_WORKING_30SEC);
	}
    if(ptrpower->inouts.bit2)                       //индикация ВИП
    {
        ptrpower->status_reg=EXTPW_IS_CONNECT;						//сеть ВИП подана
        SetState(AUTO_EXTPWD_OPEN_VALVE_SCEPL);
    }
}

void ExtPwdOnGen(void)
{
	ExternalPower *ptrpower = &Extpower;
	
	disel_gen_start();
	ptrpower->status_reg=EXTPW_IS_CONNECT;						//сеть ВИП подана
	SetState(AUTO_EXTPWD_CHECK_SPEEDUP_WORK);		
}

void ExtPwdCheckSpeedUpWork(void)
{
	ExternalPower *ptrpower = &Extpower;
	ByteData *ptrbtn3 = &btn3;
	
    user_counter(1,FIRSTCNT,5);
    if(!(sepcnt[FIRSTCNT].FlagSetTimer))return;
    sepcnt[FIRSTCNT].FlagSetTimer=0;
	if(ptrbtn3->bit2)											//разгон Д-Г 
	{								
		disel_gen_reset_out_start();
	}
	if(ptrbtn3->bit1) 											//работа Д-Г
	{								
		ptrpower->status_reg=EXTPW_IS_ON;						//ВИП включен
		disel_gen_reset_out_start();
		SetState(AUTO_EXTPWD_ON_LOAD);
	}	
}

void ExtPwdOnLoad(void)
{
    Sep *ptrsep = &Sysep;
	
	if(pki2_norma)
	{
		disel_gen_set_load();								//включить нагрузку на д-г
        ptrsep->status_reg.bit1=1;                          //СЭП включен
        ptrsep->status_reg.bit4=1;                          //СЭП работает от ВИПа
		status_pki_need_clear=NEEDCLEAR;					//очищаем статус пки от предыдущих флагов
        SetState(AUTO_EXTPWD_CHECK_G2);
	} 
    
}

void ExtPwdCheckG2(void)
{
    ExternalPower *ptrpower = &Extpower;
	BlockGenerators *ptrgen = &Sysep.generators;
	Blocks *ptrgen2 = &ptrgen->generator[1];    
	
    user_counter(ptrgen2->inouts.bit4,FIFTEENTH,5);			//счетчик = 5*100 //индикация Г2   ptrgen2->inouts.bit4
	if((sepcnt[FIFTEENTH].FlagSetTimer) & (ptrgen2->inouts.bit4)) 
	{
        sepcnt[FIFTEENTH].FlagSetTimer=0;								
		SetState(STOP_EXTPWD_);
	}
	if(sepcnt[FIFTEENTH].FlagSetTimer & (!ptrgen2->inouts.bit4))
    {
		ptrpower->status_reg=ALARM_EXTPW_NOTSTART;			//авария ВИП
        SetState(AUTO_EXTPWD_UNLOAD);
    }	
}

void ExtPwdUnLoad(void)
{
	disel_gen_set_unload();							//отключить нагрузку Д-Г
	SetState(AUTO_EXTPWD_GEN_STOP);
	flag_keep_stopdg=1;	
}

void ExtPwdGenStop(void)
{    	
	disel_gen_stop();									//стоп Д-Г
	SetState(AUTO_EXTPWD_GEN_IS_STOP);
}

void ExtPwdGenIsStop(void)
{
	ExternalPower *ptrpower = &Extpower;
	BlockGenerators *ptrgen = &Sysep.generators;
	Blocks *ptrgen2 = &ptrgen->generator[1];
    
	if(!ptrgen2->inouts.bit4) 
	{
		ptrpower->status_reg=EXTPW_STOPING;				//ВИП выключается
		valve_close();
		SetState(AUTO_EXTPWD_WORKING_30SEC);
	}
}

void ExtPwdWorking30Sec(void)
{
    Sep *ptrsep = &Sysep;
	ExternalPower *ptrpower = &Extpower;
	static unsigned long cnt_gen_unload=0;	

	cnt_gen_unload++;		
	if(cnt_gen_unload>=QUANTITY_OF_CYCLES2500) 					//~30 сек
	{						
		cnt_gen_unload=QUANTITY_OF_CYCLES_IS_0;
		//PORTD |= (1<<PD1);							//отключить СЭП
		sep_stop();
        ptrsep->status_reg.bit1=0;                          //СЭП отключен
        if(prev_state==AUTO_EXTPWD_GEN_IS_STOP)
		    ptrpower->status_reg=EXTPW_IS_OFF;				//вип выключен
		SetState(START_SEP_);
		flag_keep_stopdg=0;
	}	
}

static uint8_t cnt_recharge_start_timer=0;
static uint8_t cnt_recharge_stop_timer=0;
void RechargingTimerStart(void)
{
	ExternalPower *ptrpower = &Extpower;
	
	if(cnt_recharge_start_timer++>=QUANTITY_OF_CYCLES200)
	{
            cnt_recharge_start_timer = QUANTITY_OF_CYCLES_IS_0;
            
			if(ptrpower->inouts.bit2)
			{
				SetState(RECHARGING_IS_RECHARGE);
			}
			if(!ptrpower->inouts.bit2) 
			{	
				SetState(RECHARGING_ERROR_START);
			}
	}
}

void RechargingTimerStop(void)
{
	ExternalPower *ptrpower = &Extpower;
	
	if(cnt_recharge_stop_timer++>=QUANTITY_OF_CYCLES700)
	{
            cnt_recharge_stop_timer = QUANTITY_OF_CYCLES_IS_0;
            
			if(!ptrpower->inouts.bit2)
			{
				SetState(RECHARGING_IS_STOP);
			}
			if(ptrpower->inouts.bit2)
			{
				SetState(RECHARGING_ERROR_STOP);
			}

	}
}

void RechargingIsRecharge(void)
{
    ExternalPower *ptrpower = &Extpower;
    
    recharging=1;
	ptrpower->status_reg=RECHARGING;
	SetState(STOP_RECHARGE);
}

void RechargingErrorStart(void)
{
	ExternalPower *ptrpower = &Extpower;
	
	recharging=0;
	ptrpower->status_reg=ALARM_EXTPW_NOTSTART;
	SetState(STOP_RECHARGE);
}

void RechargingErrorStop(void)
{
	ExternalPower *ptrpower = &Extpower;
	
	recharging=0;
	ptrpower->status_reg=RECHARGING_ERR_STOP;
	SetState(RECHARGING_STOP_BY_KEY);
}

void RechargingStopByKey(void)
{
    sep_stop();									//откл СЭП
    SetState(RECHARGING_IS_STOP_BY_KEY);
}

void RechargingIsStopByKey(void)
{
	ExternalPower *ptrpower = &Extpower;
    static uint8_t cnt=0;
	
    if(cnt++<QUANTITY_OF_CYCLES200)return;
    cnt=0; 
        if(!ptrpower->inouts.bit2) 		                            //ВИП не подключен
            ptrpower->status_reg=NOTRECHARGING_BY_KEY;
        if(ptrpower->inouts.bit2) 		                            //ВИП подключен
            ptrpower->status_reg=RECHARGING_ERR_STOP_BY_KEY;
    recharging=0;
    SetState(START_SEP_);
}

void RechargingIsStop(void)
{
    ExternalPower *ptrpower = &Extpower;
    
    ptrpower->status_reg=NOTRECHARGING;
    recharging=0;
    SetState(START_SEP_);
}