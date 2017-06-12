#include "usart2.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

//初始化IO 串口1 
//bound:波特率
void Usart2_Init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;				   //IO口配置结构体
	USART_InitTypeDef USART_InitStructure;				 //串口配置结构体
	NVIC_InitTypeDef NVIC_InitStructure;				   //中断配置结构体
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //使能USART2时钟
 	USART_DeInit(USART2);          //复位串口2  在开启外设前 最好都要复位一下外设

   //USART 初始化设置--UART2
	 //USART2_TX
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3
	 
		USART_InitStructure.USART_BaudRate   = bound;//一般设置为9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits   = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity     = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口
    USART_Cmd(USART2, ENABLE);                //使能串口 
}

void Usart2_Send(u8 *buf,u8 len)
{
    u8 t;
    for(t=0;t<len;t++)		//循环发送数据
	  {		   
		    USART_SendData(USART2,buf[t]);
				while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	  
	  }
}

void Usart2_PutStr(u8 *str)
{
    while (0 != *str)    
    {    
        USART_SendData(USART2, *str);
        str++;
    }  
}

void USART2_IRQHandler(void)
{	    
 											 
} 
