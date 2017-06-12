#include "led.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;					 //����GPIO�ṹ��
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��PC�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;			 //LED-->PA.1 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.1
 GPIO_SetBits(GPIOA,GPIO_Pin_1);						 //PA.1 �����

}
 
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	 //�ṹ�嶨��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD,ENABLE);	//����GPIOA GPIODʱ��
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;				 //IO����ΪPD2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		   //���ó���������
	GPIO_Init(GPIOD,&GPIO_InitStructure);	           //��ʼ��GPIOD2

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA8���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA12���ó����룬Ĭ������	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.0
}
