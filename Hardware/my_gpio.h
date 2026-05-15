#ifndef __MY_GPIO_H
#define __MY_GPIO_H

#include "stm32f10x.h"

// 1. 封装引脚模式枚举（把标准库复杂的模式名简化）
typedef enum {
    GPIO_MODE_OUT_PP = 0,  // 推挽输出 (驱动LED、蜂鸣器)
    GPIO_MODE_OUT_OD,      // 开漏输出 (模拟I2C等)
    GPIO_MODE_IPU,         // 上拉输入 (按键)
    GPIO_MODE_IPD,         // 下拉输入
    GPIO_MODE_AIN          // 模拟输入 (ADC)
} MyGPIOMode;

// 2. 封装引脚状态枚举
typedef enum {
    GPIO_STATE_RESET = 0,  // 低电平 / 复位
    GPIO_STATE_SET         // 高电平 / 置位
} MyGPIOState;

// 3. 封装引脚初始化函数
// 参数：端口(GPIOA/B/C), 引脚(GPIO_Pin_x), 模式, 速度(仅输出模式有效)
void My_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, MyGPIOMode mode, GPIOSpeed_TypeDef speed);

// 4. 封装引脚电平控制函数
void My_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, MyGPIOState state);

// 5. 封装引脚电平读取函数 (返回 0 或 1)
uint8_t My_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void I2C_GPIO_Init(void);

#endif
