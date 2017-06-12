#include "led.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;					 //定义GPIO结构体
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能PC端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;			 //LED-->PA.1 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.1
 GPIO_SetBits(GPIOA,GPIO_Pin_1);						 //PA.1 输出高

}
 
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	 //结构体定义
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD,ENABLE);	//开启GPIOA GPIOD时钟
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;				 //IO设置为PD2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		   //设置成上拉输入
	GPIO_Init(GPIOD,&GPIO_InitStructure);	           //初始化GPIOD2

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA8设置成输入，默认上拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA12设置成输入，默认上拉	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.0
}
