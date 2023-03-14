//===================================================
#ifndef mRTOS
#define mRTOS
//===================================================
// Определение типа контекста задачи
typedef char switch_buf[2];
//===================================================
// Перечисление - статус задачи
enum StateTask { NoInit, Active, Suspend, Wait, Semaphore, StopTask };
//===================================================
// Перечисление - статус прложения
enum StateOS { Stop, Start };
//===================================================
#pragma used+
// Объявления функций mRTOS
// Инициализация mRTOS
void Init_mRTOS();  
// Ф-ции планировщика задач.
void Sheduler();           
void jmp_task(switch_buf cpu_state);
void save_reg(void (*t)(), switch_buf cpu_state);
void create_task(void(*t)(), char pri, enum StateTask st);
void wait_task(unsigned int delay, switch_buf cpu_state);
void dispatch_task( switch_buf cpu_state );
// Ф-ции для работы с событиями.
void init_event(char number_event);
void set_event(char number_event);
void set_event_value(char number_event, char value);
void clear_event(char number_event);
char get_event(char number_event);
char pop_event(char number_event);
void set_status_task(enum StateTask status); 
void setN_status_task(char number_task, enum StateTask status); 
// Ф-ции системного времени.
void inc_systime();
unsigned long int get_systime();
// Нулевая задача - процесс холостого хода.
void idle();
#pragma used-
//===================================================
// Task Control Block - структура блока контроля задачи
struct TCB {
    char pri;             // приоритет задачи
    char current_pri;     // текущий приоритет задачи
    enum StateTask st;    // текущее состояние задачи 
    int delay;            // время задержки в тиках в состоянии задачи Wait
    switch_buf cpu_state; // контекст задачи
           };
//===================================================
// Event Control Block - структура блока контроля события
struct ECB {
    char number_task;        // номер задачи закрепленной за событием 
    char flag_control_event; // флаг установлен - событие включено (ожидание события)
    char flag_event;         // флаг установлен - событие произошло  
           };
//===================================================
// Макросы mRTOS
// Количество пользовательских задач в приложении
#define APPTASKS 8        
// Общее количество задач в приложении
#define MAXTASKS APPTASKS+1        
// К-во событий в приложении
#define MAXEVENTS 2        
// Application Task Control Block - массив структур TCB всех задач приложения.
#define ATCB struct TCB tasks[MAXTASKS];                                  
// Event Task Control Block - массив структур ECB приложения.
#define AECB struct ECB events[MAXEVENTS];                                  
// Макрос - контекст текущей задачи                  
#define CPU_STATE (tasks[current_task].cpu_state)
// Макрос - переключатель задач
#define DISPATCH dispatch_task(CPU_STATE);
#define WAIT(X) wait_task(X, CPU_STATE);
#define ACTIVE(X) setN_status_task(X, Active);
#define STOP set_status_task(StopTask); DISPATCH;
//===================================================
// К-во инициализированных задач в приложении
extern char init_tasks;                         
// Статус приложения
extern enum StateOS State;
// Номер текущей задачи
extern char current_task;                              
// Массив структур TCB
extern struct TCB tasks[MAXTASKS];                                  
// Массив структур ECB
extern struct ECB events[MAXEVENTS];                                  
//===================================================
#include ".\mRTOS\src\mrtos.c"
#endif
//===================================================
