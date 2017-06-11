#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f10x.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
//#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
//#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口2接收

#define tbuf 50
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

extern u8  RX_buffer[tbuf];
extern u8  RX_num;				 //接收计数变量

//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);

//void Uart1SendStr(u8 *str);
void Uart2SendStr(u8 *str);
#endif


