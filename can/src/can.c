/**
  ******************************************************************************
  * @file    can.c
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Исходный файл содержит функции CAN интерфейса
  ******************************************************************************
*/
#include "can.h"
#include "mcp.h"
#include "queue.h"

/*!
    Обработчик прерывания контроллера CAN1
    
    Оповещает о вновь пришедших данных в CAN1
*/
interrupt [EXT_INT5] void ext_int5_isr(void)
{
    RX_control *rx_ptr = &CanEx[CAN1].RX_example[RX0];
    memset(&rx_ptr->HeaderData.Received,0x00,sizeof(rx_ptr->HeaderData.Received));        //перед приемом нового пакета, стираем старое
    rx_ptr->IntFlag=1;                                            //в буфере контроллера CAN есть непрочитанные данные
    EIMSK &= ~(1<<INT5);
}

/*!
    Обработчик прерывания контроллера CAN2
    
    Оповещает о вновь пришедших данных в CAN2
*/
interrupt [EXT_INT4] void ext_int4_isr(void)
{
    RX_control *rx_ptr = &CanEx[CAN2].RX_example[RX0];
    memset(&rx_ptr->HeaderData.Received,0x00,sizeof(rx_ptr->HeaderData.Received));        //перед приемом нового пакета, стираем старое
    rx_ptr->IntFlag=1;                                            //в буфере контроллера CAN есть непрочитанные данные  
    EIMSK &= ~(1<<INT4);
}

void vCanReceive(uint8_t ch)
{
    uint8_t com_read[2]; 
    FrDataTypeDef local;
    RX_control *pRX0 = &CanEx[ch].RX_example[RX0];
    RX_control *pRX1 = &CanEx[ch].RX_example[RX1]; 
    //if(pRX0->IntFlag)
    {
        pRX0->IntFlag=0;
        com_read[0] = READSTATUS;
        exchange_read(ch,com_read,2);
        if(com_read[1] & 0x01)
        {
            pRX0->HeaderData.com_read= READRX0DH;          
            exchange_read(ch,(uint8_t *)&pRX0->HeaderData,sizeof(pRX0->HeaderData));
            local.ch_can=ch;
            local.event = 0;
            local.flagFull=1;
            memcpy(&local.data,&pRX0->HeaderData.Received.whoIsHe.whoIsHe_build,sizeof(IdReceive));
            memcpy(local.data+5,&pRX0->HeaderData.Received.buf,LEN_DATA_CAN);
            vAdd(&RXQueue,&local);
            pRX0->MsgReceive=1;          
        }            
        if(com_read[1] & 0x02)
        {
            pRX1->HeaderData.com_read= READRX1DH;          
            exchange_read(ch,(uint8_t *)&pRX1->HeaderData,sizeof(pRX1->HeaderData));
            local.ch_can=ch;
            local.event = 0;
            local.flagFull=1;
            memcpy(&local.data,&pRX1->HeaderData.Received.whoIsHe.whoIsHe_build,sizeof(IdReceive));
            memcpy(local.data+5,&pRX1->HeaderData.Received.buf,LEN_DATA_CAN);                 
            vAdd(&RXQueue,&local);
            pRX1->MsgReceive=1;           
        }                             
       //EIMSK |= (1<<INT5)>>ch;   
    }    
}
//
//uint8_t uCheckID(uint32_t id)
//{
//    J1939_standart header;
//    memcpy(&header,&id,sizeof(uint32_t));
//    if((id==0x00000080)
//        || (id==0x00000081)
//        || (id==0x00000082)
//        || (id==0x00000083) 
//        || (header.ParamGroupNum==PGNDFM)
//        || (header.ParamGroupNum==PGNDFMCOM)
//        || (header.ParamGroupNum==PGNDFMU)
//        || (header.ParamGroupNum==PGNDFMA)
//        || (header.ParamGroupNum==PGNGOMPPRESS)
//		|| (header.ParamGroupNum==PGNGOMPTEMP)
//		|| (header.ParamGroupNum==PGNGOMPTELE)
//        ||(header.ParamGroupNum==PGNGOMPFRFREQ)
//		|| (header.ParamGroupNum==PGNGOMP)
//		|| (header.ParamGroupNum==PGNGOMPTIME))        
//     return 1;
//     else return 0;   
//}