#ifndef	__ESP8266_H
#define __ESP8266_H
#include "stm32f10x.h"

extern u8 esp_cipsend[];   	//设置发送数据长度
extern u8 esp_test[];   		//数据内容

u8  Data_compare(u8 *p);
int ESP8266_Init(void);
#endif
