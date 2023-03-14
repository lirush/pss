/**
  ******************************************************************************
  * @file    mcp_implement.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� ������� ��������� can �����������
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
  ����������� �������� ������ �� can ����
  
  \param ch_num ����� can �����������
*/
void SetImplSpeed(int ch_num);

/*!
  ������������ ������������� ��������� �����������
  
  \param *whoIam ��������� �� ��������� ���������
  \param  id ������������� ���������, ���������� ���� uint32_t
*/
void SetTX_id(Bitfilds *whoIam, uint32_t id);

/*!
  ������������ ������������� ��������� ���������
  
  \param *whoIam ��������� �� ��������� ���������
  \param  id ������������� ���������, ���������� ���� uint32_t
*/
void SetRX_id(RX_Bitfields *whoIam, uint32_t id);
/*!
  ������������� ������������� ��������� ���������
  
  \param *header ��������� �� ��������� ���������
  \return ������������� ���������
*/
uint32_t unpack_id(uint8_t *header);

/*!
  ����������� ����� � ������� can �����������
  
  \param ch_num ����� can �����������
  \param on_off_mask ���/���� �����
  \param ms RXM0 (������� �����)
  \param fl0 (RXF0) (������� ������)
  \param f1l (RXF1) (������ ������)
  \param ms1 RXM1 (������ �����)
  \param fl2 (RXF2) (������ ������)
  \param fl3 (RXF3) (������ ������)
  \param fl4 (RXF4) (��������� ������)
*/
void SettingsRx(int ch_num, uint8_t on_off_mask,uint32_t ms, cFilter fl0, cFilter fl1, uint32_t ms1,cFilter fl2, cFilter fl3, cFilter fl4);

#include "./comm/mcp/src/mcp_implement.c"

#endif