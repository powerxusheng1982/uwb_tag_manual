#ifndef __USART1_DMA_H
#define __USART1_DMA_H
#include "stm32f10x.h"

#define DMA_Rec_Len 200

extern u16  Usart1_Rec_Cnt;           //本帧数据长度	

extern volatile u8 Data_Process_Buf[DMA_Rec_Len];	//处理数据缓冲区
extern volatile u8 DataBuf_IsValid;								//表明缓冲中的数据是否有效
extern volatile u8 DataBuf_IsBusy;								//表明缓冲中的数据是否正忙

void uart1_init(u32 bound);

#endif


