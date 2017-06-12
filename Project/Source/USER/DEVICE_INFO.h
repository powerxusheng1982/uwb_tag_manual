#ifndef _DEVICE_INFO_H_
#define _DEVICE_INFO_H_
#include "stm32f10x.h"	

#define SN   0x01
#define SW   0x02
#define DECA 0x03
#define ADMI 0x04
#define PSWD 0x05
#define SVIP 0x06
#define SVPT 0x07

#define POS  0x08

typedef struct
{
	u16 flag;
	u16 start_count;
	u16 device_quantity;
	u16 server_ip[4];
	u16 remote_port;
} System_Para_TypeDef; 


extern u8 buff_version[19];
extern u8 buff_sn[41];
extern u8 buff_wifipassword[10];
extern System_Para_TypeDef sys_para;

extern u8 ROUTER_ADMI_BUFF[50];
extern u8 ROUTER_PSWD_BUFF[50];
extern u8 ROUTER_SVIP_BUFF[50];
extern u8 ROUTER_SVPT_BUFF[50];

u8 Check_cmd(u8* RXbuff);
u8 Get_UniqueID(void);
void Flash_Configuration(void);
u8 RouterInfo_Write_to_Flash(u8 OBJ, u8* buffer);
#endif

