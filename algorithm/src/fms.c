#include "fms.h"

signed char FMSFunc(int elementsCount, Transition *transitionTable, uint8_t currentState)   //, uint8_t eventToFire
{
    Transition transition;
    while (elementsCount--) 
    {
        transition = transitionTable[elementsCount];
        if (transition.step == currentState)// && (transition.event == eventToFire)) 
        {
            //return 
            transition.EventFunc();
            //break;
        }
    }
    return 0;    
}
