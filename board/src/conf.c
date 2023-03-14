/**
  ******************************************************************************
  * @file    conf.c
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Исходный файл содержит функций конфигураций блока БРИ
  ******************************************************************************
*/
#include <pgmspace.h>
#include "conf.h"
#include "signatures.h"

/*!
  Набор конфигураций плат входящих в состав блоков 
*/
PROGMEM const ComConfig configurations[14] ={				
	{.whoAmI.MasterSlave=SLAVE,											//--0-- CONFIG0	применима ко всем платам по умолчанию,
	.whoAmI.SensorKeys=BOARDSENSORS,
		.can_part.CAN_sender.BaseParamGroup=52040,
		.can_part.CAN_sender.SourceAddr=2,
		.can_part.CAN_sender.DestAddr=1,
		.polar_ios.polar_io_1_4=1,		//1
		.polar_ios.polar_io_5_8=0,		//0
		.polar_ios.polar_io_9_12=1,		//1
		.polar_ios.polar_io_13_16=0,	//0
		.polar_ios.polar_io_17_20=1,	//1
		.uart_part.addr = 0
	},      
	
	{.whoAmI.MasterSlave=SLAVE,										//--1-- 
	.whoAmI.SensorKeys=BOARDSENSORS},
    
	{.whoAmI.MasterSlave=SLAVE,										//--2-- 
	.whoAmI.SensorKeys=BOARDSENSORS},    
		
	{.whoAmI.MasterSlave=SLAVE,										//--3-- 
	.whoAmI.SensorKeys=BOARDSENSORS},
	
    {.whoAmI.MasterSlave=MASTER,                                        //--4-- БУ-2-СЭП(ПН1)
        .whoAmI.SensorKeys=BOARDSENSORS,
        .can_part.CAN_sender.BaseParamGroup=52070,
        .can_part.CAN_sender.SourceAddr=8,
        .can_part.CAN_sender.DestAddr=1,
        .can_part.mask_filter={
            {
                .MaskOn_00_Off_60=0x00,
                .Mask_={
                    0xFEFEF000,
                    0xFEFEF000
                },
                .filter={
                    {
                        .Extended=1,
                        .setFilter=0x0CF00000
                    },
                    {
                        .Extended=1,
                        .setFilter=0x0CF00000 
                    },
                    {
                        .Extended=1,
                        .setFilter=0x10CB4000
                    },
                    {
                        .Extended=1,
                        .setFilter=0x10CB4000
                    },
                    {
                        .Extended=1,
                        .setFilter=0x10CB4000
                    }
                }
                
            },
            {
                .MaskOn_00_Off_60=0x00,
                .Mask_={
                    0xFEFEF7FE,
                    0x7FFEF700
                },
                .filter={
                    {
                        .Extended=1,
                        .setFilter=0x0CCB7218				//
                    },
                    {
                        .Extended=1,
                        .setFilter=0x0CCB6B18
                    },
                    {
                        .Extended=1,
                        .setFilter=0x10CB5200
                    },
                    {
                        .Extended=1,
                        .setFilter=0x04CB5800           //0x04CBDA00 0x10CB5200
                    },
                    {
                        .Extended=1,
                        .setFilter=0x10CB5200
                    }
                }
                
            }
        },
		.polar_ios.polar_io_1_4=1,
		.polar_ios.polar_io_5_8=1,
		.polar_ios.polar_io_9_12=1,
		.polar_ios.polar_io_13_16=1,
		.polar_ios.polar_io_17_20=0,
		.uart_part.addr = 1,
		.SetConfigSlave = 9
	},
	
    {.whoAmI.MasterSlave=MASTER,                                        //--5-- БУ-2-СЭП(ПН2)
        .whoAmI.SensorKeys=BOARDSENSORS,
        .can_part.CAN_sender.BaseParamGroup=52073,
        .can_part.CAN_sender.SourceAddr=8,
        .can_part.CAN_sender.DestAddr=1,
        .can_part.mask_filter={
            {
                .MaskOn_00_Off_60=0x00,
                .Mask_={
                    0xFEFEF000,
                    0xFEFEF000
                },
                .filter={
                    {
                        .Extended=1,
                        .setFilter=0x0CF00000
                    },
                    {
                        .Extended=1,
                        .setFilter=0x0CF00000 
                    },
                    {
                        .Extended=1,
                        .setFilter=0x10CB4000
                    },
                    {
                        .Extended=1,
                        .setFilter=0x10CB4000
                    },
                    {
                        .Extended=1,
                        .setFilter=0x10CB4000
                    }
                }
                
            },
            {
                .MaskOn_00_Off_60=0x00,
                .Mask_={
                    0x0000F0F0,
                    0x0000F0F0
                },
                .filter={
                    {
                        .Extended=0,
                        .setFilter=0xF0F0				//не пропускать пакеты
                    },
                    {
                        .Extended=0,
                        .setFilter=0xF0F0
                    },
                    {
                        .Extended=0,
                        .setFilter=0xF0F0
                    },
                    {
                        .Extended=0,
                        .setFilter=0xF0F0
                    },
                    {
                        .Extended=0,
                        .setFilter=0xF0F0
                    }
                }
                
            }
        },
		.polar_ios.polar_io_1_4=0,
		.polar_ios.polar_io_5_8=0,
		.polar_ios.polar_io_9_12=1,
		.polar_ios.polar_io_13_16=1,
		.polar_ios.polar_io_17_20=1,
		.uart_part.addr = 2,
		.SetConfigSlave = 7
	},
	
	{.whoAmI.MasterSlave=SLAVE,										//--6-- 
	.whoAmI.SensorKeys=BOARDSENSORS},
	
	{.whoAmI.MasterSlave=SLAVE,										//--7-- БУ-2-СЭП(ПН3)
	.whoAmI.SensorKeys=BOARDSENSORS,
		.can_part.CAN_sender.BaseParamGroup=52080,
		.can_part.CAN_sender.SourceAddr=8,
		.can_part.CAN_sender.DestAddr=1,
		.polar_ios.polar_io_1_4=0,		
		.polar_ios.polar_io_5_8=0,		
		.polar_ios.polar_io_9_12=1,		
		.polar_ios.polar_io_13_16=0,	
		.polar_ios.polar_io_17_20=0,	
		.uart_part.addr = 3
	}, 
	
	{.whoAmI.MasterSlave=SLAVE,										//--8-- 
	.whoAmI.SensorKeys=BOARDSENSORS},
	
	{.whoAmI.MasterSlave=SLAVE,									    //--9-- для платы ключей c алгоритмом управления СЭПом
		.whoAmI.SensorKeys=BOARDCONTROLSEP,
		.uart_part.addr = 4},
	
	{.whoAmI.MasterSlave=SLAVE,										//--10-- 
	.whoAmI.SensorKeys=BOARDSENSORS},
	
	{.whoAmI.MasterSlave=SLAVE,										//--11-- 
	.whoAmI.SensorKeys=BOARDSENSORS},
	
	{.whoAmI.MasterSlave=SLAVE,									    //--12-- 
	.whoAmI.SensorKeys=BOARDSENSORS},
	
	{.whoAmI.MasterSlave=SLAVE,									    //--13-- 
	.whoAmI.SensorKeys=BOARDSENSORS},				
};

/*!
  Перечень портов программирования, с помощью которых устанавливается полярность входных дискретных каналов
*/
const set_of_inputs polar_prog[5]={								
	{&PINB,PORTB5},												//1-4
	{&PINB,PORTB6},												//5-8
	{&PINB,PORTB7},												//9-12
	{&PINE,PORTE2},												//13-16
	{&PINE,PORTE3}												//17-20
};

/*!
  Установка полярности входных дискретных каналов, в соостветствии с конфигурацией платы
*/
void set_polar_ios(Prog_polar_ios polar)						
{
	uint8_t *ptr=&polar.polar_io_1_4;
    int i=0;
	for (i=0;i<5;i++)
	{
		port_set_direct_out(polar_prog[i]);						
		if(*ptr++)												//в зависимости от сигнатуры устанавливается полярность входных блоков
			port_set_out(polar_prog[i]);
		else 
			port_unset_out(polar_prog[i]);
	}
}

uint8_t FindModeBoard(void){
	DDRG &= ~(1<<PORTG4);//!!!
	PORTG |= (1<<PORTG4);
	return (PING & (1<<PORTG4))?1:0;
}

/*!
  Определяет модель платы
  Установливает полярность входных каналов, в соостветствии с конфигурацией платы
*/
void Init_polar_ios(uint8_t type_board, Prog_polar_ios polar)
{
    if(type_board==BOARDSENSORS)
	if(FindModeBoard())												//если плата определена как новая модель платы
		set_polar_ios(polar);										//программируются полярности входных блоков
}