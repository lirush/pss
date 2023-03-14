#include "timers.h"

static volatile union timer_build timer_millisecond;
static volatile union timer_build timer_second;
static Timers_TypeDef obj_timers;
Septimers septimers[8], sepcnt[19], obj_septimers[8];
static unsigned long int ticks=0;

/*!
  Секундный таймер. 
  Считает секунду и две секунды
*/
void calc_time_sec(volatile struct timers *ptr, unsigned long int tick)
{
    if((tick%10)==0) ptr->first=1;
    if((tick%10)==0) ptr->second=1;
    if((tick%10)==0) ptr->third=1;
    if((tick%10)==0) ptr->four=1;            
    if((tick%20)==0) ptr->fifth=1;            
    if((tick%20)==0) ptr->sixth=1;
    if((tick%20)==0) ptr->seventh=1;
    if((tick%20)==0) ptr->eigth=1;            
}

/*!
  Миллисекундный таймер
  Считает 100 миллисекунд и двести миллисекунд
*/
void calc_time_ms(volatile struct timers *ptr, unsigned long int tick)
{
    if((tick%1)==0) ptr->first=1;
    if((tick%1)==0) ptr->second=1;
    if((tick%1)==0) ptr->third=1;
    if((tick%1)==0) ptr->four=1;            
    if((tick%2)==0) ptr->fifth=1;            
    if((tick%2)==0) ptr->sixth=1;
    if((tick%2)==0) ptr->seventh=1;
    if((tick%2)==0) ptr->eigth=1;            
}

/*!
  Дополнительный секундный таймер
  Считает секунду
*/
void calc_time_sec_obj(unsigned long int tick)
{
    Timers_TypeDef *tme = &obj_timers;
    if((tick%10)==0) tme->first=1;
    if((tick%10)==0) tme->second=1;
    if((tick%10)==0) tme->third=1;
    if((tick%10)==0) tme->four=1;			//1 sec
	if((tick%10)==0) tme->fifth=1;			
	if((tick%10)==0) tme->sixth=1;
	if((tick%10)==0) tme->seventh=1;
	if((tick%10)==0) tme->eigth=1;			
}

/*!
    Установка таймеров  
    \param[out] timer_millisecond
    \param[out] timer_second
    \param[out] obj_timers
*/
void count_ticks(void)
{
	volatile struct timers *ptrsec = &timer_millisecond.timer_flags;
	volatile struct timers *ptrms = &timer_second.timer_flags;
    ticks++;
	calc_time_sec(ptrsec,ticks);
	calc_time_ms(ptrms,ticks);
	calc_time_sec_obj(ticks);
}

uint8_t set_timer_sec(int num)							//отсчет с 0 до 7
{	
	if(num>7) return -1;
	return (timer_millisecond.build & (1<<num))?1:0;
}

void clr_flag_timer_sec(int num)
{
	timer_millisecond.build &= ~(1<<num);
}

uint8_t set_timer_ms(int num)							//отсчет с 0 до 7
{
	if(num>7) return -1;
	return (timer_second.build & (1<<num))?1:0;
}

void clr_flag_timer_ms(int num)
{
	timer_second.build &= ~(1<<num);
}

int8_t set_timer_sec_obj(int num)							//отсчет с 0 до 7
{
	uint8_t *tme=(uint8_t *)&obj_timers;
	if(num>7) return -1;
	return (*tme & (1<<num))?1:0;
}

void clr_flag_timer_sec_obj(int num)
{
	uint8_t *tme=(uint8_t *)&obj_timers;
	*tme &= ~(1<<num);
}

uint16_t impl_timer_sec(uint8_t num, uint8_t consta, uint8_t shift)	//constanta = шагу						
{
	Septimers *ptr = septimers+num;
	if(set_timer_sec(num+shift)){						//+7 -> системные таймеры 10-ти секундные начинаются с 7-го
		ptr->InsideCntTimer++;
		clr_flag_timer_sec(num+shift);					//сбрасываем таймер
	}
	return ptr->InsideCntTimer*consta;
}

uint16_t impl_timer_sec_obj(uint8_t num, uint8_t consta, uint8_t shift)	//constanta = шагу
{
	Septimers *ptr = obj_septimers+num;
	if(set_timer_sec_obj(num+shift)){						//шаг 5-ть секунд; +7 -> системные таймеры 5-ти секундные начинаются с 7-го
		ptr->InsideCntTimer++;
		clr_flag_timer_sec_obj(num+shift);					//сбрасываем таймер
	}
	return ptr->InsideCntTimer*consta;
}

void user_timer_with_step_two_sec(uint8_t EN, uint8_t num, uint16_t setting)
{		
	Septimers *ptr = septimers+num;
	if(!EN) return;
	if(impl_timer_sec(num,2,SHIFT5)==0) return;				
	if((impl_timer_sec(num,2,SHIFT5)%setting)==0) {
		ptr->FlagSetTimer=1;
		//*ptr |= (1<<num);
		ptr->InsideCntTimer=0;
	}
}

void user_timer_with_step_one_sec(uint8_t EN,uint8_t num, uint16_t setting)
{
	Septimers *ptr = septimers+num;
	if(!EN) return;
	if(impl_timer_sec(num,1,SHIFT0)==0) return;				
	if((impl_timer_sec(num,1,SHIFT0)%setting)==0) {
		ptr->FlagSetTimer=1;
		//*ptr |= (1<<num);
		ptr->InsideCntTimer=0;
	}	
}

void clear_user_timer(uint8_t num)
{
	//uint8_t *ptr = (uint8_t *)&septimers;
	Septimers *ptr = septimers+num;
	//*ptr &= (1<<num);
	ptr->FlagSetTimer=0;
	ptr->InsideCntTimer=0;
	clr_flag_timer_ms(7);					//сбрасываем системный таймер
}

void user_timer_with_step_two_sec_obj(uint8_t EN, uint8_t num, uint16_t setting)
{		
	Septimers *ptr = obj_septimers+num;
	if(!EN) return;
	if(impl_timer_sec_obj(num,1,SHIFT0)==0) return;				
	if((impl_timer_sec_obj(num,1,SHIFT0)%setting)==0) {
		ptr->FlagSetTimer=1;
		ptr->InsideCntTimer=0;
	}
}

void user_counter(uint8_t EN,uint8_t num,uint16_t setting)
{
	Septimers *ptrcnt = sepcnt+num;
	if(!EN) return;
	ptrcnt->InsideCntTimer++;
	if(ptrcnt->InsideCntTimer>=setting) {
		ptrcnt->FlagSetTimer=1;
		ptrcnt->InsideCntTimer=0;
	}
}
