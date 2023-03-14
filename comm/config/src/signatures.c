/**
  ******************************************************************************
  * @file    signatures.c
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Исходный файл содержит функции определения сигнатуры блоков БРИ
  ******************************************************************************
*/
#include <mega128.h>
#include "signatures.h"

#define MASK 0x0F

uint16_t signatures[]={	
	CONFIG0,
	CONFIG1,
	CONFIG2,
	CONFIG3,
	CONFIG4,
	CONFIG5,
	CONFIG6,
	CONFIG7,
	CONFIG8,
	CONFIG9,
	CONFIG10,
	CONFIG11,
	CONFIG12,
	CONFIG13
};
static uint16_t ConfNum= (sizeof(signatures)/sizeof(int));

void Init_port_combination(void)
{
    uint16_t l=3000;
	DDRG |= (1<<PORTG0)|(1<<PORTG1)|(1<<PORTG2)|(1<<PORTG3);
	PORTG |= (1<<PORTG0)|(1<<PORTG1)|(1<<PORTG2)|(1<<PORTG3);
	while(l--);
	
	//для выбора конфигурации используются входы порта G 0-3
	DDRG &= ~0b00111111;
	PORTG |= (1<<PORTG0)|(1<<PORTG1)|(1<<PORTG2)|(1<<PORTG3);
}

/*!
    Переключает порты конфигурации вход -> выход
    \param num номер входа [0..7], который надо сделать выходом
*/
void activate_conf_pin(uint8_t num)				//num - номер входа [0..7], который надо сделать выходом
{
	uint16_t l=1000;
	Init_port_combination();
	if(num==0) return;
	DDRG |= (1<<num);							//вход -> выход
	PORTG &= ~(1<<num);				
	while(l--);
}

int signature = 0;
static uint8_t FlagCash=0;
int GetSignature(void)
{
    uint8_t temp;
    int i=0;
    long j=0;	
	if(FlagCash==0){
		signature = 0;
		for(i = 0;i<4;i++)
		{
			temp=0;
			activate_conf_pin(i);
			for(j =0;j<2000L;j++)
			temp |= PING & MASK;
			signature <<= 4;
			signature |= temp;
		}
		FlagCash = 1;
	}
	return signature;
}

uint8_t GetIndex(uint16_t num)
{
    uint8_t i=0;
	for(i=0;i<ConfNum;i++){
		if(num==signatures[i])
		return i;
	}
	return -1;
}