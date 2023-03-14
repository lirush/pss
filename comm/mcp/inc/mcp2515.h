/**
  ******************************************************************************
  * @file    mcp2515.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием функций mcp2515
  ******************************************************************************
*/
#ifndef __MCP2515_H
#define __MCP2515_H

void mcp_reset(uint8_t ch_num);
void mcp_BitModify(uint8_t addr, uint8_t mask, uint8_t data,uint8_t ch_num);
void mcp_write(uint8_t data, uint8_t addr,uint8_t ch_num);
uint8_t mcp_reg_read(uint8_t addr,uint8_t ch_num);
void mcp_buf_transmit(uint8_t ch_num, uint8_t tx_num);
void mcp_buf_read(uint8_t ch_num, uint8_t rx_num);
uint8_t mcp_status_read(uint8_t ch_num);
uint8_t mcp_rx_status_read(uint8_t ch_num);
void mcp_rts(uint8_t ch_num, uint8_t buf_num);

void mcp_buf_load(uint8_t ch_num, uint8_t tx_num);

#include "./comm/mcp/src/mcp2515.c"

#endif