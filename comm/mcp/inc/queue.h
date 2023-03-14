/**
  ******************************************************************************
  * @file    queue.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием функций очереди
  ******************************************************************************
*/
#ifndef __QUEUE_H
#define __QUEUE_H

#define MAX_LENGHT_Que   (100)      /*! Глубина очереди*/

/*!
  \class FrDataTypeDef
  Фрейм одной строки очереди
*/
typedef struct{
    uint8_t ch_can;
    uint8_t event;
    uint8_t flagFull;
    uint8_t data[13];
}FrDataTypeDef;

/*!
  \struct QueueStruct
  Структура очереди
*/
struct QueueStruct{
    FrDataTypeDef data[MAX_LENGHT_Que];
    uint8_t last;
};

extern struct QueueStruct RXQueue;
//extern volatile FrDataTypeDef local;

/*!
  Инициализация очереди
  
  \param QueueStruct* q указатель на структуру очереди 
*/
void vCreation(struct QueueStruct* q);

/*!
  Удаление элемента очереди
  
  \param QueueStruct* q указатель на структуру очереди
*/
void vDelete(struct QueueStruct* q);

/*!
  Забрать верхний элемент очереди
  
  \param QueueStruct* q указатель на структуру очереди
*/
void pxTop(struct QueueStruct* q);

/*!
  Добавить в очередь элемент
  
  \param QueueStruct* q указатель на структуру очереди
  \param *data указатель на фрейм данных
*/
void vAdd(struct QueueStruct* q,FrDataTypeDef *data);

#include "./comm/mcp/src/queue.c"

#endif