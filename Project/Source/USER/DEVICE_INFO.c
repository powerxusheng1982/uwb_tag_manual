#include "device_info.h"  
#include "stm32f10x.h"	
#include "stdio.h"
#include "stmflash.h"	
#include "oled.h"	
u8 buff_version[19] = "nVer. 2.10 TREK\r\n";
u8 buff_sn[41];
u8 buff_wifipassword[10]="AT+PW=OK\r\n";


System_Para_TypeDef sys_para;
//const u8 TEXT_Buffer[]={"STM32 FLASH TEST"};
//#define SIZE sizeof(TEXT_Buffer)	 	//数组长度
#define PAGE61_ADDR  (0x08000000 + 41 * STM_SECTOR_SIZE) 	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
#define PAGE62_ADDR  (0x08000000 + 42 * STM_SECTOR_SIZE) 	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
#define PAGE63_ADDR  (0x08000000 + 43 * STM_SECTOR_SIZE) 	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
#define PAGE64_ADDR  (0x08000000 + 40 * STM_SECTOR_SIZE) 	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
#define PAGE65_ADDR  (0x08000000 + 45 * STM_SECTOR_SIZE) 	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
u8 Check_cmd(u8* RXbuff)
{
	if( (RXbuff[3]=='S') &&(RXbuff[4]=='N') )
			return SN;
	if( (RXbuff[3]=='S') &&(RXbuff[4]=='W') )
			return SW;
	if( (RXbuff[0]=='d') &&(RXbuff[4]=='$') )
			return DECA;

	if( (RXbuff[3]=='A') &&(RXbuff[4]=='D') &&(RXbuff[5]=='M') &&(RXbuff[6]=='I'))
			return ADMI;
	if( (RXbuff[3]=='P') &&(RXbuff[4]=='S') &&(RXbuff[5]=='W') &&(RXbuff[6]=='D'))
			return PSWD;
	if( (RXbuff[3]=='S') &&(RXbuff[4]=='V') &&(RXbuff[5]=='I') &&(RXbuff[6]=='P'))
			return SVIP;
	if( (RXbuff[3]=='S') &&(RXbuff[4]=='V') &&(RXbuff[5]=='P') &&(RXbuff[6]=='T'))
			return SVPT;
	return 0;
}

u8 Get_UniqueID(void)
{
	u32 CpuID[3];
	CpuID[0]=*(vu32*)(0x1ffff7e8);
	CpuID[1]=*(vu32*)(0x1ffff7ec);
	CpuID[2]=*(vu32*)(0x1ffff7f0);
	sprintf((unsigned char*)&buff_sn[0],"AT+SN=%08x-%08x-%08x-1-1.0\r\n",CpuID[0],CpuID[1],CpuID[2]);
	return 1;
}


//====================================================================//
// 语法格式：void Flash_Configuration(void)
// 实现功能：Flash记忆检测
// 参    数：无
// 返 回 值：无
// 备    注：无
//====================================================================//

u16 FLAH_BUFF0[16];
u8 ROUTER_ADMI_BUFF[50];
u8 ROUTER_PSWD_BUFF[50];
u8 ROUTER_SVIP_BUFF[50];
u8 ROUTER_SVPT_BUFF[50];
void Flash_Configuration(void)
{
	u16 temp[50];
	u8 count=0;
	STMFLASH_Read(PAGE61_ADDR, FLAH_BUFF0, 16);	

	if(FLAH_BUFF0[0] != 0xAAAA)//是否被标记过
	{
		FLAH_BUFF0[0] = 0XAAAA;
		FLAH_BUFF0[1]++;
		sys_para.start_count = FLAH_BUFF0[1];
		FLAH_BUFF0[2] = 0X0000;
		sys_para.device_quantity = 0x0000;
		
		FLAH_BUFF0[3] = 115;
		FLAH_BUFF0[4] = 159;
		FLAH_BUFF0[5] = 58;
		FLAH_BUFF0[6] = 141;
		
		FLAH_BUFF0[7] = 5000;
//		Flash_WriteByte(FLAH_WRITE_BUFF ,16, PAGE63_ADDR);//PAGE63，首次使用固定写死
		STMFLASH_Write(PAGE61_ADDR, FLAH_BUFF0, 16);//PAGE61，用作平时存储记录
	}
	else
	{
		FLAH_BUFF0[1]++;
		sys_para.start_count     =  FLAH_BUFF0[1];
		sys_para.device_quantity = 	FLAH_BUFF0[2];
		sys_para.server_ip[0]    = 	FLAH_BUFF0[3];
		sys_para.server_ip[1]    = 	FLAH_BUFF0[4];
		sys_para.server_ip[2]    = 	FLAH_BUFF0[5];
		sys_para.server_ip[3]    = 	FLAH_BUFF0[6];
		sys_para.remote_port     =  FLAH_BUFF0[7];
		STMFLASH_Write(PAGE61_ADDR, FLAH_BUFF0, 16);	//检测不是第一次配置，从PAGE61_ADDR读取数据并装配
		
		STMFLASH_Read(PAGE62_ADDR, temp, 20);
		for(count=0;count<(u8)(temp[0]+1);count++)
		{
			ROUTER_ADMI_BUFF[count] = (u8)temp[count];
			
		}
//		OLED_ShowString(0,4,"                ");											
//		OLED_ShowNum(85,4,ROUTER_ADMI_BUFF[0],4,16);			
//		OLED_ShowString(0,4,ROUTER_ADMI_BUFF+1);
		
		STMFLASH_Read(PAGE63_ADDR, temp, 20);
		for(count=0;count<(u8)(temp[0]+1);count++)
			ROUTER_PSWD_BUFF[count] = (u8)temp[count];

//		OLED_ShowString(0,6,"                ");											
//		OLED_ShowNum(85,6,ROUTER_PSWD_BUFF[0],4,16);			
//		OLED_ShowString(0,6,ROUTER_PSWD_BUFF+1);	

		
		STMFLASH_Read(PAGE64_ADDR, temp, 20);
		for(count=0;count<(u8)(temp[0]+1);count++)
			ROUTER_SVIP_BUFF[count] = (u8)temp[count];

//		OLED_ShowString(0,0,"                ");											
//		OLED_ShowNum(85,0,ROUTER_SVIP_BUFF[0],4,16);			
//		OLED_ShowString(0,0,ROUTER_SVIP_BUFF+1);	

		STMFLASH_Read(PAGE65_ADDR, temp, 20);
		for(count=0;count<(u8)(temp[0]+1);count++)
			ROUTER_SVPT_BUFF[count] = (u8)temp[count];

//		OLED_ShowString(0,2,"                ");											
//		OLED_ShowNum(85,2,ROUTER_SVPT_BUFF[0],4,16);			
//		OLED_ShowString(0,2,ROUTER_SVPT_BUFF+1);	
	}
}

u8 RouterInfo_Write_to_Flash(u8 OBJ, u8* buffer)
{
	u8 i=50;
  u16 temp[50];
	u8 count =0 ;
	for(i=0;i<50;i++)
	{
		if(buffer[i] == 0)
		{
			if(OBJ == ADMI)
			{
				ROUTER_ADMI_BUFF[0] = i;
				for(count =0 ;count<ROUTER_ADMI_BUFF[0]+1;count++)
					temp[count] = ROUTER_ADMI_BUFF[count];
				STMFLASH_Write(PAGE62_ADDR, temp, ROUTER_ADMI_BUFF[0]+1);
			}
			if(OBJ == PSWD)
			{
				ROUTER_PSWD_BUFF[0] = i;
				for(count =0 ;count<ROUTER_PSWD_BUFF[0]+1;count++)
					temp[count] = ROUTER_PSWD_BUFF[count];
				STMFLASH_Write(PAGE63_ADDR, temp, ROUTER_PSWD_BUFF[0]+1);
			}
			if(OBJ == SVIP)
			{
				ROUTER_SVIP_BUFF[0] = i;
				for(count =0 ;count<ROUTER_SVIP_BUFF[0]+1;count++)
					temp[count] = ROUTER_SVIP_BUFF[count];
				STMFLASH_Write(PAGE64_ADDR, temp, ROUTER_SVIP_BUFF[0]+1);
			}
			if(OBJ == SVPT)
			{
				ROUTER_SVPT_BUFF[0] = i;
				for(count =0 ;count<ROUTER_SVPT_BUFF[0]+1;count++)
					temp[count] = ROUTER_SVPT_BUFF[count];
				STMFLASH_Write(PAGE65_ADDR, temp, ROUTER_SVPT_BUFF[0]+1);
			}				
			return i;
		}
			
		if(OBJ == ADMI)
			ROUTER_ADMI_BUFF[i+1] = buffer[i];
		if(OBJ == PSWD)
			ROUTER_PSWD_BUFF[i+1] = buffer[i];
		if(OBJ == SVIP)
			ROUTER_SVIP_BUFF[i+1] = buffer[i];
		if(OBJ == SVPT)
			ROUTER_SVPT_BUFF[i+1] = buffer[i];
	}
	return i;
}


