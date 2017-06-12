#include "usart2.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

//��ʼ��IO ����1 
//bound:������
void Usart2_Init(u32 bound)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;				   //IO�����ýṹ��
	USART_InitTypeDef USART_InitStructure;				 //�������ýṹ��
	NVIC_InitTypeDef NVIC_InitStructure;				   //�ж����ýṹ��
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //ʹ��USART2ʱ��
 	USART_DeInit(USART2);          //��λ����2  �ڿ�������ǰ ��ö�Ҫ��λһ������

   //USART ��ʼ������--UART2
	 //USART2_TX
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3
	 
		USART_InitStructure.USART_BaudRate   = bound;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits   = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity     = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������
    USART_Cmd(USART2, ENABLE);                //ʹ�ܴ��� 
}

void Usart2_Send(u8 *buf,u8 len)
{
    u8 t;
    for(t=0;t<len;t++)		//ѭ����������
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
