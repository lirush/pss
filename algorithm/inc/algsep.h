#ifndef _ALGSEP_H
#define _ALGSEP_H

#define T_MAIN               (100)

#define DEBUG_SEP            (1)

#define QUANTITY_STARTER_START	(3)

#define ENGINESPEED100       (800)
#define ENGINESPEED400       (2800L)
#define ENGINESPEED600       (4800L)
#define ENGINESPEED800       (6400L)
#define ENGINESPEED900       (7200L)
#define ENGINESPEED1000      (8000L)
#define ENGINESPEED1700      (13600L)
#define ENGINESPEED1746      (13968L)
#define ENGINESPEED1750      (14000L)
#define ENGINESPEED1790      (14320L)
#define ENGINESPEED1800      (14400L)
#define ENGINESPEED1810      (14480L)
#define ENGINESPEED1850      (14800L)
#define ENGINESPEED1854      (14832L)
#define ENGINESPEED1900      (15200L)
#define TEMPCOOLIQ70         (70)
#define TEMPCOOLIQ150        (150)
#define TEMPCOOLIQ300        (300)
#define TEMPCOOLIQ450        (450)
#define TEMPCOOLIQ558        (558)
#define TEMPCOOLIQ824        (824)
#define PRESS175             (175)
#define PRESS180             (180)

#define QUANTITY_OF_CYCLES_IS_0  (0)
#define QUANTITY_OF_CYCLES200    (200/T_MAIN)
#define QUANTITY_OF_CYCLES300    (300/T_MAIN)
#define QUANTITY_OF_CYCLES700    (700/T_MAIN)
#define QUANTITY_OF_CYCLES800    (800/T_MAIN)
#define QUANTITY_OF_CYCLES2500   (2500/T_MAIN)
#define QUANTITY_OF_CYCLES400    (400/T_MAIN)
#define QUANTITY_OF_CYCLES10000  (10000/T_MAIN)

/*!
  \enum
  Набор таймеров
*/
enum ListTimers{
    FIRSTTIMER,
    SECONDTIMER,
    THIRDTIMER,
    FOURTIMER,
    FIFTHTIMER,
    SIXTHTIMER,
    SEVENTHTIMER,
    EIGHTTIMER
    };

/*!
  \enum
  Набор счетчиков
*/    
enum ListCnt{
    FIRSTCNT,
    SECONDCNT,
    THIRDCNT,
    FOURCNT,
    FIFTHCNT,
    SIXTHCNT,
    SEVENTHCNT,
    EIGHTCNT,
    NINTHCNT,
    TENTH,
    ELEVENTH,
    TWELFTH,
    THIRTEENTH,
    FOURTEENTH,
    FIFTEENTH,
    SIXTEENTH,
    SEVENTEENTH,
    EIGHTEENTH,
    NINETEENTH
    };

enum SepState{
    ENGIN_SEP_READY_TO_START=0x01,
    ENGIN_SEP_STARTED=0x02,
    ENGIN_SEP_READY_TO_LOAD=0x03,
    ENGIN_SEP_LOADED=0x04,
    ENGIN_SEP_STOPING=0x05,
    ALARM_ENGIN_SEP_NOTSTART=0x0F,
    EXTPW_IS_CONNECT=0x10,
    EXTPW_IS_ON=0x20,
    EXTPW_IS_OFF=0x30,
    EXTPW_IS_DISCONNECT=0x40,
    EXTPW_STOPING=0x50,
    RECHARGING=0x60,
    NOTRECHARGING=0x70,
    RECHARGING_ERR_START=0x80,
    RECHARGING_ERR_STOP=0x90,
    NOTRECHARGING_BY_KEY=0xA0,
    RECHARGING_ERR_STOP_BY_KEY=0xB0,
    ALARM_EXTPW_NOTSTART=0xF0
    }; 
    
enum ListStatesSep{
    START_SEP_,                         //0
    AUTO_DIS_OPEN_VALVE_SCEPL,          //1
    AUTO_DIS_ON_EBU,                    //2
    AUTO_DIS_ALG_STARTER,               //3
    AUTO_DIS_IS_SPEED900,               //4
    AUTO_DIS_IS_SPEED1800,              //5
    AUTO_DIS_ON_GENS,                   //6
    AUTO_DIS_GENS_IS_ON,                //7
    STOP_DIS,                           //8
    AUTO_DIS_OFF_GENS,                  //9
    AUTO_DIS_WORKING_5MIN,              //A
    AUTO_DIS_STOP,                      //B
    AUTO_EXTPWD_CHECK_INDEP,            //C
    AUTO_EXTPWD_OPEN_VALVE_SCEPL,       //D
    AUTO_EXTPWD_ON_GEN,                 //E
    AUTO_EXTPWD_CHECK_SPEEDUP_WORK,     //F
    AUTO_EXTPWD_ON_LOAD,                //10 
    AUTO_EXTPWD_CHECK_G2,
    STOP_EXTPWD_,                       //11
    AUTO_EXTPWD_UNLOAD,                 //12
    AUTO_EXTPWD_GEN_STOP,               //13 
    AUTO_EXTPWD_GEN_IS_STOP,            //14
    AUTO_EXTPWD_WORKING_30SEC,          //15
    RECHARGING_TIMER_START,             //16
    RECHARGING_IS_RECHARGE,             //17
    STOP_RECHARGE,                      //18
    RECHARGING_TIMER_STOP,              //19
    RECHARGING_ERROR_START,             //1A
    RECHARGING_ERROR_STOP,              //1B 
    RECHARGING_STOP_BY_KEY,             //1C
    RECHARGING_IS_STOP,                 //1D
    RECHARGING_IS_STOP_BY_KEY           //1E    
};    

enum ListInputs{
    OPEN=0x01,
    CLOSE=0x02,
    FEEDBACKOPEN=0x05,
    FEEDBACKCLOSE=0x0A,
    ERRSETCOMMAND=0x03
};

enum ListState{
    OPENING=0x01,
    CLOSING=0x02,    
    ISOPEN=0x05,
    ISOPENVALVE=0x04,
    ISCLOSE=0x0A,
    ISCLOSEVALVE=0x08,
    ERRORCONNECT=0x0C,
    ERRORDISCONNECT=0x0D,
    ERROROPEN1=0x11,
    ERRORCLOSE1=0x12,
    ERROROPEN2=0x19,
    ERRORCLOSE2=0x1E,    
    ERROROPEN3=0x1D,
    ERRORCLOSE3=0x16
};

enum ListNumPN{
	PN1,
	PN2,
	PN3,
	PN4
	};     
    
typedef struct
{
	uint8_t In[40];					
}BoardsNormal;

extern BoardsNormal BoardNormal[4];    

typedef struct
{
    unsigned char bit0    :1;
    unsigned char bit1    :1;
    unsigned char bit2    :1;
    unsigned char bit3    :1;
    unsigned char bit4    :1;
    unsigned char bit5    :1;
    unsigned char bit6    :1;
    unsigned char bit7    :1;
}ByteData;

typedef struct
{
    unsigned char bit0    :1;
    unsigned char bit1    :1;
    unsigned char bit2    :1;
    unsigned char bit3    :1;
    unsigned char bit4    :1;
    unsigned char alarm    :3;
}StatusReg;

typedef struct
{
    ByteData inouts;
    //StatusReg status_reg;    
    uint8_t status_reg;
}Blocks;

/*************************************************************************************************************/
/******************************** регистр статуса ЭБУ*********************************************************/
/*бит -     |  7   |     6    |   5  |       4      |        3     |      2       |      1     |     0      |*/
/*наимен. - |авария|нет аварии|резерв| 1800 оборотов| 900 оборотов | 600 оборотов | резерв     | включен    |*/
/*************************************************************************************************************/
typedef struct
{
    uint8_t startstop;
    uint8_t alarm;
    uint16_t read_engine_speed;            //обороты двигателя
    uint8_t flag_cntl;                    //флаг режим управления ЭБУ
    uint16_t set_engine_speed;            //уставка на обороты
    StatusReg status_reg;        
}Ebu;

typedef struct  
{
    uint8_t startstop;
    uint8_t alarm;
    uint16_t temp_liq;                        //температура ОЖ; входной параметр
    uint8_t warm;                        //команда подогреть
    StatusReg status_reg;
}Pgd;

typedef struct
{
    ByteData inouts;
    Blocks scepl;                        //сцепление
    Blocks starter;                        //стартер
    Blocks valve;                        //заслонка
    Pgd pgd;                            //ПЖД
    Ebu ebu;                            //ЭБУ
    StatusReg alarm_status;
    //StatusReg reg_status;
    uint8_t reg_status;
}Disel;


/*************************************************************************************************************/
/******************************** регистр статуса ПКИ ********************************************************/
/*бит -     |  7   |   6  |        5        |      4          |    3   |    2   |    1   |    0   |          */
/*наимен. - |резерв|резерв| R изоляции ПКИ2 | R изоляции ПКИ1 | f ПКИ2 | U ПКИ2 | f ПКИ1 | U ПКИ1 |          */
/*************************************************************************************************************/
typedef struct  
{
    ByteData inouts;
    uint8_t phase_a;
    uint8_t phase_b;
    uint8_t phase_c;
    uint16_t freq;
    uint16_t resist;                    //сопротивление изоляции
    StatusReg status_reg;
}Pki;

typedef struct
{
    ByteData inouts;
    Blocks generator[2];
    Pki pki[2];                            //прибор контроля измерений
    //StatusReg status_reg;
    uint8_t status_reg_extpw;            //в режиме вип
    uint8_t status_reg;                    //в режиме дизель
}BlockGenerators;

typedef struct
{
    ByteData inouts;
    Disel disel;
    BlockGenerators generators;         
    StatusReg status_reg;	
	StatusReg alarm_status;
}Sep;
extern Sep Sysep;

typedef struct
{
	ByteData inouts;
	//StatusReg status_reg;
	uint8_t status_reg;
}ExternalPower;
extern ExternalPower Extpower;
extern ByteData btn1;
extern ByteData btn2;
extern uint8_t OutDataForCantmp[8];

void alg_sep(void);

#include ".\algorithm\src\algsep.c"

#endif