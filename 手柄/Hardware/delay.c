#include "delay.h"

static uint32_t fac_us = 0;  // us延时倍乘数

// 初始化SysTick延时
// 必须在主函数开始时调用一次
void Delay_Init(void)
{
    // 更新系统时钟变量（SystemCoreClock在system_stm32f10x.c中定义）
    SystemCoreClockUpdate();
    
    // 设置SysTick时钟为HCLK/8 = 72MHz/8 = 9MHz
    // 每9个周期为1us，所以fac_us = 9
    fac_us = SystemCoreClock / 8000000;  // 72M/8M = 9
    
    // 配置SysTick，不使用中断
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

// 微秒级精确延时
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;
    
    // 计算需要计数的次数
    ticks = nus * fac_us;
    
    // 如果SysTick还没初始化，先初始化LOAD寄存器
    if(reload == 0)
    {
        SysTick->LOAD = 0xFFFFFF;  // 设置最大计数值
        SysTick->VAL = 0;
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // 使能SysTick
        reload = SysTick->LOAD;
    }
    
    // 获取当前计数值
    told = SysTick->VAL;
    while(1)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            // 计算经过的计数次数（注意递减计数）
            if(tnow < told)
                tcnt += told - tnow;
            else
                tcnt += reload - tnow + told;
            
            told = tnow;
            
            if(tcnt >= ticks)
                break;
        }
    }
}

// 毫秒级精确延时
void delay_ms(uint32_t nms)
{
    while(nms--)
    {
        delay_us(1000);
    }
}

// 秒级延时
void delay_s(uint32_t nS)
{
    while(nS--)
    {
        delay_ms(1000);
    }
}