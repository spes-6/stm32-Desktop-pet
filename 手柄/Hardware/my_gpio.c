#include "my_gpio.h"

/**
  * @brief  高度封装的GPIO初始化函数（极致精简版）
  */
void My_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, MyGPIOMode mode, GPIOSpeed_TypeDef speed) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 核心优化：利用寄存器地址偏移，一行代码自动开启对应的APB2时钟
    // (GPIOx基地址 - GPIOA基地址) / 0x400 刚好等于对应的时钟位偏移量
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA << ((uint32_t)GPIOx - (uint32_t)GPIOA) / 0x400, ENABLE);

    // 将自定义枚举映射到标准库宏
    switch (mode) {
        case GPIO_MODE_OUT_PP: GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; break;
        case GPIO_MODE_OUT_OD: GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; break;
        case GPIO_MODE_IPU:    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    break;
        case GPIO_MODE_IPD:    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;    break;
        case GPIO_MODE_AIN:    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;    break;
        default:               GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; break;
    }

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Speed = speed;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/**
  * @brief  封装后的写引脚电平函数
  */
void My_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, MyGPIOState state) {
    // 直接操作 BSRR 寄存器：高16位写1清零(低电平)，低16位写1置位(高电平)
    GPIOx->BSRR = (state == GPIO_STATE_SET) ? GPIO_Pin : (uint32_t)GPIO_Pin << 16;
}

/**
  * @brief  封装后的读引脚电平函数
  */
uint8_t My_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
    return GPIO_ReadInputDataBit(GPIOx, GPIO_Pin);
}
