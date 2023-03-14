#include <string.h>
#include "mcp_implement.h"
#include "conf.h"
#include "def.h"
#include "mcp2515.h"
#include "mcp.h"
#include "spi.h"

//uint8_t CAN_TX_data[LENGHT_DATA_SEND];

//enum Mask_mode{ON_all,ON_std,ON_ext,OFF};
enum BIT_shift{BIT_3=3,BIT_8=8,BIT_13=13,BIT_16=16,BIT_18=18,BIT_21=21};
enum RX_control_mask{ALL_MSG,BY_MASK_MSG};
enum MF_num{SIDH,SIDL,EID8,EID0};
 
 void ConfBitSpeed(int ch_num, uint8_t brp, uint8_t PrSeg, uint8_t PhSeg1, uint8_t PhSeg2)
 {
	 mcp_BitModify(CANCTRL,MODE_MASK,MODE_CONF,ch_num);			//вход в режим конфигурирования CAN контроллера
	 mcp_BitModify(CNF1,CNF1_MASK_BRP,brp,ch_num);
	 //mcp_BitModify(CNF1,CNF1_MASK_BRP | CNF1_MASK_SJW,brp | CNF1_SET_SJW_4,ch_num);
	 mcp_BitModify(CNF2,CNF2_MASK_PHSEG1 | CNF2_MASK_PRSEG | CNF2_MASK_BTLMODE,(PhSeg1<<3) | PrSeg | 0x80,ch_num);
	 mcp_BitModify(CNF3,CNF3_MASK_PHSEG2,PhSeg2,ch_num);
	 mcp_BitModify(CANCTRL,MODE_MASK,MODE_NORM,ch_num);			//выход из режима конфигурирования
 }
 
 void WriteMask(int ch_num, uint8_t ex,uint32_t mask, int num)
 {
	 Mask *ptr = &CanEx[ch_num].RX_SetMask[num];
	 uint32_t lc_mask = mask;
	 if(ex){														//распихиваем в регистры масок и фильтров собственно маски и фильтры
		 ptr->MaskReg.RXMSidh = lc_mask>>BIT_21;
		 ptr->MaskReg.RXMSidlSID =lc_mask>>BIT_18;
		 ptr->MaskReg.RXMSidlEID = lc_mask>>BIT_16;
		 ptr->MaskReg.RXMEid8 = lc_mask>>BIT_8;
		 ptr->MaskReg.RXMEid0 = lc_mask;
	 }
	 else{
		 uint16_t loc_mask = (uint16_t)lc_mask;
		 ptr->MaskReg.RXMSidh = loc_mask>>BIT_3;
		 ptr->MaskReg.RXMSidlSID = loc_mask;
	 }
	 if(num==RXM0){
		 mcp_write(ptr->MaskReg_build[SIDH],RXM0SIDH,ch_num);
		 mcp_write(ptr->MaskReg_build[SIDL],RXM0SIDL,ch_num);
		 mcp_write(ptr->MaskReg_build[EID8],RXM0EID8,ch_num);	//укладываем маску и фильтр в контроллер CAN
		 mcp_write(ptr->MaskReg_build[EID0],RXM0EID0,ch_num);
	 }
	 if(num==RXM1){
		 mcp_write(ptr->MaskReg_build[SIDH],RXM1SIDH,ch_num);
		 mcp_write(ptr->MaskReg_build[SIDL],RXM1SIDL,ch_num);
		 mcp_write(ptr->MaskReg_build[EID8],RXM1EID8,ch_num);	//укладываем маску и фильтр в контроллер CAN
		 mcp_write(ptr->MaskReg_build[EID0],RXM1EID0,ch_num);
	 }
 }
 
 void WriteFilter(int ch_num,uint8_t ex,uint32_t filter, int num)
 {
	 Filter *ptr = &CanEx[ch_num].RX_SetFilter[num];
	 if(ex){																				//распихиваем в регистры масок и фильтров собственно маски и фильтры
		 ptr->FilterReg.RXFSidh = filter>>BIT_21;				//значение в фильтре должен совпадать с id приемника TX?
		 ptr->FilterReg.RXFSidlSID = filter>>BIT_18;
		 ptr->FilterReg.RXFSidlEID = filter>>BIT_16;
		 ptr->FilterReg.RXFEid8 = filter>>BIT_8;
		 ptr->FilterReg.RXFEid0 = filter;
		 ptr->FilterReg.RXFSidlEXIDE=ex;
	 }
	 else{
		 ptr->FilterReg.RXFSidh = filter>>BIT_3;
		 ptr->FilterReg.RXFSidlSID = filter;
	 }
	 if(num==RXF0){
		 mcp_write(ptr->FiterReg_build[SIDH],RXF0SIDH,ch_num);
		 mcp_write(ptr->FiterReg_build[SIDL],RXF0SIDL,ch_num);
		 if(ex){														//укладываем маску и фильтр в контроллер CAN
			 mcp_write(ptr->FiterReg_build[EID8],RXF0EID8,ch_num);
			 mcp_write(ptr->FiterReg_build[EID0],RXF0EID0,ch_num);
		 }
	 }
	 if(num==RXF1){
		 mcp_write(ptr->FiterReg_build[SIDH],RXF1SIDH,ch_num);
		 mcp_write(ptr->FiterReg_build[SIDL],RXF1SIDL,ch_num);
		 if(ex){														//укладываем маску и фильтр в контроллер CAN
			 mcp_write(ptr->FiterReg_build[EID8],RXF1EID8,ch_num);
			 mcp_write(ptr->FiterReg_build[EID0],RXF1EID0,ch_num);
		 }
	 }
	 if(num==RXF2){
		 mcp_write(ptr->FiterReg_build[SIDH],RXF2SIDH,ch_num);
		 mcp_write(ptr->FiterReg_build[SIDL],RXF2SIDL,ch_num);
		 if(ex){														//укладываем маску и фильтр в контроллер CAN
			 mcp_write(ptr->FiterReg_build[EID8],RXF2EID8,ch_num);
			 mcp_write(ptr->FiterReg_build[EID0],RXF2EID0,ch_num);
		 }
	 }
	 if(num==RXF3){
		 mcp_write(ptr->FiterReg_build[SIDH],RXF3SIDH,ch_num);
		 mcp_write(ptr->FiterReg_build[SIDL],RXF3SIDL,ch_num);
		 if(ex){														//укладываем маску и фильтр в контроллер CAN
			 mcp_write(ptr->FiterReg_build[EID8],RXF3EID8,ch_num);
			 mcp_write(ptr->FiterReg_build[EID0],RXF3EID0,ch_num);
		 }
	 }
	 if(num==RXF4){
		 mcp_write(ptr->FiterReg_build[SIDH],RXF4SIDH,ch_num);
		 mcp_write(ptr->FiterReg_build[SIDL],RXF4SIDL,ch_num);
		 if(ex){														//укладываем маску и фильтр в контроллер CAN
			 mcp_write(ptr->FiterReg_build[EID8],RXF4EID8,ch_num);
			 mcp_write(ptr->FiterReg_build[EID0],RXF4EID0,ch_num);
		 }
	 }
	 if(num==RXF5){
		 mcp_write(ptr->FiterReg_build[SIDH],RXF5SIDH,ch_num);
		 mcp_write(ptr->FiterReg_build[SIDL],RXF5SIDL,ch_num);
		 if(ex){														//укладываем маску и фильтр в контроллер CAN
			 mcp_write(ptr->FiterReg_build[EID8],RXF5EID8,ch_num);
			 mcp_write(ptr->FiterReg_build[EID0],RXF5EID0,ch_num);
		 }
	 }
 }

void SetImplSpeed(int ch_num){	
	uint32_t wishes_speed =0;
	uint8_t brp, ps1, ps2, ps;			//brp - прескалер скорости MCP2515, ps - сегмент распространения, ps1, ps2 - сегменты фаз 
	uint8_t PhSeg1, PhSeg2, PrSeg;		//биты управления длинной сегментов регистров CNF1, CNF2, CNF3
	if (ch_num>=CH_quantity)
	return;	    	
	switch(CanEx[ch_num].rate)
	{	
		case S_50k:
		wishes_speed=50000l;
		break;		
		case S_100k:
		wishes_speed=100000l;
		break;		
		case S_125k:
		wishes_speed=125000l;
		break;
		case S_250k:
		wishes_speed=250000l;	
		break;
		case S_500k:
		wishes_speed=500000l;
		break;
	}
	brp = (FOSC_MCP/(2*wishes_speed*TQ_8))-1;			//расчитываем прескалер исходя из выбранной скорости; NBT выбран 8TQ
	ps = 2;													//длинна сегмента распространения
	ps1 = 3;												//длинна сегмента фазы 1
	ps2 = 2;												//длинна сегмента фазы 2
	PrSeg = ps-1;									
	PhSeg1 = ps1-1;
	PhSeg2 = ps2-1;	
	ConfBitSpeed(ch_num,brp,PrSeg,PhSeg1,PhSeg2);			//управляем битами регистров CNF для установки выбранной скорости
}

 /*
 эта функция выполняет единтсвенную настройку - установка приоритета сообщения
 */
//void SettingsTx(int ch_num, int tx_num, uint8_t priority)
// {
//	 uint8_t addr=0;
//	 switch(tx_num)
//	 {
//		 case 0:
//		 addr = TXB0CTRL;										//
//		 break;
//		 case 1:
//		 addr = TXB1CTRL;
//		 break;
//		 case 2:
//		 addr = TXB2CTRL;
//		 break;
//	 }
//	 mcp_BitModify(addr,MASK_PRIORITY_MSG,priority,ch_num);	//установка приоритета сообщения
// }
 
void SetTX_id(Bitfilds *whoIam, uint32_t id)
{
	if(whoIam->TXBSidlEXIDE){												//укладываем id в стандартный или расширенный формат
		whoIam->TXBSidh=(uint8_t)((uint32_t)id>>BIT_21);										//в расширенном формате старшие 11 бит - стандартный идентификатор
		whoIam->TXBSidlEID=(uint8_t)((uint32_t)id>>BIT_16);
		whoIam->TXBSidlSID=(uint8_t)((uint32_t)id>>BIT_18);
		whoIam->TXBEid8=(uint8_t)((uint32_t)id>>BIT_8);
		whoIam->TXBEid0=(uint8_t)id;
	}
	else{
		whoIam->TXBSidh = (uint8_t)((uint16_t)id>>BIT_3);
		whoIam->TXBSidlSID = (uint8_t)id;
	}
} 

void SetRX_id(RX_Bitfields *whoIam, uint32_t id)
{
	if(whoIam->RXBSidlIDE){												//укладываем id в стандартный или расширенный формат
		whoIam->RXBSidh=(uint8_t)((uint32_t)id>>BIT_21);										//в расширенном формате старшие 11 бит - стандартный идентификатор
		whoIam->RXBSidlEID=(uint8_t)((uint32_t)id>>BIT_16);
		whoIam->RXBSidlSID=(uint8_t)((uint32_t)id>>BIT_18);
		whoIam->RXBEid8=(uint8_t)((uint32_t)id>>BIT_8);
		whoIam->RXBEid0=(uint8_t)id;
	}
	else{
		whoIam->RXBSidh = (uint8_t)((uint16_t)id>>BIT_3);
		whoIam->RXBSidlSID = (uint8_t)id;
	}
}

unsigned long unpack_id(uint8_t *header)
{
	IdReceive whoIAm;
	unsigned long id=0;
	memcpy(whoIAm.whoIsHe_build,header,sizeof(whoIAm.whoIsHe_build));
	if(whoIAm.whoIsHe.RXBSidlIDE) {
		id = ((uint32_t)((uint8_t)whoIAm.whoIsHe.RXBSidh)<<BIT_21);
		id |= ((uint32_t)((uint8_t)whoIAm.whoIsHe.RXBSidlEID)<<BIT_16);
		id |= ((uint32_t)((uint8_t)whoIAm.whoIsHe.RXBSidlSID)<<BIT_18);
		id |= (uint32_t)((uint8_t)whoIAm.whoIsHe.RXBEid8)<<BIT_8;
		id |= whoIAm.whoIsHe.RXBEid0;
	}else {
		id = (uint32_t)((uint8_t)whoIAm.whoIsHe.RXBSidh)<<BIT_3;
		id |= whoIAm.whoIsHe.RXBSidlSID;
	}
	return (uint32_t)id;
}

//void SettingsRx(int ch_num, uint8_t on_off_mask,uint8_t ex,uint32_t ms,uint32_t fl0,uint32_t fl1)
void SettingsRx(int ch_num, uint8_t on_off_mask,uint32_t ms, cFilter fl0, cFilter fl1, uint32_t ms1,cFilter fl2, cFilter fl3, cFilter fl4)
{
	switch(on_off_mask)
	{
		case MASK_OFF:
		mcp_write(MASK_OFF | MASK_BUKT,RXB0CTRL,ch_num);								//маски/фильтры отключены; будет принято любое сообщение, пролонгация			| MASK_BUKT
        mcp_write(MASK_OFF,RXB1CTRL,ch_num);
		break;
		case MASK_ON_00:
		mcp_write(MASK_ON_00,RXB0CTRL,ch_num);								//маски/фильтры включены приемник RX0     | MASK_BUKT
		mcp_write(MASK_ON_00,RXB1CTRL,ch_num);
		mcp_BitModify(CANCTRL,MODE_MASK,MODE_CONF,ch_num);				//вход в режим конфигурирования CAN контроллера
		WriteMask(ch_num,fl0.Extended,ms,RXM0);								//маска для приемника RX0
		WriteFilter(ch_num,fl0.Extended,fl0.setFilter,RXF0);								//фильтры для RX0
		WriteFilter(ch_num,fl1.Extended,fl1.setFilter,RXF1);
		//if(ex){
		WriteMask(ch_num,fl2.Extended,ms1,RXM1);
		WriteFilter(ch_num,fl2.Extended,fl2.setFilter,RXF2);
		WriteFilter(ch_num,fl3.Extended,fl3.setFilter,RXF3);
		WriteFilter(ch_num,fl4.Extended,fl4.setFilter,RXF4);
		WriteFilter(ch_num,0,0,RXF5);
		//}
		mcp_BitModify(CANCTRL,MODE_MASK,MODE_NORM,ch_num);			//выход из режим конфигурирования CAN контроллера
		break;
		case MASK_ON_01:
		mcp_write(MASK_ON_01,RXB0CTRL,ch_num);								//маски/фильтры включены приемник RX0     | MASK_BUKT
		mcp_write(MASK_ON_01,RXB1CTRL,ch_num);
		mcp_BitModify(CANCTRL,MODE_MASK,MODE_CONF,ch_num);				//вход в режим конфигурирования CAN контроллера
		WriteMask(ch_num,fl0.Extended,ms,RXM0);								//маска для приемника RX0
		WriteFilter(ch_num,fl0.Extended,fl0.setFilter,RXF0);								//фильтры для RX0
		WriteFilter(ch_num,fl1.Extended,fl1.setFilter,RXF1);
		//if(ex){
		WriteMask(ch_num,fl2.Extended,ms1,RXM1);
		WriteFilter(ch_num,fl2.Extended,fl2.setFilter,RXF2);
		WriteFilter(ch_num,fl3.Extended,fl3.setFilter,RXF3);
		WriteFilter(ch_num,fl4.Extended,fl4.setFilter,RXF4);
		WriteFilter(ch_num,0,0,RXF5);
		//}
		mcp_BitModify(CANCTRL,MODE_MASK,MODE_NORM,ch_num);			//выход из режим конфигурирования CAN контроллера
		break;
	}
}