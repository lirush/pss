/**
  ******************************************************************************
  * @file    mcp.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   ������������ ���� � ��������� �������, ��������, ������������ � ������� mcp2515
  ******************************************************************************
*/
#ifndef __MCP_H
#define __MCP_H

#define MAX_SIZE
#define SIZE_STANDART_ID	0x000007FFL
#define LEN_DATA_CAN		(8)
#define LEN_HEAD_CAN		(5)

#define TX_quantity			3
#define RX_quantity			2
#define CH_quantity			2

/*!
  \enum speed
  ����� ��������� ������ �� CAN ����
*/
typedef enum {S_50k,S_100k,S_125k,S_250k,S_500k}speed;
enum ch{CAN1,CAN2};
enum tx{TX0,TX1,TX2};
enum rx{RX0,RX1};
enum rx_mask{RXM0,RXM1};
enum rx_filter{RXF0,RXF1,RXF2,RXF3,RXF4,RXF5};
enum FrameDataMode{STANDART,EXTEND};

/*!
  \class Bitfilds
  \brief ������� ���� ��������� TXBnSIDH, TXBnSIDL, TXBnEID8, TXBnEID0
*/
typedef struct{
	uint8_t TXBSidh			:8;			//���� ������������ �������������� ��������� [10:3]	
	uint8_t TXBSidlEID		:2;			//���� ������������ �������������� ��������� [17:16]	
	uint8_t					:1;
	uint8_t TXBSidlEXIDE	:1;			//��� ���������� ����������� ���������
	uint8_t					:1;
	uint8_t TXBSidlSID		:3;			//���� ������������ �������������� ��������� [2:0]
	uint8_t TXBEid8			:8;			//���� ������������ �������������� ��������� [15:8]
	uint8_t TXBEid0			:8;			//���� ������������ �������������� ��������� [7:0]
	uint8_t TXBDlcDLC		:4;			//���������� ���� ������������ ������
	uint8_t					:2;
	uint8_t TXBDlcRTR		:1;			//��� ������� �������� � �������� ���������
	uint8_t					:1;
}Bitfilds;

/*!
  \class DataBuf
  �������� ��������� ������ ������, ����� ������, ������� ��� �������� ������ � �������
*/
typedef struct{
	uint8_t CommandToSend;
	Bitfilds whoIam;
	uint8_t data[8];
}DataBuf;

/*!
  \class TX_control
  ����� ��� ���������� �������� ������ � ����� can �����������
*/
typedef struct{
	DataBuf whatToSend;
	volatile uint8_t IntFlag;					//���� ������������� ���������� ���������� ������������ (����� ���������, ���� ��������)
}TX_control;

/*!
  \class RX_Bitfields
  \brief ������� ���� ��������� RXBnSIDH, RXBnSIDL, RXBnEID8, RXBnEID0
*/
typedef struct{
	uint8_t RXBSidh			:8;			//���� ������������ �������������� ��������� [10:3]
	uint8_t RXBSidlEID		:2;			//���� ������������ �������������� ��������� [17:16]
	uint8_t					:1;
	uint8_t RXBSidlIDE		:1;			//��� ����������� ���������. ����� ����� ��� ������: ����������� ��� �����������
	uint8_t RXBSidlSRR		:1;			//1 - ������ Standard Frame Remote Transmit Request; 0 - ������ ����������� ����� ������ (Standard Data Frame)
	uint8_t RXBSidlSID		:3;			//���� ������������ �������������� ��������� [2:0]
	uint8_t RXBEid8			:8;			//���� ������������ �������������� ��������� [15:8]
	uint8_t RXBEid0			:8;			//���� ������������ �������������� ��������� [7:0]
	uint8_t RXBDlcDLC		:4;			//���������� ���� �������� ������
	uint8_t					:2;
	uint8_t RXBDlcRTR		:1;			//��� ������� �������� � �������� ���������
	uint8_t					:1;	
}RX_Bitfields;

/*!
  \union IdReceive
  \brief ��������� ����� ���������� ������ �� CAN ����
*/
typedef union{
	uint8_t whoIsHe_build[5];
	RX_Bitfields whoIsHe;
}IdReceive;

/*!
  \class RX_data_control
  ����� ������ � ���������� ����� ���������� ������ �� CAN ����
*/
typedef struct{
	IdReceive whoIsHe;
	uint8_t buf[8];
}RX_data_control;

/*!
  \class RX_received
  ����� ������ � ���������� � ������� �� ������ ������ ������ can �����������
*/
typedef struct{
	uint8_t com_read;
	RX_data_control Received;
}RX_received;

/*!
  \class RX_control
  ����� ��� ������ ������ �� CAN ����
*/
typedef struct{	
	RX_received HeaderData;
	volatile uint8_t IntFlag;
	volatile uint8_t MsgReceive;						//��������� �������, ���������, �� �� ����������
}RX_control;

/*!
  \class BitsInt
  \brief ������� ���� ��� ���������������� ���������� � can ����������
*/
typedef struct{
	uint8_t RX0_en			:1;				//��� ���������� ���������� �� ���������� ������ ������ RX0
	uint8_t RX1_en			:1;				//��� ���������� ���������� �� ���������� ������ ������ RX1
	uint8_t TX0_en			:1;				//��� ���������� ���������� �� ���������� ������ �������� TX0
	uint8_t TX1_en			:1;				//��� ���������� ���������� �� ���������� ������ �������� TX1
	uint8_t TX2_en			:1;				//��� ���������� ���������� �� ���������� ������ �������� TX2
	uint8_t ERR_en			:1;				//���������� �� ��������� ��������� EFLG,
	uint8_t WAK_en			:1;				//���������� �� ������ ��� ������ ��� �������� ���������
	uint8_t MERR_en			:1;				//���������� �� ���������� ���� CAN
}BitsInt;

/*!
  \class int_control
  ���� ��� ���������������� ���������� � can ����������
*/
typedef union{
	uint8_t interrupt_build;
	BitsInt inter_rupt;
}int_control;

/*!
  \class To_send
  ���� ��� ������������� �������� ���������
*/
typedef struct{
	uint8_t ToSend_build;
	volatile uint8_t ToSend;	
}To_send;

/*!
  \class RX_Int_Clr
  ���� ��� ������������� ������ ���������� � can �����������
*/
typedef struct{
	uint8_t data[4];
	uint8_t FlagClr;
}RX_Int_Clr;

/*!
  \class MaskBitfields
  \brief ������� ���� ��������� ���������� ������� can �����������
*/
typedef struct{
	uint8_t RXMSidh			:8;			//���� ������������ �������������� ��������� [10:3]
	uint8_t RXMSidlEID		:2;			//���� ������������ �������������� ��������� [17:16]
	uint8_t					:1;
	uint8_t					:1;
	uint8_t					:1;
	uint8_t RXMSidlSID		:3;			//���� ������������ �������������� ��������� [2:0]
	uint8_t RXMEid8			:8;			//���� ������������ �������������� ��������� [15:8]
	uint8_t RXMEid0			:8;			//���� ������������ �������������� ��������� [7:0]
}MaskBitfields;

/*!
  \class Mask
  ����� ��������� ����� � can �����������
*/
typedef union{
	uint8_t MaskReg_build[4];
	MaskBitfields MaskReg;
}Mask;

/*!
  \class FilterBitfields
  \brief ������� ���� ��������� ���������� ��������� can �����������
*/
typedef struct{
	uint8_t RXFSidh			:8;			//���� ������������ �������������� ��������� [10:3]
	uint8_t RXFSidlEID		:2;			//���� ������������ �������������� ��������� [17:16]
	uint8_t					:1;
	uint8_t	RXFSidlEXIDE	:1;
	uint8_t					:1;
	uint8_t RXFSidlSID		:3;			//���� ������������ �������������� ��������� [2:0]
	uint8_t RXFEid8			:8;			//���� ������������ �������������� ��������� [15:8]
	uint8_t RXFEid0			:8;			//���� ������������ �������������� ��������� [7:0]
}FilterBitfields;

/*!
  \union Filter
  ���������������� �������� can �����������
*/
typedef union{
	uint8_t FiterReg_build[4];
	FilterBitfields FilterReg;
}Filter;

/*!
  \class CAN_ch
  ����� ��� ��������� can �����������, ������������� ��������, ������������� ������ ������� ������ �� can ���� 
*/
typedef struct {
	speed rate;								//�������� ���� can
	int_control inter_rupt;					//����������/������ ����������
	TX_control TX_example[TX_quantity];		//����������
	RX_control RX_example[RX_quantity];		//��������
	uint8_t Tx_num;							//������ ����� ����������� � ������
	To_send Request_To_Send;				//��������� ���������� ������������� �������� ���������
	RX_Int_Clr RX_Int_clear;				//������������� ������ ���������� � can �����������
	uint8_t Rx_num;							//������ ����� ��������� � ������
	uint8_t Rx_num_next;	
	Mask RX_SetMask[2];
	Filter RX_SetFilter[6];
}CAN_ch;
extern CAN_ch CanEx[CH_quantity];

/*!
  ������� ������������� can �����������
  ����������� �������� ������, ����� � ������� �������� �������
  \param ch_num ����� ����������� can
*/
void Init_CAN(int ch_num);

/*!
  �������������� �������� � ��������� ����� � ������� � can ����������
  
  \param ch_num ����� ����������� can
  \param id ������������� can ������
  \param ex ����������� ��� ����������� ����� ������
  \param *data ��������� �� ������ ��� ��������
  \param size ������ ������
*/
void vSendMsg(uint8_t ch_num, uint32_t id, uint8_t ex, uint8_t *data, uint8_t size);

#include "./comm/mcp/src/mcp.c"

#endif