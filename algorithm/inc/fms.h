#ifndef FMS_H
#define FMS_H

typedef struct{
    uint8_t step;
    //uint8_t event;
    void (*EventFunc)();
}Transition;

/*
������� �������� ��
������� ���������:
    int elementsCount - ���������� ��������� ������� ���������
    Transition *transitionTable - ��������� �� ������� ���������
    State currentState - ������� ��������� ��
    Event eventToFire - ������� ��� ��
*/
signed char FMSFunc(int elementsCount, Transition *transitionTable, uint8_t currentState);

#include ".\algorithm\src\fms.c"

#endif          //FMS_H

