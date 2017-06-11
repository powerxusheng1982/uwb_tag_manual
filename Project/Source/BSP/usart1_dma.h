#ifndef __USART1_DMA_H
#define __USART1_DMA_H
#include "stm32f10x.h"
#define DMA_Rec_Len 200

extern u8  DMA_Rece_Buf[DMA_Rec_Len];	//DMA���մ������ݻ�����
extern u16 Usart1_Rec_Cnt;            //��֡���ݳ���	

void uart1_init(u32 bound);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);

#endif


