#include "u757.h"


//uint8_t *pack_msg_uart(uint8_t dst, uint8_t src, uint8_t cmd, uint8_t *data,uint8_t len)
//{                                            //������ � �������� uart
//    local.srcdst =src;                        //�������� � ����������, ������ � ���� ����
//    local.srcdst |=(dst<<4);
//    local.what_isit=cmd;
//    memcpy(local.data,data,len);
//    return (uint8_t *)&local;
//}


uart_ntw_level unpack_msg_uart(uint8_t *data)    
{                                                        //������������� ���������
    uart_ntw_level local;
    memcpy(&local,data,sizeof(local));
	return local;
}