#ifndef __TIMERS_H
#define __TIMERS_H

#define SHIFT0	(0)
#define SHIFT5	(4)

struct timers{
    unsigned int first        :1;
    unsigned int second        :1;
    unsigned int third        :1;
    unsigned int four        :1;
    unsigned int fifth        :1;
    unsigned int sixth        :1;
    unsigned int seventh    :1;
    unsigned int eigth        :1;
};

typedef struct{
    unsigned int first        :1;
    unsigned int second        :1;
    unsigned int third        :1;
    unsigned int four        :1;
    unsigned int fifth        :1;
    unsigned int sixth        :1;
    unsigned int seventh    :1;
    unsigned int eigth        :1;
}Timers_TypeDef;

union timer_build{
    struct timers timer_flags;
	uint16_t build;
};

typedef struct{
	uint8_t FlagSetTimer;
	uint16_t InsideCntTimer;
}Septimers;
extern Septimers septimers[8];
extern Septimers obj_septimers[8];
extern Septimers sepcnt[19];

void count_ticks(void);
void user_timer_with_step_two_sec(uint8_t EN, uint8_t num, uint16_t setting);
void user_timer_with_step_one_sec(uint8_t EN,uint8_t num, uint16_t setting);
void user_timer_with_step_two_sec_obj(uint8_t EN, uint8_t num, uint16_t setting);
void user_counter(uint8_t EN,uint8_t num,uint16_t setting);

#include ".\algorithm\src\timers.c"

#endif