#include <stdio.h>
#include "queue.h"
#include "mspi.h"

struct QueueStruct RXQueue;
volatile FrDataTypeDef local;

void vCreation(struct QueueStruct* q)
{
    q->last=0;
}

void vAdd(struct QueueStruct* q,FrDataTypeDef *data)
{
    uint8_t last = q->last;
    if(q->last==MAX_LENGHT_Que)return;
    //q->data[last] = data;
    memcpy(&q->data[last],data,sizeof(FrDataTypeDef));
    q->last++;
}

void pxTop(struct QueueStruct* q)
{
//    FrDataTypeDef *local = (FrDataTypeDef *)malloc(sizeof(FrDataTypeDef));
    local = q->data[0];
//    memcpy(local,&q->data[0],sizeof(FrDataTypeDef));
//    return local;
}

void vDelete(struct QueueStruct* q)
{
    int i=0;
    if(q->last==0)return;
    for(i=0;(i<q->last && i<MAX_LENGHT_Que);i++)
        q->data[i] = q->data[i+1];
    
    q->last--;

}

uint8_t uSize(struct QueueStruct* q)
{
 return q->last;
}