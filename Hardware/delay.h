#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void Delay_Init(void);      // 初始化延时函数（必须在main中先调用）
void delay_us(uint32_t nUs);
void delay_ms(uint32_t nMs);
void delay_s(uint32_t nS);

#endif