#ifndef __MY_USART_H
#define __MY_USART_H

#include "stm32f10x.h"
#include <stdio.h>

// USART1 引脚宏定义
#define USART1_TX_GPIO_PORT      GPIOA
#define USART1_TX_GPIO_PIN       GPIO_Pin_9
#define USART1_RX_GPIO_PORT      GPIOA
#define USART1_RX_GPIO_PIN       GPIO_Pin_10
#define USART1_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USART1_CLK               RCC_APB2Periph_USART1

// 如果需要使用其他串口，可以在这里添加
// #define USART2_TX_GPIO_PORT      GPIOA
// #define USART2_TX_GPIO_PIN       GPIO_Pin_2
// #define USART2_RX_GPIO_PORT      GPIOA
// #define USART2_RX_GPIO_PIN       GPIO_Pin_3
// #define USART2_GPIO_CLK          RCC_APB2Periph_GPIOA
// #define USART2_CLK               RCC_APB1Periph_USART2

// 函数声明
void USART1_Init(uint32_t baudrate);    // 初始化USART1
void USART1_SendByte(uint8_t data);     // 发送一个字节
void USART1_SendString(char *str);      // 发送字符串
void USART1_SendData(uint8_t *data, uint16_t len); // 发送指定长度数据
uint8_t USART1_ReceiveByte(void);       // 接收一个字节（阻塞）
uint8_t USART1_IsDataAvailable(void);   // 检查是否有数据可读

#endif