#ifndef __USART1_DMA_H
#define __USART1_DMA_H
#include "stm32f10x.h"

#define DMA_Rec_Len 200

extern u16  Usart1_Rec_Cnt;           //��֡���ݳ���	

extern volatile u8 Data_Process_Buf[DMA_Rec_Len];	//�������ݻ�����
extern volatile u8 DataBuf_IsValid;								//���������е������Ƿ���Ч
extern volatile u8 DataBuf_IsBusy;								//���������е������Ƿ���æ

void uart1_init(u32 bound);

#endif


