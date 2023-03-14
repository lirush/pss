/**
*******************************************************
\mainpage 
��� ��������� ���� ����������� � ����� ����������������
CodeWizardAVR V3.12. \n
��������� ��� ������ ���������� �������� �������������� (��-���)
�� ���� ����������������� 1887��7� (������ ATmega128) \n
\n
��������� ������� �� ���� ������������� �� mRTOS.
http://www.movilavn.narod.ru 


Project : SEP \n
Version : 2.0.0 \n
Date    : 03.07.2020 \n
Author  : Murzina LH  \n
Company : AO NPP AME  \n
Comments: \n
\n
Chip type               : ATmega128 \n
Program type            : Application \n
AVR Core Clock frequency: 8,000000 MHz \n
Memory model            : Small \n
External RAM size       : 0 \n
Data Stack size         : 1024 \n
\n
*******************************************************
*/
/**
  ******************************************************************************
  * @file    sep.c
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    03.07.2020
  * @brief   �������� ���� ������� ���.
  ******************************************************************************
*/
#include <mega128.h>
#include "sep.h"

// Standard Input/Output functions
#include <stdio.h>

#define SIZE_MYCONFIG		(sizeof(ComConfig))
#define UART_IAM			(MyConfig.uart_part.addr)

ComConfig MyConfig;
J1939_params params, params2;
Prog_polar_ios prog_ios;
MaskAndFilter MaskFilterEx[2];
uint8_t implementation =0;
uint8_t CanDataTmp[30];

/*!
  ���������� ���������� ��������� ������� �� mRTOS.
  �������� �� ������ 1��.
*/
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
	unsigned char ii;
	#asm("cli")
    TCNT0=131;
	inc_systime();
	for (ii=0;ii<init_tasks;ii++)
	{
		if(tasks[ii].delay){tasks[ii].delay--;}
	}                          
    
    TIFR |= (1<<TOIE0);
	#asm("sei")
}

/*!
  ������������� ��������� ������� �� mRTOS.
  ������ 1��.
*/
void vInitTim0(void)
{
	TCNT0 = 131;                               //1��
	TCCR0 = 4;								   // ����������� �������� ������� 1024
	TIMSK |= (1 << TOIE0);					   // ���������� ���������� ���������� ��������
}

/*!
  ������������� ������� CAN ���������.
*/
void vInitQueue(void)
{
        vCreation(&RXQueue);
}

void vSendUART(uint8_t ch_uart,uint8_t dst,uint8_t src,uint8_t what)
{
	uint8_t tmp[BUFF_SIZE];
    uint8_t len=0; 
	tmp[0]=src;
	tmp[0] |= (dst<<4);
	tmp[1] = what;
	switch(what) {
        case SEND_TO:
            memcpy(tmp+LEN_HEAD_UART,DI_Out,sizeof(DI_Out));
            len = sizeof(DI_Out)+LEN_HEAD_UART;
            if(UART_IAM==BOARD1)
            {
                memcpy(tmp+len,CanDataTmp,30);               //��2 - 3-� �����, ��3 - 3-� �����, ��� - 2-� �����, ���1,2 - 6-�� ����, T - 2-� �����, P - 2-� �����
                len += 30;
            }
            if(UART_IAM==BOARD4)
            {
               memcpy(tmp+len,OutDataForCantmp,8);
               len += 8; 
            }
        break;
	}
	USART_send(ch_uart,tmp,len);
}

/*!
  ������������ �����.
*/
void vRunConfig(uint8_t num)
{
	int i=0; uint16_t k=0;
	uint8_t *ptr = (uint8_t *)&configurations[num];
	uint8_t *ptr_conf = (uint8_t *)&MyConfig;
	for(i=0; i<SIZE_MYCONFIG;i++) {							//������ ������������ �� flash (program flash)
		ptr_conf[i] = pgm_read_byte(ptr+i);
		k=1000;
		while(k--);
	}
	params.base_id=MyConfig.can_part.CAN_sender.BaseParamGroup;
	params.addr_source = MyConfig.can_part.CAN_sender.SourceAddr;
	params.addr_dest = MyConfig.can_part.CAN_sender.DestAddr;
	params2 = params;
	params2.base_id += 3;
	if(num == 4) params2.base_id=52183;	
	if(num == 5) params2.base_id=52080;    
	prog_ios = MyConfig.polar_ios;
	MaskFilterEx[0] = MyConfig.can_part.mask_filter[0];
	MaskFilterEx[1] = MyConfig.can_part.mask_filter[1];
}

void vReconfigOnPortF(void)
{
    uint8_t scan_portf=0;
    int i=0;
	if(MyConfig.whoAmI.MasterSlave!=MASTER){
		PORTF = 0xFF;
		for(i=0;i<7;i++){									//��������� ������ 6-�� ����� ����� 
			if(PINF & (1<<i))
			scan_portf++;
		}
		if(scan_portf>=5){										//���� ����� ������������, ���� ����� �� ����� ���. "1" -> ����� ������
			implementation=9;										//9- ������ ������������ ����� ������. conf.c
			vRunConfig(implementation);							//�����������������		
		}
		PORTF = 0x00;
	}
}

/*!
  ��������� ���������, �������� �� CAN ����
*/
void vParcingCanData(void)
{
    IdReceive head; 
    J1939_standart header;
    uint32_t id=0;
    uint16_t pgn;
    
    pxTop(&RXQueue);
    if(local.flagFull)
    { 
        local.flagFull=0;
        memcpy(&head,local.data,sizeof(head));
        id = unpack_id((uint8_t *)&head);
        memcpy(&header,&id,sizeof(uint32_t));			            //����������� � ���������
        pgn = header.ParamGroupNum;            
        switch(pgn)
        {
            case PGNEBUSPEED:
                memcpy(CanDataTmp+6,local.data+LEN_HEAD_CAN+3,2);                  //3-��, 4-�� ���� - ������� ���������
                vDelete(&RXQueue);
            break;
            case PGNPKI1:
                memcpy(CanDataTmp+8,local.data+LEN_HEAD_CAN,LEN_DATA_CAN);
                vDelete(&RXQueue);
            break;
            case PGNPKI2:
                memcpy(CanDataTmp+16,local.data+LEN_HEAD_CAN,LEN_DATA_CAN);
                vDelete(&RXQueue);
            break;
            case PGNBRI1:
                memcpy(CanDataTmp+24,local.data+LEN_HEAD_CAN+4,4);               //4-��, 5-�� ���� - ����������� ��, 6-��, 7-�� - �������� ����� 
                vDelete(&RXQueue);
            break;
            case PGNDSSEP2:
                memcpy(CanDataTmp,local.data+LEN_HEAD_CAN,3);
                vDelete(&RXQueue);
            break; 
            case PGNDSSEP3:
                memcpy(CanDataTmp+3,local.data+LEN_HEAD_CAN,3);
                vDelete(&RXQueue);            
            break;  
            case PGNPMVOFFSEP:
                memcpy(CanDataTmp+28,local.data+LEN_HEAD_CAN,2);
                vDelete(&RXQueue);              
            break;                          
        }
          
    }     
}

void vParcingUart(uint8_t ch)
{
    UART_basket *ptr = UART_RX_basket+ch; 
    uart_ntw_level tmp;
    uint8_t len = ptr->size_data;
    uint8_t src =0;
    tmp = unpack_msg_uart(ptr->buf);
    src =  (tmp.srcdst & 0x0F)-1;
    if(!ptr->fl_state.msg_receive) return;
    switch(tmp.what_isit)
    {
        case SEND_TO:
           memcpy(BoardNormal[src].In,ptr->buf+LEN_HEAD_UART,len-LEN_HEAD_UART);
        break;       
    }       
    ptr->fl_state.msg_receive=0;        
}

/*!
  ��������� � ������� CAN ��������� ��� �������� �� ���������� � ���������� ������� SLAVE  � MASTER ����
*/
void vAddQueueCAN(void)
{
    static uint8_t flip_flop=0;
    uint8_t src=0; 
    if(flip_flop)flip_flop=0;
    else flip_flop=1; 
    switch(flip_flop)
    {
     case 0:
        {
            vJ1939(CAN2,params,3,DI1,DI_Out,sizeof(DI_Out));
            if(UART_IAM == BOARD1)
            {
                src=BOARD4-1;
                vJ1939(CAN2,params2,4,AN1FROM1TO4,BoardNormal[src].In+3,LEN_DATA_CAN);
            }
            if(UART_IAM == BOARD2)src=BOARD3-1;
            vJ1939(CAN2,params2,3,DI1,BoardNormal[src].In,sizeof(DI_Out));             
        }                
     break;
     case 1:
   
     break;                                            
    }   
} 

/*!
  �������� ��������� � CAN ���� �� �������
*/
void vSendCAN(void)
{
    IdReceive head; 
    uint32_t id=0;
    uint8_t ex=0, len=0;     
    static uint8_t cnt_task=0;
    static uint8_t cnt_id_dis=0;    
//    FrDataTypeDef local;
//    FrDataTypeDef *local1;
//        local1 = pxTop(&RXQueue);
//        memcpy(&local,local1,sizeof(local));                                          
        pxTop(&RXQueue);
        if(local.flagFull)
        {           
            cnt_task++;
            local.flagFull=0;
            vDelete(&RXQueue);
            memcpy(&head,local.data,sizeof(head));            
            id = unpack_id((uint8_t *)&head);
            ex = head.whoIsHe.RXBSidlIDE;
            len = head.whoIsHe.RXBDlcDLC;  
            if(local.event==0)                                  //���� �������==0 -> �������� ����
            {
                if(local.ch_can==CAN1)local.ch_can=CAN2;
                else local.ch_can=CAN1;
            }
            if(id== 0x0CF00400) cnt_id_dis++;
            if(((UART_IAM==BOARD1) && (local.event==1)) || (UART_IAM==BOARD2))
            { 
                if(id != 0x0CF00300) 
                {
                    if(id != 0x0CF00400)
                    {
                        vSendMsg(local.ch_can,id,ex,local.data+5,len);
                    }    
                    else
                    {
                        if((cnt_task<10) && (cnt_id_dis<=1))
                        {
                          vSendMsg(local.ch_can,id,ex,local.data+5,len);
                        }
                    }
                }
            }
            if(cnt_task>=10){cnt_task=0; cnt_id_dis=0;}
            local.event=0;  
        } 
//        free(local1);
}

uint8_t class_cicle=CLASS_DEF;
/*!
  ���������� ����� �����.
*/
uint8_t class_recognition(void)
{
	if(implementation==9)
	class_cicle = CLASS_KEYS;
	if(implementation==4)
	class_cicle= CLASS_SEPCAN_1;
	if(implementation==5)
	class_cicle = CLASS_SEPCAN_2;
	if(implementation==7)
	class_cicle = CLASS_SEPNOTCAN;
	return class_cicle;
}

void vTaskDIRead(void)
{
    while(1)
    {
       Get_DISensors();
       WAIT(100); 
    }
}

void vTaskDORead(void)
{
    while(1)
    {
       Get_StateOuts();
       WAIT(100); 
    }
}

void vTaskLoadU1(void)
{
 while(1)
 {
  putuart(U1);
  DISPATCH;
 }
}

void vTaskLoadU0(void)
{
 while(1)
 {
  putuart(U0);
  WAIT(10);
 }
}

void vTaskAdddMsgCAN(void)
{
 while(1)
 {
    vAddQueueCAN();
    WAIT(100);
 }
}

void vTaskParcingAddMsgCAN(void)
{
    while(1)
    {
        vParcingCanData();
        WAIT(10);    
    }
}

void vTaskCANMessage(void)
{
    while(1)
    {   
        vSendCAN();
        WAIT(10);  
    }
}

void vTaskReceiveUart(void)
{
 UART_basket *ptr = UART_RX_basket+U1;
    while(1)
    {
        {                                 
            UCSR1B |= (1<<RXCIE1);
            while(!ptr->fl_state.msg_receive){DISPATCH};
            vParcingUart(U1);
            UCSR1B &= ~(1<<RXCIE1);
        }
        WAIT(10);        
    } 
}

void vTaskReceiveUart0(void)
{
 UART_basket *ptr = UART_RX_basket+U0;
    while(1)
    {
        {                                 
            UCSR0B |= (1<<RXCIE0);
            while(!ptr->fl_state.msg_receive){DISPATCH};
            vParcingUart(U0);
            UCSR0B &= ~(1<<RXCIE0);
        }
        WAIT(1);        
    } 
}

void vTaskUART1Send(void)
{
    while(1)
    {      
        vSendUART(U1,BOARD4,UART_IAM,SEND_TO);
        WAIT(10);
    }
}

void vTaskUART0Send(void)
{
    while(1)
    {      
        vSendUART(U0,BOARD4,UART_IAM,SEND_TO);
        WAIT(10);
    }
}

void vTaskCanReceive(void)
{
    while(1) 
    {                
        vCanReceive(CAN1);
        vCanReceive(CAN2);   
        WAIT(10);
    }
}

void vTaskAlgorithmSep(void)
{
   while(1)
   {
        count_ticks();              
        alg_sep();      
        WAIT(100);
   }
}

void vTaskReadStatusCan(void)
{
    uint32_t id=0;
    while(1)
    {
        RegStatusErr[6]=0; RegStatusErr[7]=0;   
        ReadStatus(CAN1);
        ReadStatus(CAN2);
        if(RegStatusErr[6]==0x01){Init_CAN(CAN2);}
        if(RegStatusErr[7]==0x01){Init_CAN(CAN1);}               
        WAIT(200);
    }
}

/*!
  �������� �������.
  1. ��������� ������������ �����
  2. �������� ��������������� ������������ � ����������� ����� ��� ���
  3. ���������� ����� �����: CLASS_BRI, CLASS_DEF
  4. ����������� ���������� ������� ������
  5. ����������� ���������� ������
  6. ����������� UART ���������
  7. � ������������� � ��������� ������� �����������: SPI ���������, CAN ���������, ������ CAN ��������� � ������������� SLAVE ����� ����� UART
  8. �������������� ��������� �� mRTOS
  9. ������� ������ ������������ ���������� � ���������� �������
  10. ������� ������ ����� �� UART ���������, ������ �� ����������� ��������� u757
  11. ������� ������ �������� ������ � UART �� ��������� u757   
  12. ������� ������ ���������� � ������� CAN ��������� �� ���������� � ���������� �������
  13. ������� ������ �������� ��������� � CAN ����� �������.
  14. ������� ������ ����� ��������� �� CAN ����
  15. ����������� ������� �� mRTOS.
*/
void main(void)
{
#asm("cli")
Init_port_combination();								        //���� g ��� ������ ������������ ����� � ����������
implementation = GetIndex(GetSignature());                 
if(implementation>sizeof(configurations))implementation=0;
vRunConfig(implementation);
vReconfigOnPortF();
Init_Sensors(MyConfig.whoAmI.SensorKeys);
Init_polar_ios(MyConfig.whoAmI.SensorKeys,prog_ios);
Init_Interfaces(MyConfig.whoAmI.MasterSlave);
vInitTim0();
class_cicle = class_recognition();
if(MyConfig.whoAmI.MasterSlave==MASTER)
{
    Init_CANDevices();
    vInitQueue();
}
#asm("sei")
Init_mRTOS();
switch(class_cicle)
{
    case CLASS_KEYS: 
        create_task(vTaskDORead,10,Active);  
        create_task(vTaskReceiveUart,10,Active);
        create_task(vTaskUART1Send,10,Active); 
        create_task(vTaskAlgorithmSep,10,Active);
    break;
    case CLASS_SEPCAN_1:
        create_task(vTaskDIRead,10,Active);
        create_task(vTaskReceiveUart,10,Active);
        create_task(vTaskAdddMsgCAN,10,Active);
        create_task(vTaskParcingAddMsgCAN,10,Active);        
        create_task(vTaskCANMessage,10,Active);
        create_task(vTaskCanReceive,10,Active);
        create_task(vTaskUART1Send,10,Active);  
        create_task(vTaskReadStatusCan,10,Active);
    break;
    case CLASS_SEPCAN_2:
        create_task(vTaskDIRead,10,Active);
        create_task(vTaskReceiveUart,10,Active);
        create_task(vTaskAdddMsgCAN,10,Active);
        create_task(vTaskCANMessage,10,Active);
        create_task(vTaskCanReceive,10,Active); 
        create_task(vTaskUART1Send,10,Active);      
        create_task(vTaskReadStatusCan,10,Active);  
    break;
    case CLASS_SEPNOTCAN:
        create_task(vTaskDIRead,10,Active);   
//        create_task(vTaskReceiveUart,10,Active);
//        create_task(vTaskUART0Send,10,Active);
        create_task(vTaskUART1Send,10,Active);          
    break;    
}
Sheduler();

while (1)
      {

      }
}
