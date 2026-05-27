#include "dma.h"

void My_DMA_Init(void)
{
    // 1. 使能 DMA1 时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    // 2. 如果需要 DMA 传输完成中断（可选）
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // ADC1 使用 DMA1 通道1
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

