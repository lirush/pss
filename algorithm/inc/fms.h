#ifndef FMS_H
#define FMS_H

typedef struct{
    uint8_t step;
    //uint8_t event;
    void (*EventFunc)();
}Transition;

/*
Функция перехода КА
входные параметры:
    int elementsCount - количество элементов таблицы переходов
    Transition *transitionTable - указатель на таблицу переходов
    State currentState - текущее состояния КА
    Event eventToFire - событие для КА
*/
signed char FMSFunc(int elementsCount, Transition *transitionTable, uint8_t currentState);

#include ".\algorithm\src\fms.c"

#endif          //FMS_H

