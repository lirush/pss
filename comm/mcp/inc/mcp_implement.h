/**
  ******************************************************************************
  * @file    mcp_implement.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием функций настройки can контроллера
  ******************************************************************************
*/
#ifndef __MCP_IMPL
#define __MCP_IMPL
#include "mcp.h"
#include "conf.h"

#define FOSC_MCP		8000000l
#define TQ_8			8
#define TQ_16			16

/*!
  Настраивает скорость обмена по can шине
  
  \param ch_num номер can контроллера
*/
void SetImplSpeed(int ch_num);

/*!
  Конвертирует идентификатор сообщения передатчика
  
  \param *whoIam указатель на заголовок сообщения
  \param  id идентификатор сообщения, переменная типа uint32_t
*/
void SetTX_id(Bitfilds *whoIam, uint32_t id);

/*!
  Конвертирует идентификатор сообщения приемника
  
  \param *whoIam указатель на заголовок сообщения
  \param  id идентификатор сообщения, переменная типа uint32_t
*/
void SetRX_id(RX_Bitfields *whoIam, uint32_t id);
/*!
  Распаковывает идентификатор сообщения приемника
  
  \param *header указатель на заголовок сообщения
  \return идентификатор сообщения
*/
uint32_t unpack_id(uint8_t *header);

/*!
  Настраивает маски и фильтры can контроллера
  
  \param ch_num номер can контроллера
  \param on_off_mask вкл/выкл масок
  \param ms RXM0 (нулевая маска)
  \param fl0 (RXF0) (нулевой фильтр)
  \param f1l (RXF1) (первый фильтр)
  \param ms1 RXM1 (первая маска)
  \param fl2 (RXF2) (второй фильтр)
  \param fl3 (RXF3) (третий фильтр)
  \param fl4 (RXF4) (четвертый фильтр)
*/
void SettingsRx(int ch_num, uint8_t on_off_mask,uint32_t ms, cFilter fl0, cFilter fl1, uint32_t ms1,cFilter fl2, cFilter fl3, cFilter fl4);

#include "./comm/mcp/src/mcp_implement.c"

#endif