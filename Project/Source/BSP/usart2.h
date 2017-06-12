#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f10x.h"

void Usart2_Init(u32 bound);
void Usart2_Send(u8 *buf,u8 len);
void Usart2_PutStr(u8 *str);
#endif


