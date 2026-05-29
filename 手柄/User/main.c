#include "stm32f10x.h"
#include "delay.h"
#include "dht11.h"
#include "my_usart.h"
#include "adc.h"
 int left_x, left_y, right_x, right_y;
 
 void send_left_x(void)
{
    if(left_x == 0)      USART1_SendString("A\r\n");
    else if(left_x == 1) USART1_SendString("B\r\n");
    else if(left_x == 2) USART1_SendString("C\r\n");
}

// 发送左摇杆 Y 轴
void send_left_y(void)
{
    if(left_y == 0)      USART1_SendString("D\r\n");
    else if(left_y == 1) USART1_SendString("E\r\n");
    else if(left_y == 2) USART1_SendString("F\r\n");
}

// 发送右摇杆 X 轴
void send_right_x(void)
{
    if(right_x == 0)      USART1_SendString("G\r\n");
    else if(right_x == 1) USART1_SendString("H\r\n");
    else if(right_x == 2) USART1_SendString("I\r\n");
}

// 发送右摇杆 Y 轴
void send_right_y(void)
{
    if(right_y == 0)      USART1_SendString("J\r\n");
    else if(right_y == 1) USART1_SendString("K\r\n");
    else if(right_y == 2) USART1_SendString("L\r\n");
}
int main(void)
{
    // 1. 系统初始化
    SystemInit();           // 系统时钟配置（72MHz）
    Delay_Init();           // 延时函数初始化
    
    // 2. 外设初始化
    USART1_Init(115200);    // 串口1初始化（连接蓝牙模块）
    My_ADC_Init();          // ADC + DMA 初始化（4通道摇杆）
    
    // 3. 主循环
    while(1)
    {
        // 读取摇杆数据，更新 left_x, left_y, right_x, right_y
        ADC_data();
        
        // 发送摇杆指令
        send_left_x();
        send_left_y();
        send_right_x();
        send_right_y();
        
        // 每 50ms 发送一次
        delay_ms(50);
    }
}