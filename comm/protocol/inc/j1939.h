/**
  ******************************************************************************
  * @file    j1939.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с перечислениями, классами и функциями протокола J1939
  ******************************************************************************
*/
#ifndef TRADE_H_
#define TRADE_H_

/*!
    \enum KindOfMsg
    \brief Набор типов сообщений передаваймых в can шину
*/
typedef enum{AN1FROM1TO4,					//!<первая плата: с 1 по 4 аналоговые
			AN1FROM5TO8,					//!<первая плата: с 5 по 8 аналоговые
			DI1,							//!<первая плата: дискретные сигналы
			AN2FROM1TO4,					//!<вторая плата: с 1 по 4 аналоговые
			AN2FROM5TO8,					//!<вторая плата: с 5 по 8 аналоговые
			DI2								//!<вторая плата: дискретные сигналы
}KindOfMsg;

/*!
    \class J1939_standart
    \brief Структура стандарта J1939
*/
typedef struct{								//
	uint8_t FromWhom			:4;			//адрес источника
	uint8_t ToWhom				:4;			//адрес получателя
	uint16_t ParamGroupNum		:16;
	uint8_t reserv				:2;	
	uint8_t priority			:3;	
	uint8_t zero				:3;
}J1939_standart;

/*!
    \class J1939_params
    \brief Параметры стандарта J1939
*/
typedef struct{
	uint16_t base_id;
	uint8_t addr_source;
	uint8_t addr_dest;
}J1939_params;

/*!
    Добавляет в очередь пакет (по стандарту J1939) с данными для передачи в CAN шину
    
    \param ch_num номер can контроллера
    \param params параметры стандарта
    \param priority сообщения
    \param kind_of_msg тип передаваемого сообщения
    \param uint8_t *data указатель на передаваемые данные
    \param size количество передаваемых байтов данных
*/
void vJ1939(uint8_t ch_num, J1939_params params, uint8_t priority, uint8_t kind_of_msg, uint8_t *data, uint8_t size);

#include "./comm/protocol/src/j1939.c"

#endif /* TRADE_H_ */