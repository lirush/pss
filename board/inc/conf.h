/**
  ******************************************************************************
  * @file    conf.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием классов, структур, перечислений и функций конфигураций блока БРИ
  ******************************************************************************
*/
#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

/*!
  \enum Role
  Набор ролей платы в составе блока 
*/
typedef enum{SLAVE,MASTER}Role;
/*!
  \enum KindOf
  Набор типов платы в составе блока 
*/					
typedef enum{BOARDSENSORS,BOARDKEYS,BOARDCONTROLSEP} KindOf;			
enum {NO,YES};

/*!
  \class WhoAmI 
  Структура определения роли и типа платы в составе блока 
*/
typedef	struct{
	Role MasterSlave;
	KindOf SensorKeys;
}WhoAmI;

/*!
  \class Uart_part 
  Структура обозначения устройства в UART обмене
*/
typedef	struct{
	uint8_t addr;					//адрес в сети uart
	//uint8_t SetConfig;				//не знаю пока!!!!!
}Uart_part;

/*!
  \class Sender 
  Структура обозначения устройства в CAN обмене
*/
typedef	struct{
	uint16_t BaseParamGroup;			//базовый адрес
	uint8_t SourceAddr;					//адрес источника в j1939
	uint8_t	DestAddr;					//получатель
}Sender;

/*!
  \class cFilter 
  \brief Настройка фильтров CAN устройств
*/
typedef struct{
	uint8_t Extended;				//расширенный или стандартный формат данных
	uint32_t setFilter;
}cFilter;

/*!
  \class MaskAndFilter 
  \brief Настройка масок и фильтров CAN устройств
*/
typedef struct{
	uint8_t MaskOn_00_Off_60;					//разрешить маску
	uint32_t Mask_[2];
	cFilter filter[6];
}MaskAndFilter;

/*!
  \class CAN_Part 
  Настройка масок и фильтров и обозачения устройства в CAN шине
*/
typedef	struct{
	Sender CAN_sender;
	MaskAndFilter mask_filter[2];
}CAN_Part;

/*!
  \class Prog_polar_ios 
  \brief Настройка полярности дискретных входных каналов платы
*/
typedef struct  
{
	uint8_t polar_io_1_4;			//полярность дискретных входов 1-4
	uint8_t polar_io_5_8;			//полярность дискретных входов 5-8
	uint8_t polar_io_9_12;			//полярность дискретных входов 9-12
	uint8_t polar_io_13_16;			//полярность дискретных входов 13-16
	uint8_t polar_io_17_20;			//полярность дискретных входов 17-20
}Prog_polar_ios;

/*!
  \class Prog_polar_ios 
  \brief Настройка конфигурации платы 
*/
typedef struct{
	WhoAmI whoAmI;
	CAN_Part can_part;
	Prog_polar_ios polar_ios;
	Uart_part uart_part;
	uint8_t SetConfigSlave;		//индекс сигнатуры для слейва
}ComConfig;
extern const ComConfig configurations[14];	

/*!
  Определяет модель платы
  Старая модель - порт PG подключен к земле
  Новая модель - порт PG в воздухе 
*/
uint8_t FindModeBoard(void);

/*!
  Программирует полярности дискретных входных каналов.
  Программирование дискретных каналов разбито на пять блоков
  В каждом блоке по четыре дискретных канала
  Для программирования отрицательной полярности в перечне конфигураций, 
  в соответствующей конфигурации для конкретного блока установить 0.
  Для программирования положительной полярности - установить 1.   
*/
void Init_polar_ios(uint8_t type_board, Prog_polar_ios polar);

#include ".\board\src\conf.c"

#endif /* CONFIGURATOR_H_ */