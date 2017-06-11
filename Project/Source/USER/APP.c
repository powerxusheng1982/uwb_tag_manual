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
int main(void)
{
		u8 datatemp[SIZE];
    uint32_t count = 0;
    uint32_t len = 0;
	  uint32_t i = 0;	
    int temp = 0;
	
	  int distance=0;
		uint8_t oled_buf[50];
    static uint8_t buf[200] = {0};
		static uint8_t buffa[65]="mc 0f 00001724 00001429 00001f40 00001e1d e9dd 4a 00707c03 a7:0\r\n";
		uint8_t* buffb[65];
		
	  float z_measure = 1.56;		
    BSP_Init();
		LED_Init();
		KEY_Init();
		Get_UniqueID();
		
    USB_Config();
		uart1_init(115200);
//		uart_init(115200);
		TIM2_Int_Init(9999,7199);//20Khz的计数频率  1秒翻转一次
		OLED_Init();
		
		Flash_Configuration();
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
//mc 01 00000d19 00000000 00000000 00000000 001e 4e 00006677 a6:0
//mr 01 00000d19 00000000 00000000 00000000 001e 4e 40224022 a6:0
//mc 01 00000d19 00000000 00000000 00000000 001e 4e 00006677 a6:0
//mr 01 00000d19 00000000 00000000 00000000 001e 4e 40224022 a6:0
//mc 01 00000d19 00000000 00000000 00000000 001e 4e 00006677 a6:0
//mr 01 00000d19 00000000 00000000 00000000 00			  
        if (count == 0) 
				{
					for(i=0;i<150;i++)
					{
						if((DMA_Rece_Buf[i]=='m') && (DMA_Rece_Buf[i+1]=='c'))
						{
							memcpy(buf,&DMA_Rece_Buf[i+6],8);
							sscanf((unsigned char* )buf,"%x",&Range_deca[0]); 
//						OLED_ShowString(40,4,buf);
							OLED_ShowNum(50,0, Range_deca[0] ,5,16);


							memcpy(buf,&DMA_Rece_Buf[i+15],8);
							sscanf((unsigned char* )buf,"%x",&Range_deca[1]); 
//						OLED_ShowString(40,4,buf);
							OLED_ShowNum(50,1, Range_deca[1] ,5,16);		

							memcpy(buf,&DMA_Rece_Buf[i+24],8);
							sscanf((unsigned char* )buf,"%x",&Range_deca[2]); 
//						OLED_ShowString(40,4,buf);
							OLED_ShowNum(50,2, Range_deca[2] ,5,16);		

							memcpy(buf,&DMA_Rece_Buf[i+33],8);
							sscanf((unsigned char* )buf,"%x",&Range_deca[3]); 
//						OLED_ShowString(40,4,buf);
							OLED_ShowNum(50,3, Range_deca[3] ,5,16);								
						}

					}

//					z_measure = 1.56 + frand()*0.1;
//					temp = (int)(Get_Kalman_Filter_Data(z_measure)*1000);
//					sprintf((unsigned char*)&buffb[0], "mc 0f %08x %08x %08x %08x e9dd 4a 00707c03 a7:0\r\n", temp, (int)(z_measure*1000),(int)(report.x*1000), (int)(report.y*1000));
					for(i=0;i<10;i++)  
					{
						//USB_TxWrite((unsigned char*)buffb, sizeof(buffb));
						
						//strcpy((unsigned char*)buffa,(unsigned char*)buffb);
						//USB_TxWrite((unsigned char*)buffa, sizeof(buffa));
						
						USB_TxWrite((unsigned char*)DMA_Rece_Buf, Usart1_Rec_Cnt);
					}					
					memset( buffb, 0, sizeof(buffb) );
				}

        if (count >= 0xFFFE) 
				{
					LED_OFF(eLED_0);
					//OLED_ShowNum(0,2,(int)(temp),4,16);
					Get_Best_Position();//计算坐标
					OLED_ShowFloat(50,4,fabs(report.x), 2, 12);
					OLED_ShowFloat(50,5,fabs(report.y), 2, 12);
					OLED_ShowFloat(50,6,fabs(report.z), 2, 12);
					
				}
        if (++count > 0xFFFF) count = 0;
				
        len = USB_RxRead(buf, sizeof(buf));
				if(len>0)
				{
					if(Check_cmd(buf) == DECA)
					{
						USB_TxWrite(buff_version, 19);
						memset( buf, 0, sizeof(buf) );
					}
					if(Check_cmd(buf) == SN)
					{
						Get_UniqueID();
						USB_TxWrite(buff_sn, 41);
						memset( buf, 0, sizeof(buf) );
					}
					
					if(Check_cmd(buf) == ADMI)
					{
						//USB_TxWrite(buff_wifipassword, 10);
						//sscanf("AT+ADMI=12D_DWD-FF122\r\n","%*[^=]=%s",oled_buf);
						sscanf((unsigned char*)buf,"%*[^=]=%s",oled_buf);
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
						sscanf((unsigned char*)buf,"%*[^=]=%s",oled_buf);
						memset( buf, 0, sizeof(buf) );
						RouterInfo_Write_to_Flash(PSWD, oled_buf);
						memset( oled_buf, 0, sizeof(oled_buf) );
						
						OLED_ShowString(0,6,"                ");											
						OLED_ShowNum(85,6,ROUTER_PSWD_BUFF[0],4,16);			
						OLED_ShowString(0,6,ROUTER_PSWD_BUFF+1);					
					}
					if(Check_cmd(buf) == SVIP)
					{
						sscanf((unsigned char*)buf,"%*[^=]=%s",oled_buf);
						memset( buf, 0, sizeof(buf) );
						RouterInfo_Write_to_Flash(SVIP, oled_buf);
						memset( oled_buf, 0, sizeof(oled_buf) );
						
						OLED_ShowString(0,0,"                ");											
						OLED_ShowNum(85,0,ROUTER_SVIP_BUFF[0],4,16);			
						OLED_ShowString(0,0,ROUTER_SVIP_BUFF+1);	
					}
					if(Check_cmd(buf) == SVPT)
					{
						sscanf((unsigned char*)buf,"%*[^=]=%s",oled_buf);
						memset( buf, 0, sizeof(buf) );
						RouterInfo_Write_to_Flash(SVPT, oled_buf);
						memset( oled_buf, 0, sizeof(oled_buf) );
						
						OLED_ShowString(0,2,"                ");													
						OLED_ShowNum(85,2,ROUTER_SVPT_BUFF[0],4,16);			
						OLED_ShowString(0,2,ROUTER_SVPT_BUFF+1);	
					}					
					
				}
//				if( (KEY0==0) || (KEYUP==0))//按键按下 配置WIFI
//				{
//					ESP8266_Init();
//				}
    }
}



