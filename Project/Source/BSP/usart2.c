#include "usart2.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"


//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;				   //IO�����ýṹ��
	USART_InitTypeDef USART_InitStructure;				   //�������ýṹ��
	NVIC_InitTypeDef NVIC_InitStructure;				   //�ж����ýṹ��
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOA,Gʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
 	//USART_DeInit(USART2);          //��λ����2  �ڿ�������ǰ ��ö�Ҫ��λһ������
	//USART1_TX   PA.2
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ����2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ

  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;                //���������� һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;        //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	   //�շ�ģʽ   ����������ý��� �����

	USART_Init(USART2, &USART_InitStructure); //��ʼ������

	//#if EN_USART2_RX		  //���ʹ���˽���  
  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�	 ���յ����ݽ����ж�
	//#endif

	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
}


 //����1�����ַ���
//void Uart1SendStr(u8 *s)
//{
//  while (*s)                  //����ַ���������־
//  {
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
//		USART_SendData(USART1,*s++);
//  }
//}


u8  RX_buffer[tbuf];


//����2�����ַ���
void Uart2SendStr(u8 *s)
{
  while (*s)                  //����ַ���������־
  {
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	  
		USART_SendData(USART2,*s++);
  }
}

u8 RX_num=0;   //���ռ�������
void USART2_IRQHandler(void)
{	    
 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
	{	 
	 			 
		RX_buffer[RX_num] =USART_ReceiveData(USART2); 	//��ȡ���յ�������
		RX_num++;               
		if(RX_num>tbuf) RX_num = 0;   
	}  											 
} 
