#include "APP.h"
#include "KALMAN.h"
#include "POSICAL.h"
#include "DEVICE_INFO.h"
#include "BSP.h"

#include "oled.h"

#include "led.h"
#include "usart1_dma.h"
#include "usart2.h"
#include "timer.h"
#include "stmflash.h"

#include "HW_Config.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

#define PARAMS_FLAG_ADDR  0x0800F800
#define PARAMS_DATA_ADDR  0x0800FC00
#define PARAMS_FLAG_VALID 0x1234


void Positon_Input_Process(void);
void Command_Input_Process(void);
void Reference_Position_ReadFromFlash(void);
void Reference_Position_WriteToFlash(void);

int main(void)
{
//		u8 datatemp[SIZE];
    uint32_t count = 0;
		uint8_t buf[200] = {0};
		
    BSP_Init();
		LED_Init();
		KEY_Init();
		Get_UniqueID();
		
    USB_Config();
		uart1_init(115200);
		Usart2_Init(115200);
//		TIM2_Int_Init(9999,7199);//20Khz的计数频率  1秒翻转一次
		
		OLED_Init();
		
		Reference_Position_Init();
		Reference_Position_ReadFromFlash();		
//		Flash_Configuration();
//		OLED_ShowNum(40,2,sys_para.start_count ,4,16);

		OLED_ShowString(0,0,"T-A0         mm");
		OLED_ShowString(0,1,"T-A1         mm");
		OLED_ShowString(0,2,"T-A2         mm");
		OLED_ShowString(0,3,"T-A3         mm");
		OLED_ShowString(0,4,"Tag X:       m");
		OLED_ShowString(0,5,"Tag Y:       m");
		OLED_ShowString(0,6,"Tag Z:       m");
			
   while (1)
   {
        if (count == 0) 
				{
						Positon_Input_Process();
				}

        if (count >= 0x2FFE) 
				{
					LED_OFF(eLED_0);
					
					Get_Best_Position();//计算坐标
					
				/*	sprintf(buf, "vec0 %f, %f, %f\r\n", m_anchorArray[0].x, m_anchorArray[0].y, m_anchorArray[0].z);
					USB_TxWrite(buf, strlen(buf));
					sprintf(buf, "vec1 %f, %f, %f\r\n", m_anchorArray[1].x, m_anchorArray[1].y, m_anchorArray[1].z);
					USB_TxWrite(buf, strlen(buf));
					sprintf(buf, "vec2 %f, %f, %f\r\n", m_anchorArray[2].x, m_anchorArray[2].y, m_anchorArray[2].z);
					USB_TxWrite(buf, strlen(buf));
					sprintf(buf, "vec3 %f, %f, %f\r\n", m_anchorArray[3].x, m_anchorArray[3].y, m_anchorArray[3].z);
					USB_TxWrite(buf, strlen(buf));*/
					
							
					OLED_ShowFloat(50,4,fabs(report.x), 2, 12);
					OLED_ShowFloat(50,5,fabs(report.y), 2, 12);
					OLED_ShowFloat(50,6,fabs(report.z), 2, 12);	
					
					sprintf(buf, "CAL POS %7.3f %7.3f %7.3f\r\n", report.x, report.y, report.z);
					Usart2_PutStr(buf);
				}
        if (++count > 0x2FFF) count = 0;
				
				Command_Input_Process();
    }
}

void Positon_Input_Process(void)
{
	uint32_t i = 0;	
	uint8_t buf[200] = {0};
	
	DataBuf_IsBusy = 1;
	
	if (DataBuf_IsValid == 0)
	{
		DataBuf_IsBusy = 0;
		return;
	}
		
	for(i=0;i<150;i++)
	{
		if((Data_Process_Buf[i]=='m') && (Data_Process_Buf[i+1]=='c'))
		{
			memcpy(buf, (void*)&Data_Process_Buf[i+6], 8);
			sscanf((char*)buf, "%x", &Range_deca[0]);
			OLED_ShowNum(50,0, Range_deca[0] ,5,16);

			memcpy(buf, (void*)&Data_Process_Buf[i+15], 8);
			sscanf((char*)buf, "%x", &Range_deca[1]);
			OLED_ShowNum(50,1, Range_deca[1] ,5,16);

			memcpy(buf, (void*)&Data_Process_Buf[i+24], 8);
			sscanf((char*)buf, "%x", &Range_deca[2]); 
			OLED_ShowNum(50,2, Range_deca[2] ,5,16);

			memcpy(buf, (void*)&Data_Process_Buf[i+33], 8);
			sscanf((char*)buf, "%x", &Range_deca[3]);
			OLED_ShowNum(50,3, Range_deca[3] ,5,16);							
		}
	}
	
	USB_TxWrite((uint8_t*)Data_Process_Buf, Usart1_Rec_Cnt);
	
	DataBuf_IsBusy = 0;
}

void Command_Input_Process(void)
{
	uint8_t  oled_buf[50];
	uint8_t  buf[200] = {0};
	uint32_t len = USB_RxRead(buf, sizeof(buf));

	int   index, x, y, z;
	vec3d anchor;
				
	if(len>0)
	{
		USB_TxWrite(buf, len);
		if(Check_cmd(buf) == DECA)
		{
			USB_TxWrite(buff_version, 19);
			memset( buf, 0, sizeof(buf) );
		}
		if(Check_cmd(buf) == SN)
		{
			Get_UniqueID();
			USB_TxWrite(buff_sn, 41);
			memset(buf, 0, sizeof(buf));
		}
					
		if(Check_cmd(buf) == ADMI)
		{
			//USB_TxWrite(buff_wifipassword, 10);
			//sscanf("AT+ADMI=12D_DWD-FF122\r\n","%*[^=]=%s",oled_buf);
			sscanf((char*)buf,"%*[^=]=%s",oled_buf);
			memset( buf, 0, sizeof(buf) );
			RouterInfo_Write_to_Flash(ADMI, oled_buf);
			memset( oled_buf, 0, sizeof(oled_buf) );
							
			OLED_ShowString(0,4,"                ");											
			OLED_ShowNum(85,4,ROUTER_ADMI_BUFF[0],4,16);			
			OLED_ShowString(0,4,ROUTER_ADMI_BUFF+1);
		}
					
		if(Check_cmd(buf) == PSWD)
		{
		  //USB_TxWrite(buff_wifipassword, 10);
			sscanf((char*)buf,"%*[^=]=%s",oled_buf);
			memset( buf, 0, sizeof(buf) );
			RouterInfo_Write_to_Flash(PSWD, oled_buf);
			memset( oled_buf, 0, sizeof(oled_buf) );
						
			OLED_ShowString(0,6,"                ");											
			OLED_ShowNum(85,6,ROUTER_PSWD_BUFF[0],4,16);			
			OLED_ShowString(0,6,ROUTER_PSWD_BUFF+1);					
		}
		
		if(Check_cmd(buf) == SVIP)
		{
			sscanf((char*)buf,"%*[^=]=%s",oled_buf);
			memset( buf, 0, sizeof(buf) );
			RouterInfo_Write_to_Flash(SVIP, oled_buf);
			memset( oled_buf, 0, sizeof(oled_buf) );
						
			OLED_ShowString(0,0,"                ");											
			OLED_ShowNum(85,0,ROUTER_SVIP_BUFF[0],4,16);			
			OLED_ShowString(0,0,ROUTER_SVIP_BUFF+1);	
		}
		
		if(Check_cmd(buf) == SVPT)
		{
			sscanf((char*)buf,"%*[^=]=%s",oled_buf);
			memset( buf, 0, sizeof(buf) );
			RouterInfo_Write_to_Flash(SVPT, oled_buf);
			memset( oled_buf, 0, sizeof(oled_buf) );
						
			OLED_ShowString(0,2,"                ");													
			OLED_ShowNum(85,2,ROUTER_SVPT_BUFF[0],4,16);			
			OLED_ShowString(0,2,ROUTER_SVPT_BUFF+1);	
		}

		/* POS A0 xxxxxx yyyyyy zzzzzz */
		if(Check_cmd(buf) == POS)
		{
			sscanf((char*)buf+5, "%x", &index);
			
			sscanf((char*)buf+7, "%d", &x);
			sscanf((char*)buf+14, "%d", &y);
			sscanf((char*)buf+21, "%d", &z);
			
			anchor.x = x/1000.0;
			anchor.y = y/1000.0;
			anchor.z = z/1000.0;
			
			Reference_Position_Set(index, anchor);		
			Reference_Position_WriteToFlash();
		}
	}
}

void Reference_Position_ReadFromFlash(void)
{
	vec3d anchor;
	u16 usFlag = 0, i = 0;
	u16 ausPos[12] = {0};
	
	STMFLASH_Read(PARAMS_FLAG_ADDR, &usFlag, 1);
	if (PARAMS_FLAG_VALID == usFlag)
	{
		STMFLASH_Read(PARAMS_DATA_ADDR, ausPos, 12);
		for (i=0; i<4; i++)
		{
			anchor.x = ausPos[3*i+0]/1000.0;
			anchor.y = ausPos[3*i+1]/1000.0;
			anchor.z = ausPos[3*i+2]/1000.0;
	
			Reference_Position_Set(i, anchor);
		}
	}
}

void Reference_Position_WriteToFlash(void)
{
	vec3d anchor;
	u16 usFlag = 0, i = 0;
	u16 ausPos[12] = {0};

	STMFLASH_Write(PARAMS_FLAG_ADDR, &usFlag, 1);
	
	for (i=0; i<4; i++)
	{
		anchor = Reference_Position_Get(i);
		ausPos[3*i+0]= (u16)(anchor.x * 1000);
		ausPos[3*i+1]= (u16)(anchor.y * 1000);
		ausPos[3*i+2]= (u16)(anchor.z * 1000);
	}
	
	STMFLASH_Write(PARAMS_DATA_ADDR, ausPos, 12);
	
	usFlag = PARAMS_FLAG_VALID;
	STMFLASH_Write(PARAMS_FLAG_ADDR, &usFlag, 1);
}

