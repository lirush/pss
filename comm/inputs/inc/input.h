/**
  ******************************************************************************
  * @file    input.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием функций работы с портами микрокроконтроллера
  ******************************************************************************
*/
#ifndef INIT_BOARD_H_
#define INIT_BOARD_H_

typedef struct{
	unsigned char *port;
	uint8_t b;
}set_of_inputs;

void port_setting(set_of_inputs io);
uint8_t port_state(set_of_inputs io);
void port_set_direct_out(set_of_inputs io);
void port_set_direct_in(set_of_inputs io);
void port_set_out(set_of_inputs io);
void port_unset_out(set_of_inputs io);
uint8_t port_state_for_outs(set_of_inputs io);

#include "./comm/inputs/src/input.c"

#endif /* INIT_BOARD_H_ */