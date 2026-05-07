#include "stm32f10x.h"                  // Device header

#include "stm32f10x.h"

// 延时函数（简单延时）
void delay_ms(uint32_t n)
{
    uint32_t i;
    for(; n > 0; n--)
        for(i = 0; i < 8000; i++);  // 粗略延时，实际需要根据时钟调整
}

int main(void)
{
    // 1. 使能GPIO时钟（根据你的LED连接的GPIO端口选择）
    // LED接在PC13（STM32F103核心板常见）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    // 或者如果LED接在PA8
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 2. 配置GPIO为推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;      // PC13
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // 3. 主循环：LED闪烁
    while(1)
    {
        // 输出低电平点亮LED（取决于LED接线，有些板子是低电平点亮）
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // 点亮LED
        delay_ms(500);  // 延时500ms
        
        // 输出高电平熄灭LED
       // GPIO_SetBits(GPIOC, GPIO_Pin_13);    // 熄灭LED
        delay_ms(500);  // 延时500ms
    }
}
