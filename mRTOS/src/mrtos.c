//===================================================
// mRTOS version 1.11 02.11.2009 г.
// © Copyright Movila V.N.
//===================================================
#include "mRTOS.h"
//===================================================
// Определяем Application Task Control Block - массив TCB структур для всех задач приложения.
ATCB;
// Определяем Application Event Task Control Block - массив ECB структур приложения.
AECB;
//===================================================
// К-во инициализированных задач в приложении
char init_tasks;                         
// Статус приложения
enum StateOS State;
// Номер текущей задачи
char current_task; 
// Глобальные переменные планировщика задач Sheduler().
char pri, i_shed, i_pri, factive, flStop = 0;   
// Переменная - время работы системы в системных тиках.
unsigned long int systime = 0;
//=================================================== 
void idle()
{           
set_status_task(Active);        
setN_status_task(0, Active);   
// Инициализируем событие 0 - ОТЛАДКА.
init_event(0); 
while(1) {  
    if(get_event(0)) {
        PORTD.4 = 0;
        WAIT(50);
        PORTD.4 = 1;
                     }
    DISPATCH;                           
         }
}
//=================================================== 
void Init_mRTOS()
{
char i;

for(i = 0;i<MAXTASKS;i++) {
    tasks[i].pri = 0;                          
    tasks[i].current_pri = 0;                          
    tasks[i].st = NoInit;                          
    tasks[i].delay = 0;                          
                          }
for(i = 0;i<MAXEVENTS;i++) {
    events[i].number_task = 0;
    events[i].flag_control_event = 0;
    events[i].flag_event = 0;                           
                           }    
init_tasks = 0;                         
State = Stop;
current_task = 0;                            
//init_event(0);
//set_event(0);
//pop_event(0);
//get_event(0);
//clear_event(0);
create_task(idle, 5, Active);
get_systime();
}
//===================================================
#pragma warn-
void jmp_task(switch_buf cpu_state)
{
#asm("cli")
#ifdef _MODEL_TINY_
#asm
    ld r26, y
    clr r27
#endasm
#else
#asm
    ld r26, y    
    ldd r27, y+1  
#endasm
#endif           
#asm    
    ld r24, x+
    ld r25, x
    pop r22
    pop r23
    push r24
    push r25  
    sei
#endasm                   
}
#pragma warn+
//===================================================
#pragma warn-
void save_reg(void (*t)(), switch_buf cpu_state)
{            
#asm("cli")
#ifdef _MODEL_TINY_
#asm
    ld r26, y
    clr r27
    ldd r24, y+1
    ldd r25, y+2
#endasm
#else
#asm
    ld r26, y    
    ldd r27, y+1  
    ldd r24, y+2
    ldd r25, y+3
#endasm
#endif           
#asm   
    st x+, r24
    st x, r25
    sei
#endasm                   
}
#pragma warn+
//===================================================
void create_task(void(*t)(), char pri, enum StateTask st)
{                                       
if(init_tasks >= MAXTASKS) return;
save_reg(t, tasks[init_tasks].cpu_state);
tasks[init_tasks].pri = pri;
tasks[init_tasks].current_pri = pri;
tasks[init_tasks++].st = st;             
}
//===================================================
#pragma warn-
void wait_task(unsigned int delay, switch_buf cpu_state)
{
tasks[current_task].st = Wait;
tasks[current_task].delay = delay;
#asm("cli")
#ifdef _MODEL_TINY_
#asm
    ld r26, y
    clr r27
#endasm
#else
#asm
    ld r26, y    
    ldd r27, y+1  
#endasm
#endif           
#asm   
    pop r24
    pop r25
    st x+, r25
    st x, r24    
#endasm      
#ifdef _MODEL_TINY_
#asm   
    adiw R28,3
    sei
#endasm      
#else
#asm   
    adiw R28,4
    sei
#endasm      
#endif           
Sheduler();
}
#pragma warn+
//===================================================
#pragma warn-
void dispatch_task( switch_buf cpu_state )
{
if(tasks[current_task].st == StopTask) flStop = 1;
#asm("cli")
#ifdef _MODEL_TINY_
#asm
    ld r26, y
    clr r27
#endasm
#else
#asm
    ld r26, y    
    ldd r27, y+1  
#endasm
#endif           
#asm   
    pop r24
    pop r25
    st x+, r25
    st x, r24    
#endasm      
#ifdef _MODEL_TINY_
#asm   
    adiw R28,1
    sei
#endasm      
#else
#asm   
    adiw R28,2
    sei
#endasm      
#endif           
if(flStop == 1)
{
	flStop = 0;
	tasks[current_task].st = StopTask;
}
else tasks[current_task].st = Active;
Sheduler();
}
#pragma warn+
//===================================================
void Sheduler()
{                                    
#asm   
    cli
    pop r22
    pop r22
    sei
#endasm      

pri = 0;                        
factive = 0;

// При 1-ом входе в планировщик переводим приложение в состояние Start.
// И передаем управление 1-ой задаче.
if(State == Stop) { 
    State = Start;
    jmp_task(tasks[current_task].cpu_state);
                  }          
// Выполняем следующие 2 операции -
// 1. Поиск задачи в состоянии задержки - если находим, то переход на эту задачу,
//    невзирая на приоритеты.
// 2. Поиск задачи с наиболее высоким приоритетом и , соответственно, то переход на эту задачу.
--tasks[current_task].current_pri;
// Если хоть одна задача с приоритетом равным нулю, то восстанавливаем приоритеты ВСЕХ задач!
if(tasks[current_task].current_pri == 0) 
    for(i_shed = 0;i_shed<init_tasks;i_shed++) tasks[i_shed].current_pri = tasks[i_shed].pri;
if(tasks[current_task].st == Active) {
    tasks[current_task].st = Suspend;
    i_pri = current_task;
    factive = 1;
                                     }
for(i_shed = 0;i_shed<init_tasks;i_shed++) {
// Поиск задач в состоянии задержки.
    if(!tasks[i_shed].delay && tasks[i_shed].st == Wait) {
        pri = tasks[i_shed].current_pri;
        current_task = i_shed;                           
        break;
                                                         }                                
// Поиск задачи с наиболее высоким приоритетом.
    if(tasks[i_shed].current_pri >= pri && tasks[i_shed].st == Active) {
        pri = tasks[i_shed].current_pri;
        current_task = i_shed;                           
                                                                       }
                                           }
if(factive) tasks[i_pri].st = Active;
jmp_task(tasks[current_task].cpu_state);
}
//===================================================
void init_event(char number_event)
{
if(number_event > MAXEVENTS - 1) return;
events[number_event].number_task = current_task;
events[number_event].flag_control_event = 1;
events[number_event].flag_event = 0;
}
//===================================================
void set_event(char number_event)
{           
#asm("cli");
if(number_event > MAXEVENTS - 1) return;
if(events[number_event].flag_control_event) events[number_event].flag_event++;     
#asm("sei");
}
//===================================================
void set_event_value(char number_event, char value)
{
#asm("cli");
if(number_event > MAXEVENTS - 1) return;
if(events[number_event].flag_control_event) events[number_event].flag_event = value;     
#asm("sei");
}
//===================================================
void clear_event(char number_event)
{
#asm("cli");
if(number_event > MAXEVENTS - 1) return;
events[number_event].flag_control_event = 0;
events[number_event].flag_event = 0;
#asm("sei");
}
//===================================================
char get_event(char number_event)
{    
char ret;

#asm("cli");
if(number_event > MAXEVENTS - 1) ret = 0;
else if(events[number_event].flag_control_event) ret = events[number_event].flag_event;
events[number_event].flag_event = 0;
#asm("sei");
return ret;
}
//===================================================
char pop_event(char number_event)
{    
char ret;

#asm("cli");
if(number_event > MAXEVENTS - 1) ret = 0;
else if(events[number_event].flag_control_event) ret = events[number_event].flag_event;
#asm("sei");
return ret;
}
//===================================================
void set_status_task(enum StateTask status)
{
#asm("cli");
tasks[current_task].st = status;
#asm("sei");
}            
//===================================================
void setN_status_task(char number_task, enum StateTask status)
{
#asm("cli");
if(tasks[number_task].st == StopTask) tasks[number_task].st = status;
#asm("sei");
}            
//===================================================
void inc_systime()
{
systime++;
}
//===================================================
unsigned long int get_systime()
{  
return systime;
}
//===================================================
// End mRTOS
//===================================================
