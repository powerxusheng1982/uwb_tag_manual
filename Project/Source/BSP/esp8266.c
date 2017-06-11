#include  "usart2.h"
#include  "string.h"
#include  "bsp.h"
#include  "esp8266.h"
#include  "oled.h"
#include  "timer.h"
#include "device_info.h"  

u8 esp_at[]="AT\r\n";                          //��������ָ�����"OK"
u8 esp_cwmode[]="AT+CWMODE=1\r\n";             //����ESP8266�Ĺ���ģʽ1 Station������"OK"����"no change"
u8 esp_cwjap[]="AT+CWJAP=\"test\",\"12345678\"\r\n";  //���ӵ�WiFi�ȵ������·���ϣ�MMAΪ����·�����ƣ�86081573Ϊ���룻���ӳɹ����ء�OK��     
u8 esp_cifsr[]="AT+CIFSR\r\n";                 //����IP��ַ��ѯָ��
u8 esp_cipsta[]="AT+CIPSTART=\"TCP\",\"192.168.43.238\",5000\r\n"; //���ӵ�TCP�����������ء�Linked�� 
                                               //192.168.0.149Ϊ������IP��ַ 6000Ϊ�������˿ں� ��ͬ���Բ�ͬ������ܻ᲻һ���� 
u8 esp_cipsend[]="AT+CIPSEND=16\r\n";   		     //���÷������ݳ���
u8 esp_test[]="UWB Smart Link\r\n";   					         //��������


//ָ���ַ����뻺���������ݽ������ݱȽ�
//*p Ҫ�Ƚϵ�ָ���ַ���ָ������
//���أ�1 ����һ��  0 ���ݲ�һ�� 
u8 Data_compare(u8 *p)
{ 
	if(strstr(RX_buffer,p)!=NULL)
	    return 1;
	else
		return 0;
}


int ESP8266_Init(void)
{	
		memset(RX_buffer, 0, tbuf);//�建������
		RX_num=0;				  				 //���ռ���������0
		
		//����AT ��������
		while(1)
		{
			 OLED_ShowString(0,0,"SEND AT    "); 
			 Uart2SendStr(esp_at);	   //����2��wifiģ�鷢������ָ�� ��AT
			 if(Data_compare("OK"))break;
			 //else  Uart1SendStr("ERROR1,some problems with ESP8266 \r\n");
			 else{;}
			 delay_ms(600);
		}
		//Uart1SendStr("OK,mcu connection success with ESP8266! \r\n");
		memset(RX_buffer, 0, tbuf);//�建������
		RX_num=0;				           //���ռ���������0


		//����wifi����ģʽΪstaģʽ
    while(1)
		{
			OLED_ShowString(0,0,"CFG STA    "); 
			Uart2SendStr(esp_cwmode);	   //����2��wifiģ�鹤��ģʽ��������  
			if(Data_compare("OK")||Data_compare("no change"))break;
			//else  Uart1SendStr("ERROR2,some problems with ESP8266 \r\n");
			else{;}
			delay_ms(600);
		}
		//Uart1SendStr("OK,set mode as Station with ESP8266! \r\n");	 
		memset(RX_buffer, 0, tbuf);//�建������	
		RX_num=0;				   //���ռ���������0

		//�����ȵ�wifi  wifi�� ����  ���ʧ�� ��ʱ��������
    while(1)
		{
			OLED_ShowString(0,0,"Connect Wifi     ");
			
			sprintf((unsigned char*)&esp_cwjap[0],"AT+CWJAP=\"%s\",\"%s\"\r\n",ROUTER_ADMI_BUFF+1,ROUTER_PSWD_BUFF+1);
			Uart2SendStr(esp_cwjap);	   //����2���� ָ��wifi�� ���� �ȴ�ģ������ 
			if(Data_compare("OK"))break;
			//else  Uart1SendStr("ERROR3,some problems with ESP8266 \r\n");
			else{;}
			delay_ms(600);
		}
		//Uart1SendStr("OK,Succeed establish connection with WiFi AP\r\n");	
		memset(RX_buffer, 0, tbuf);//�建������
		RX_num=0;				   //���ռ���������0



		//���ӷ�����server
		while(1)
		{
			OLED_ShowString(0,0,"Connect Server  ");
			Uart2SendStr(esp_cipsta);	   //wifiģ������ ������ IP �Ͷ˿�
			if(Data_compare("OK"))break;
			//else  Uart1SendStr("ERROR4,some problems with ESP8266 \r\n");
			else{;}
			delay_ms(600);
		}
		//Uart1SendStr("OK,Succeed establish connection with TCP sever\r\n");	
		memset(RX_buffer, 0, tbuf);//�建������
		RX_num=0;				   //���ռ���������0

		//���ݲ���
		while(1)
		{
			OLED_ShowString(0,0,"MSG Test      ");
			Uart2SendStr(esp_cipsend);	   //���ò��������ֽڳ���
			delay_ms(600);
			Uart2SendStr(esp_test);	   //���Ͳ�������
			delay_ms(600);
			if(Data_compare("SEND OK"))break;
			//else  Uart1SendStr("ERROR5,some problems with ESP8266 \r\n");
			else{;}
		}
		//Uart1SendStr("Test ok, You can send commands through TCP sever now\r\n");	
		memset(RX_buffer, 0, tbuf);//�建������
		RX_num=0;				   //���ռ���������0
		
		OLED_ShowString(0,0,"Let's Start     ");
		return 1;
}

