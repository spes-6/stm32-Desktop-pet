#include "my_usart.h"

// 串口1初始化函数 (TX: PA9, RX: PA10)
void USART1_Init(uint32_t baudrate) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // 开启串口1和GPIOA的时钟
    RCC_APB2PeriphClockCmd(USART1_CLK | USART1_GPIO_CLK, ENABLE);

    // 配置 TX 引脚为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = USART1_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure);

    // 配置 RX 引脚为浮空输入
    GPIO_InitStructure.GPIO_Pin = USART1_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure);

    // 串口参数配置
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

// 发送一个字节（阻塞等待）
void USART1_SendByte(uint8_t data)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, data);
}

// 发送字符串
void USART1_SendString(char *str)
{
    while (*str)
    {
        USART1_SendByte((uint8_t)*str++);
    }
}

// 发送指定长度的数据
void USART1_SendData(uint8_t *data, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        USART1_SendByte(data[i]);
    }
}

// 接收一个字节（阻塞等待）
uint8_t USART1_ReceiveByte(void)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return (uint8_t)USART_ReceiveData(USART1);
}

// 检查是否有数据可读
uint8_t USART1_IsDataAvailable(void)
{
    return (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET);
}

// 重定向 printf 到串口
// 注意：需要在Keil中勾选 "Use MicroLIB"
int fputc(int ch, FILE *f)
{
    USART1_SendByte((uint8_t)ch);
    return ch;
}

// 重定向 getchar/scanf 到串口（可选）
int fgetc(FILE *f)
{
    return (int)USART1_ReceiveByte();
}