#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

void beep_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;					 //定义GPIO结构体
 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);	//使能PC端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.1
	
	//GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
	//GPIO_SetBits(GPIOA, GPIO_Pin_2);			
}

void beep_SetFreq(uint16_t freq)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	uint16_t CCR_Val = 100;
	uint16_t PrescalerValue = 0;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);        //时钟使能
	
	//定时器TIM2初始化
  PrescalerValue = (uint16_t) (SystemCoreClock / (freq*200) ) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 200-1;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM2, ENABLE);
	//TIM_ITConfig(TIM2,TIM_IT_CC1,ENABLE);
	//TIM_CtrlPWMOutputs(TIM2,ENABLE); 
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2	
}

void beep_Stop(void)
{
	TIM_Cmd(TIM2, DISABLE);  //使能TIM2
	//GPIO_ResetBits(GPIOA,GPIO_Pin_2);	
}
