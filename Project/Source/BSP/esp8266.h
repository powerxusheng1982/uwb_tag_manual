#ifndef	__ESP8266_H
#define __ESP8266_H
#include "stm32f10x.h"

extern u8 esp_cipsend[];   	//���÷������ݳ���
extern u8 esp_test[];   		//��������

u8  Data_compare(u8 *p);
int ESP8266_Init(void);
#endif
