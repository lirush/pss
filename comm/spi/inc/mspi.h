/**
  ******************************************************************************
  * @file    mspi.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� ������� � ������� SPI ����������
  ******************************************************************************
*/
#ifndef __SPI_H
#define __SPI_H

#define DD_SCK 1
#define DD_MOSI 2
#define SPI_BUFF_SIZE	50		/*!������� ������ ������*/

/*!
   \class selector
   ������������� ��������� SPI ����������
*/
typedef struct{
	volatile uint8_t *port;
	uint8_t b;
}selector;
selector Sel_impl[2];

extern uint8_t SPI_data[SPI_BUFF_SIZE];		/*!����� ����������� ������*/
extern volatile uint8_t SPI_busy;

/*!
   \enum DIV
   ����� ��������� ������� SPI
*/
enum DIV {OSC_DIV_2=1, OSC_DIV_4,OSC_DIV_8,OSC_DIV_16,OSC_DIV_32,OSC_DIV_64,OSC_DIV_128};

/*!
   ������������� SPI �������
*/
void Init_SPIMaster(enum DIV);

/*!
   ����� �� SPI
   
   \param *data_in_out ��������� �� ������
   \param size_data ���������� ���� ������
   \return 0
*/
uint8_t uSPIWrite(uint8_t *data_in_out, uint8_t size_data);
/*!
   ����� �� SPI
   ����� �������� ��� ���������� ����. ���� �� �������� �� ������
   
   \param[in] *data ��������� �� ������
   \param[in] size_data ���������� ���� ������
   \param[out] *data ��������� �� ������
   \return 0 
*/
uint8_t uSPIReadWrite(uint8_t *data, uint8_t size_data);

/*!
   ��������/��������� ���������� ��� ������ �� SPI ����������
   �������� ������� ������
   ����� �������� ��� ���������� ����. ���� �� �������� �� ������
   ������������ ������������� ��� ������ ������ � ���������� SPI
   
   \param ch_num ����� ������ ����������
   \param *data ��������� �� ������
   \param size ���������� ���� ������ 
*/
void exchange(uint8_t ch_num,uint8_t *data,uint8_t size);
/*!
   ��������/��������� ���������� ��� ������ �� SPI ����������
   �������� ������� ������
   ������������ ��� ������/������ ������ ���������� SPI
   
   \param ch_num ����� ������ ����������
   \param *data ��������� �� ������
   \param size ���������� ���� ������ 
*/
void exchange_read(uint8_t ch_num,uint8_t *data,uint8_t size);

/*!
    ���� ��������� SPI ���������� 
*/
void SPIBusy(void);

/*!
    ������ �� ������ ����� � SPI �������� ��� ��������/������
    ������������ ��� ������� ������������� ������ �� SPI. 
*/
void putspi(void);

#include "./comm/spi/src/mspi.c"

#endif