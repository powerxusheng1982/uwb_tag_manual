#ifndef	__LED_H
#define __LED_H
#include "stm32f10x.h"
#define KEY0   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define KEYUP  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
void LED_Init(void);
void KEY_Init(void);
#endif
