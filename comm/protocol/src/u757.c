#include "u757.h"


//uint8_t *pack_msg_uart(uint8_t dst, uint8_t src, uint8_t cmd, uint8_t *data,uint8_t len)
//{                                            //пакуем в протокол uart
//    local.srcdst =src;                        //источник и получатель, пакуем в один байт
//    local.srcdst |=(dst<<4);
//    local.what_isit=cmd;
//    memcpy(local.data,data,len);
//    return (uint8_t *)&local;
//}


uart_ntw_level unpack_msg_uart(uint8_t *data)    
{                                                        //распаковываем сообщение
    uart_ntw_level local;
    memcpy(&local,data,sizeof(local));
	return local;
}