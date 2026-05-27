#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

// 改名：避免与标准库的 ADC_Init() 冲突
void My_ADC_Init(void);             
void My_ADC_Start(void);            
uint16_t My_ADC_GetValue(void);     
uint16_t My_ADC_GetLight(void);      
void ADC1_Tick(void);
uint16_t ADC_GetLight(void);
#endif /* __ADC_H */