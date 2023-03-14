#include <stdio.h>
#include "j1939.h"
#include "queue.h"

extern uint8_t implementation;

/*
void uart_send_text_ANDI(void)                                //функция - исключительно для отладочных целей
{
    char  tmp[100] = {};
    uint16_t Out[CH_ADC_num];
    uint16_t tmp_d[CH_ADC_num];
    for (uint8_t i=0 ; i<CH_ADC_num;i++)
    {
        cli();
        Out[i]=ADC_converter[i];
        sei();
        tmp_d[i]=(int)(Out[i]>>FILTER_LEVEL);
    }
    sprintf(tmp,"\r$AN\t%06d \t%06d \t%06d \t%06d \t%06d \t%06d \t%s",tmp_d[0],tmp_d[1],tmp_d[2],tmp_d[3],tmp_d[4],tmp_d[5],GetResult());
    USART_send(U1,(uint8_t *)tmp,strlen(tmp));
}*/

FrDataTypeDef local1;
void vJ1939(uint8_t ch_num, J1939_params params, uint8_t priority, uint8_t kind_of_msg, uint8_t *data, uint8_t size)
{
    uint32_t id=0;
    J1939_standart tmp;
    RX_Bitfields head;
	if(size<=0) return;
    
    tmp.priority = priority;
    tmp.reserv=0;
    tmp.ParamGroupNum = params.base_id+kind_of_msg;
    tmp.ToWhom = params.addr_dest;
    tmp.FromWhom = params.addr_source;
    tmp.zero=0; 
    memcpy(&id,&tmp,sizeof(id));
    //SetRX_id(&head,id);
    head.RXBSidlIDE=EXTEND;
	head.RXBDlcDLC = size;							//размер данных
	head.RXBDlcRTR=0;
    head.RXBSidlSRR=0; 
    head.RXBSidh=(uint8_t)((uint32_t)id>>BIT_21);										//в расширенном формате старшие 11 бит - стандартный идентификатор
    head.RXBSidlEID=(uint8_t)((uint32_t)id>>BIT_16);
    head.RXBSidlSID=(uint8_t)((uint32_t)id>>BIT_18);
    head.RXBEid8=(uint8_t)((uint32_t)id>>BIT_8);
    head.RXBEid0=(uint8_t)id;       
    memcpy(&local1.data,&head,5);
    memcpy(local1.data+5,data,size);
    local1.ch_can=ch_num;
    local1.flagFull=1; local1.event=1;
    vAdd(&RXQueue,&local1);

//    vSendMsg(ch_num,id,EXTEND,data,size);
}



