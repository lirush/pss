/**
  ******************************************************************************
  * @file    queue.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� ������� �������
  ******************************************************************************
*/
#ifndef __QUEUE_H
#define __QUEUE_H

#define MAX_LENGHT_Que   (100)      /*! ������� �������*/

/*!
  \class FrDataTypeDef
  ����� ����� ������ �������
*/
typedef struct{
    uint8_t ch_can;
    uint8_t event;
    uint8_t flagFull;
    uint8_t data[13];
}FrDataTypeDef;

/*!
  \struct QueueStruct
  ��������� �������
*/
struct QueueStruct{
    FrDataTypeDef data[MAX_LENGHT_Que];
    uint8_t last;
};

extern struct QueueStruct RXQueue;
//extern volatile FrDataTypeDef local;

/*!
  ������������� �������
  
  \param QueueStruct* q ��������� �� ��������� ������� 
*/
void vCreation(struct QueueStruct* q);

/*!
  �������� �������� �������
  
  \param QueueStruct* q ��������� �� ��������� �������
*/
void vDelete(struct QueueStruct* q);

/*!
  ������� ������� ������� �������
  
  \param QueueStruct* q ��������� �� ��������� �������
*/
void pxTop(struct QueueStruct* q);

/*!
  �������� � ������� �������
  
  \param QueueStruct* q ��������� �� ��������� �������
  \param *data ��������� �� ����� ������
*/
void vAdd(struct QueueStruct* q,FrDataTypeDef *data);

#include "./comm/mcp/src/queue.c"

#endif