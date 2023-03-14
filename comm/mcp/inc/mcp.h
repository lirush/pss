/**
  ******************************************************************************
  * @file    mcp.h
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    30.06.2020
  * @brief   Заголовочный файл с описанием классов, структур, перечислений и функций mcp2515
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
  Набор скоростей обмена по CAN шине
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
  \brief Битовое поле регистров TXBnSIDH, TXBnSIDL, TXBnEID8, TXBnEID0
*/
typedef struct{
	uint8_t TXBSidh			:8;			//биты стандартного идентификатора сообщения [10:3]	
	uint8_t TXBSidlEID		:2;			//биты расширенного идентификатора сообщения [17:16]	
	uint8_t					:1;
	uint8_t TXBSidlEXIDE	:1;			//бит разрешения расширенной адресации
	uint8_t					:1;
	uint8_t TXBSidlSID		:3;			//биты стандартного идентификатора сообщения [2:0]
	uint8_t TXBEid8			:8;			//биты расширенного идентификатора сообщения [15:8]
	uint8_t TXBEid0			:8;			//биты расширенного идентификатора сообщения [7:0]
	uint8_t TXBDlcDLC		:4;			//количество байт передаваемых данных
	uint8_t					:2;
	uint8_t TXBDlcRTR		:1;			//бит запроса передачи с дальнего окончания
	uint8_t					:1;
}Bitfilds;

/*!
  \class DataBuf
  Содержит заголовок пакета данных, буфер данных, команду для отправки пакета с данными
*/
typedef struct{
	uint8_t CommandToSend;
	Bitfilds whoIam;
	uint8_t data[8];
}DataBuf;

/*!
  \class TX_control
  Класс для управления отправки данных в буфер can контроллера
*/
typedef struct{
	DataBuf whatToSend;
	volatile uint8_t IntFlag;					//флаг инициализации локального прерывания прередатчика (буфер упаковали, надо передать)
}TX_control;

/*!
  \class RX_Bitfields
  \brief Битовое поле регистров RXBnSIDH, RXBnSIDL, RXBnEID8, RXBnEID0
*/
typedef struct{
	uint8_t RXBSidh			:8;			//биты стандартного идентификатора сообщения [10:3]
	uint8_t RXBSidlEID		:2;			//биты расширенного идентификатора сообщения [17:16]
	uint8_t					:1;
	uint8_t RXBSidlIDE		:1;			//бит расширенной адресации. какой фрейм был принят: стандартный или расширенный
	uint8_t RXBSidlSRR		:1;			//1 - принят Standard Frame Remote Transmit Request; 0 - принят стандартный фрейм данных (Standard Data Frame)
	uint8_t RXBSidlSID		:3;			//биты стандартного идентификатора сообщения [2:0]
	uint8_t RXBEid8			:8;			//биты расширенного идентификатора сообщения [15:8]
	uint8_t RXBEid0			:8;			//биты расширенного идентификатора сообщения [7:0]
	uint8_t RXBDlcDLC		:4;			//количество байт принятых данных
	uint8_t					:2;
	uint8_t RXBDlcRTR		:1;			//бит запроса передачи с дальнего окончания
	uint8_t					:1;	
}RX_Bitfields;

/*!
  \union IdReceive
  \brief Заголовок вновь прибывшего пакета из CAN шины
*/
typedef union{
	uint8_t whoIsHe_build[5];
	RX_Bitfields whoIsHe;
}IdReceive;

/*!
  \class RX_data_control
  Пакет данных с заголовком вновь прибывшего пакета из CAN шины
*/
typedef struct{
	IdReceive whoIsHe;
	uint8_t buf[8];
}RX_data_control;

/*!
  \class RX_received
  Пакет данных с заголовком и команда на чтения буфера приема can контроллера
*/
typedef struct{
	uint8_t com_read;
	RX_data_control Received;
}RX_received;

/*!
  \class RX_control
  Класс для приема пакета из CAN шины
*/
typedef struct{	
	RX_received HeaderData;
	volatile uint8_t IntFlag;
	volatile uint8_t MsgReceive;						//сообщение считано, сохранено, но не обработано
}RX_control;

/*!
  \class BitsInt
  \brief Битовое поле для конфигурирования прерываний в can контроллер
*/
typedef struct{
	uint8_t RX0_en			:1;				//бит разрешения прерывания по заполнению буфера приема RX0
	uint8_t RX1_en			:1;				//бит разрешения прерывания по заполнению буфера приема RX1
	uint8_t TX0_en			:1;				//бит разрешения прерывания по заполнению буфера передачи TX0
	uint8_t TX1_en			:1;				//бит разрешения прерывания по заполнению буфера передачи TX1
	uint8_t TX2_en			:1;				//бит разрешения прерывания по заполнению буфера передачи TX2
	uint8_t ERR_en			:1;				//прерывание по изменению состояния EFLG,
	uint8_t WAK_en			:1;				//прерывание от ошибки при приеме или передаче сообщения
	uint8_t MERR_en			:1;				//прерывание от активности шины CAN
}BitsInt;

/*!
  \class int_control
  Клас для конфигурирования прерываний в can контроллер
*/
typedef union{
	uint8_t interrupt_build;
	BitsInt inter_rupt;
}int_control;

/*!
  \class To_send
  Клас для инициализации передачи сообщений
*/
typedef struct{
	uint8_t ToSend_build;
	volatile uint8_t ToSend;	
}To_send;

/*!
  \class RX_Int_Clr
  Клас для инициализации сброса прерывания в can контроллере
*/
typedef struct{
	uint8_t data[4];
	uint8_t FlagClr;
}RX_Int_Clr;

/*!
  \class MaskBitfields
  \brief Битовое поле регистров управления масками can контроллера
*/
typedef struct{
	uint8_t RXMSidh			:8;			//биты стандартного идентификатора сообщения [10:3]
	uint8_t RXMSidlEID		:2;			//биты расширенного идентификатора сообщения [17:16]
	uint8_t					:1;
	uint8_t					:1;
	uint8_t					:1;
	uint8_t RXMSidlSID		:3;			//биты стандартного идентификатора сообщения [2:0]
	uint8_t RXMEid8			:8;			//биты расширенного идентификатора сообщения [15:8]
	uint8_t RXMEid0			:8;			//биты расширенного идентификатора сообщения [7:0]
}MaskBitfields;

/*!
  \class Mask
  Класс настройки масок в can контроллере
*/
typedef union{
	uint8_t MaskReg_build[4];
	MaskBitfields MaskReg;
}Mask;

/*!
  \class FilterBitfields
  \brief Битовое поле регистров управления фильтрами can контроллера
*/
typedef struct{
	uint8_t RXFSidh			:8;			//биты стандартного идентификатора сообщения [10:3]
	uint8_t RXFSidlEID		:2;			//биты расширенного идентификатора сообщения [17:16]
	uint8_t					:1;
	uint8_t	RXFSidlEXIDE	:1;
	uint8_t					:1;
	uint8_t RXFSidlSID		:3;			//биты стандартного идентификатора сообщения [2:0]
	uint8_t RXFEid8			:8;			//биты расширенного идентификатора сообщения [15:8]
	uint8_t RXFEid0			:8;			//биты расширенного идентификатора сообщения [7:0]
}FilterBitfields;

/*!
  \union Filter
  Конфигурирование фильтров can контроллера
*/
typedef union{
	uint8_t FiterReg_build[4];
	FilterBitfields FilterReg;
}Filter;

/*!
  \class CAN_ch
  Класс для настройки can контроллера, инициализации передачи, инициализации приема пакетов данных из can шины 
*/
typedef struct {
	speed rate;								//скорость шины can
	int_control inter_rupt;					//разрешение/запрет прерываний
	TX_control TX_example[TX_quantity];		//передатчик
	RX_control RX_example[RX_quantity];		//приемник
	uint8_t Tx_num;							//хранит номер передатчика в канале
	To_send Request_To_Send;				//структура инструкции инициализации передачи сообщения
	RX_Int_Clr RX_Int_clear;				//инициализация сброса прерывания в can контроллере
	uint8_t Rx_num;							//хранит номер приемника в канале
	uint8_t Rx_num_next;	
	Mask RX_SetMask[2];
	Filter RX_SetFilter[6];
}CAN_ch;
extern CAN_ch CanEx[CH_quantity];

/*!
  Функция инициализации can контроллера
  Настраивает скорость обмена, маски и фильтры приемных буферов
  \param ch_num номер контроллера can
*/
void Init_CAN(int ch_num);

/*!
  Инициализирует передачу и выгружает пакет с данными в can контроллер
  
  \param ch_num номер контроллера can
  \param id идентификатор can пакета
  \param ex расширенный или стандартный пакет данных
  \param *data указатель на данные для передачи
  \param size размер данных
*/
void vSendMsg(uint8_t ch_num, uint32_t id, uint8_t ex, uint8_t *data, uint8_t size);

#include "./comm/mcp/src/mcp.c"

#endif