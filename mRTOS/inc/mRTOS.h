//===================================================
#ifndef mRTOS
#define mRTOS
//===================================================
// ����������� ���� ��������� ������
typedef char switch_buf[2];
//===================================================
// ������������ - ������ ������
enum StateTask { NoInit, Active, Suspend, Wait, Semaphore, StopTask };
//===================================================
// ������������ - ������ ���������
enum StateOS { Stop, Start };
//===================================================
#pragma used+
// ���������� ������� mRTOS
// ������������� mRTOS
void Init_mRTOS();  
// �-��� ������������ �����.
void Sheduler();           
void jmp_task(switch_buf cpu_state);
void save_reg(void (*t)(), switch_buf cpu_state);
void create_task(void(*t)(), char pri, enum StateTask st);
void wait_task(unsigned int delay, switch_buf cpu_state);
void dispatch_task( switch_buf cpu_state );
// �-��� ��� ������ � ���������.
void init_event(char number_event);
void set_event(char number_event);
void set_event_value(char number_event, char value);
void clear_event(char number_event);
char get_event(char number_event);
char pop_event(char number_event);
void set_status_task(enum StateTask status); 
void setN_status_task(char number_task, enum StateTask status); 
// �-��� ���������� �������.
void inc_systime();
unsigned long int get_systime();
// ������� ������ - ������� ��������� ����.
void idle();
#pragma used-
//===================================================
// Task Control Block - ��������� ����� �������� ������
struct TCB {
    char pri;             // ��������� ������
    char current_pri;     // ������� ��������� ������
    enum StateTask st;    // ������� ��������� ������ 
    int delay;            // ����� �������� � ����� � ��������� ������ Wait
    switch_buf cpu_state; // �������� ������
           };
//===================================================
// Event Control Block - ��������� ����� �������� �������
struct ECB {
    char number_task;        // ����� ������ ������������ �� �������� 
    char flag_control_event; // ���� ���������� - ������� �������� (�������� �������)
    char flag_event;         // ���� ���������� - ������� ���������  
           };
//===================================================
// ������� mRTOS
// ���������� ���������������� ����� � ����������
#define APPTASKS 8        
// ����� ���������� ����� � ����������
#define MAXTASKS APPTASKS+1        
// �-�� ������� � ����������
#define MAXEVENTS 2        
// Application Task Control Block - ������ �������� TCB ���� ����� ����������.
#define ATCB struct TCB tasks[MAXTASKS];                                  
// Event Task Control Block - ������ �������� ECB ����������.
#define AECB struct ECB events[MAXEVENTS];                                  
// ������ - �������� ������� ������                  
#define CPU_STATE (tasks[current_task].cpu_state)
// ������ - ������������� �����
#define DISPATCH dispatch_task(CPU_STATE);
#define WAIT(X) wait_task(X, CPU_STATE);
#define ACTIVE(X) setN_status_task(X, Active);
#define STOP set_status_task(StopTask); DISPATCH;
//===================================================
// �-�� ������������������ ����� � ����������
extern char init_tasks;                         
// ������ ����������
extern enum StateOS State;
// ����� ������� ������
extern char current_task;                              
// ������ �������� TCB
extern struct TCB tasks[MAXTASKS];                                  
// ������ �������� ECB
extern struct ECB events[MAXEVENTS];                                  
//===================================================
#include ".\mRTOS\src\mrtos.c"
#endif
//===================================================
