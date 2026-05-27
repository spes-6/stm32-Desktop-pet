#include "stm32f10x.h"                  // Device header

void TIM4_Init(void)
{
    // 1. 使能 TIM4 时钟（TIM4 在 APB1 总线上）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    // 2. 使用内部时钟（可选，默认就是内部）
    TIM_InternalClockConfig(TIM4);
    
    // 3. 配置时基
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;      // 自动重装值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;     // 预分频器
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;  // 高级定时器才用，TIM4 忽略
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
    
    // 4. 清除中断标志，使能更新中断
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    
    // 5. 配置 NVIC 中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;       // 改为 TIM4_IRQn
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    // 6. 使能 TIM4
    TIM_Cmd(TIM4, ENABLE);
}
//-----------------TIM3----------------------
//TIM3初始化  PA6是底部 PA7头部
void TIM3_PWM_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//  配置GPIO (PA6, PA7 复用推挽输出)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   // 复用推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3. 配置TIM3基本定时器参数
    // 时钟频率: 72MHz
    // 预分频器(PSC) = 71 -> 计数频率 = 72MHz / (71+1) = 1MHz (1us计数一次)
    // 自动重载值(ARR) = 19999 -> 周期 = (19999+1) * 1us = 20ms
    TIM_TimeBaseStructure.TIM_Period = 19999;           // 自动重载值(ARR)
    TIM_TimeBaseStructure.TIM_Prescaler = 71;           // 预分频器(PSC)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	 // 4. 配置PWM输出通道 (TIM3_CH1 - PA6, TIM3_CH2 - PA7)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   // PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  // 使能输出
    TIM_OCInitStructure.TIM_Pulse = 1500;               // 初始脉宽 (1.5ms, 对应90度)
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  // 高电平有效
		 // 应用到通道1 (PA6)
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);   // 使能预装载寄存器
    
    // 应用到通道2 (PA7)
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);   // 使能预装载寄存器
     // 5. 使能TIM3自动重载预装载
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    
    // 6. 启动TIM3的PWM输出
    TIM_Cmd(TIM3, ENABLE);
}
/*
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/
