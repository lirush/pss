/**
  ******************************************************************************
  * @file    mcp.c    
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Исходный файл содержит функции инициализации, передачи пакетов данных в CAN шину 
  ******************************************************************************
*/
#include <string.h>
#include "def.h"
#include "mcp2515.h"
#include "mcp.h"
#include "mcp_implement.h"
#include "mspi.h"

CAN_ch CanEx[CH_quantity];
uint8_t RegStatusErr[8];
extern MaskAndFilter MaskFilterEx[2];

/*!
  Настраивает скорость обмена CAN шине
  
  \param ch_num номер CAN контроллера
  \param Speed выбор скорости обмена 
*/
void SetSpeed(int ch_num,speed Speed){
	if (ch_num>=CH_quantity)
	return;
	CanEx[ch_num].rate=Speed;
	SetImplSpeed(ch_num);
}

/*!
  Настраивает маски и фильтры приемных буферов
  
  \param ch_num номер CAN контроллера
  \param on_mask включение маски (т.е соббщения будут фильтроваться по маске и фильтрам) 
*/
void SetRx(int ch_num, uint8_t on_mask)
{
	if (ch_num>=CH_quantity)
	return;
	SettingsRx(ch_num,on_mask,
	MaskFilterEx[ch_num].Mask_[0],
	MaskFilterEx[ch_num].filter[0],
	MaskFilterEx[ch_num].filter[1],
	MaskFilterEx[ch_num].Mask_[1],
	MaskFilterEx[ch_num].filter[2],
	MaskFilterEx[ch_num].filter[3],
	MaskFilterEx[ch_num].filter[4]);	
}

void vSendMsg(uint8_t ch_num, uint32_t id, uint8_t ex, uint8_t *data, uint8_t size)
{
    static uint8_t tx_num=0;
    uint16_t t=500;
    CAN_ch *pCAN = CanEx+ch_num;
    TX_control *pTX = pCAN->TX_example+tx_num;
    if (ch_num>=CH_quantity)
	return;
    memset(&pTX->whatToSend,0x00,sizeof(pTX->whatToSend));               //clear перед отправкой новых данных
	pTX->whatToSend.whoIam.TXBSidlEXIDE = ex;							//режим данных стандартный - 0; расширенный - 1
	pTX->whatToSend.whoIam.TXBDlcDLC = size;							//размер данных
	pTX->whatToSend.whoIam.TXBDlcRTR=0;
	SetTX_id(&pTX->whatToSend.whoIam,id);								//формируем id по битам
    pTX->whatToSend.CommandToSend = LOADTX0DH | (tx_num<<1);		    //инструкция загрузки буфера начиная с регистра TXBnDH, где n-номер буфера 
    memcpy(pTX->whatToSend.data,data,sizeof(pTX->whatToSend.data));
    exchange(ch_num,(uint8_t *)&pTX->whatToSend,sizeof(pTX->whatToSend));
    pCAN->Request_To_Send.ToSend_build = 0x80 | (1<<tx_num);
    while(t--);
    exchange(ch_num,(uint8_t *)&pCAN->Request_To_Send.ToSend_build,sizeof(pCAN->Request_To_Send.ToSend_build));                                                    
    if(++tx_num>2){tx_num=0;}
      
}

void Init_CAN(int ch_num)
{
    uint16_t t=0;
	mcp_reset(ch_num);															//перед началом использования mcp, сбросить его на начальные установки
	SetRx(ch_num,MaskFilterEx[ch_num].MaskOn_00_Off_60);
	t=100;
	while(t--);	
	SetSpeed(ch_num,S_125k);													//настраиваем скорость каналов
	mcp_write(CanEx[ch_num].inter_rupt.interrupt_build,CANINTE,ch_num);			//настраиваем прерывания
}

void ReadStatus(uint8_t ch)
{
    static uint8_t data[7]={0,0,0,0,0,0,0};  
    uint8_t shift=0;
    data[0] = READ;
    data[1] = TEC;                  //addr 
    exchange_read(ch,data,4);
    data[4] = READ;
    data[5] = EFLG;                 //addr 
    exchange_read(ch,data+4,3);
    if(ch==CAN1)shift=3;
    RegStatusErr[0+shift] = data[2];             //TEC
    RegStatusErr[1+shift] = data[3];             //REC
    RegStatusErr[2+shift] = data[6];             //EFGL
    if(ch==CAN1)shift=1;
    else shift=0;
    if((data[6] & (1<<5)) || (data[2] > 128)  || (data[3] > 128))RegStatusErr[6+shift]=0x01;
}

void ResetStatus(uint8_t ch)
{
   uint8_t data[4];
   data[0] = WRITE;
   data[1] = TEC;                   //addr 
   data[2] = 0; data[3] =0; 
   exchange(ch,data,sizeof(data));
}