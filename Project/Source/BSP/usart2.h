#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f10x.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
//#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
//#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������2����

#define tbuf 50
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

extern u8  RX_buffer[tbuf];
extern u8  RX_num;				 //���ռ�������

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);

//void Uart1SendStr(u8 *str);
void Uart2SendStr(u8 *str);
#endif

