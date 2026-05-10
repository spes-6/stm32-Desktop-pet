#include "stm32f10x.h"
#include "delay.h"
#include "dht11.h"
#include "my_usart.h"

int main(void)
{
    u8 temp = 0;
    u8 humi = 0;
    u8 count = 0;
    
    Delay_Init();
    USART1_Init(115200);
    
    printf("\r\n========================================\r\n");
    printf("     DHT11 Temperature & Humidity       \r\n");
    printf("========================================\r\n");
    
    // 初始化 DHT11
    printf("Initializing DHT11...\r\n");
    delay_ms(1000);  // 等待上电稳定
    
    if(DHT11_Init() == 0)
    {
        printf("[OK] DHT11 detected!\r\n");
    }
    else
    {
        printf("[ERROR] DHT11 not found!\r\n");
        printf("Check: VCC(3.3V/5V), DATA(PA5), GND, 4.7k pull-up\r\n");
        while(1);
    }
    
    // 等待传感器稳定并丢弃第一次不稳定数据
    printf("Waiting for sensor to stabilize...\r\n");
    delay_ms(2000);
    DHT11_Read_Data(&temp, &humi);  // 丢弃第一次
    delay_ms(100);
    
    printf("========================================\r\n");
    printf("Starting data reading...\r\n");
    printf("========================================\r\n\r\n");
    
    while(1)
    {
        if(DHT11_Read_Data(&temp, &humi) == 0)
        {
            count++;
            printf("[%03d] Temperature: %3d°C  |  Humidity: %3d%%RH\r\n", 
                   count, temp, humi);
        }
        else
        {
            // 读取失败时简单重试，不打印错误
            delay_ms(10);
            continue;
        }
        
        delay_ms(2000);  // 2秒读一次
    }
}