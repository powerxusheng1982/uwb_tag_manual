#include  "usart2.h"
#include  "string.h"
#include  "bsp.h"
#include  "esp8266.h"
#include  "oled.h"
#include  "timer.h"
#include "device_info.h"  

u8 esp_at[]="AT\r\n";                          //握手连接指令，返回"OK"
u8 esp_cwmode[]="AT+CWMODE=1\r\n";             //设置ESP8266的工作模式1 Station，返回"OK"或者"no change"
u8 esp_cwjap[]="AT+CWJAP=\"test\",\"12345678\"\r\n";  //连接到WiFi热点或无线路由上，MMA为无线路由名称，86081573为密码；连接成功返回“OK”     
u8 esp_cifsr[]="AT+CIFSR\r\n";                 //本机IP地址查询指令
u8 esp_cipsta[]="AT+CIPSTART=\"TCP\",\"192.168.43.238\",5000\r\n"; //连接到TCP服务器，返回“Linked” 
                                               //192.168.0.149为服务器IP地址 6000为服务器端口号 不同电脑不同软件可能会不一样的 
u8 esp_cipsend[]="AT+CIPSEND=16\r\n";   		     //设置发送数据长度
u8 esp_test[]="UWB Smart Link\r\n";   					         //数据内容


//指定字符串与缓存数组数据进行数据比较
//*p 要比较的指定字符串指针数据
//返回：1 数据一致  0 数据不一致 
u8 Data_compare(u8 *p)
{ 
	if(strstr(RX_buffer,p)!=NULL)
	    return 1;
	else
		return 0;
}


int ESP8266_Init(void)
{	
		memset(RX_buffer, 0, tbuf);//清缓存数据
		RX_num=0;				  				 //接收计数变量清0
		
		//发送AT 进行握手
		while(1)
		{
			 OLED_ShowString(0,0,"SEND AT    "); 
			 Uart2SendStr(esp_at);	   //串口2对wifi模块发送握手指令 即AT
			 if(Data_compare("OK"))break;
			 //else  Uart1SendStr("ERROR1,some problems with ESP8266 \r\n");
			 else{;}
			 delay_ms(600);
		}
		//Uart1SendStr("OK,mcu connection success with ESP8266! \r\n");
		memset(RX_buffer, 0, tbuf);//清缓存数据
		RX_num=0;				           //接收计数变量清0


		//配置wifi工作模式为sta模式
    while(1)
		{
			OLED_ShowString(0,0,"CFG STA    "); 
			Uart2SendStr(esp_cwmode);	   //串口2对wifi模块工作模式进行设置  
			if(Data_compare("OK")||Data_compare("no change"))break;
			//else  Uart1SendStr("ERROR2,some problems with ESP8266 \r\n");
			else{;}
			delay_ms(600);
		}
		//Uart1SendStr("OK,set mode as Station with ESP8266! \r\n");	 
		memset(RX_buffer, 0, tbuf);//清缓存数据	
		RX_num=0;				   //接收计数变量清0

		//连接热点wifi  wifi名 密码  如果失败 延时继续连接
    while(1)
		{
			OLED_ShowString(0,0,"Connect Wifi     ");
			
			sprintf((unsigned char*)&esp_cwjap[0],"AT+CWJAP=\"%s\",\"%s\"\r\n",ROUTER_ADMI_BUFF+1,ROUTER_PSWD_BUFF+1);
			Uart2SendStr(esp_cwjap);	   //串口2发送 指点wifi名 密码 等待模块连接 
			if(Data_compare("OK"))break;
			//else  Uart1SendStr("ERROR3,some problems with ESP8266 \r\n");
			else{;}
			delay_ms(600);
		}
		//Uart1SendStr("OK,Succeed establish connection with WiFi AP\r\n");	
		memset(RX_buffer, 0, tbuf);//清缓存数据
		RX_num=0;				   //接收计数变量清0



		//连接服务器server
		while(1)
		{
			OLED_ShowString(0,0,"Connect Server  ");
			Uart2SendStr(esp_cipsta);	   //wifi模块配置 服务器 IP 和端口
			if(Data_compare("OK"))break;
			//else  Uart1SendStr("ERROR4,some problems with ESP8266 \r\n");
			else{;}
			delay_ms(600);
		}
		//Uart1SendStr("OK,Succeed establish connection with TCP sever\r\n");	
		memset(RX_buffer, 0, tbuf);//清缓存数据
		RX_num=0;				   //接收计数变量清0

		//数据测试
		while(1)
		{
			OLED_ShowString(0,0,"MSG Test      ");
			Uart2SendStr(esp_cipsend);	   //配置测试数据字节长度
			delay_ms(600);
			Uart2SendStr(esp_test);	   //发送测试数据
			delay_ms(600);
			if(Data_compare("SEND OK"))break;
			//else  Uart1SendStr("ERROR5,some problems with ESP8266 \r\n");
			else{;}
		}
		//Uart1SendStr("Test ok, You can send commands through TCP sever now\r\n");	
		memset(RX_buffer, 0, tbuf);//清缓存数据
		RX_num=0;				   //接收计数变量清0
		
		OLED_ShowString(0,0,"Let's Start     ");
		return 1;
}

