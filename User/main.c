#include "stm32f10x.h"
#include "delay.h"
#include "dht11.h"
#include "my_usart.h"
#include "OLED_Data.h"
#include "OLED.h"
#include "my_gpio.h"
#include "Timer.h"
#include "Key.h"
#include "my_usart.h"
//----------------------------------全局变量定义-------------------------------------------------------------------------
int feel_left=0,feel_right=0,anger=0;//灭灯为1,anger为怒气值
int turn_left_A,turn_right_A;//暂停使用标志位
int mood=0,clear=0;//表示心情  清屏
extern uint8_t Key_Num;
int left_x=0,left_y=0,right_x=0,right_y=0;
 //uint16_t light_show=50;  // 光照值（0-100）
//----------------------------------end-------------------------------------------------------------------------


//-----------------------------------------------MODE 1----------------------------------------------------------------------------
void body(uint16_t base, uint16_t head)
{
	if(base>=2500){base=2500;}
	else if(base<=500){base=500;}
	if(head>=2500){head=2500;}
	else if(head<1200){head=1200;}
	 TIM_SetCompare1(TIM3, base);//底部   2500-500 
   TIM_SetCompare2(TIM3, head);//上部   2500-1200 2500-低头 1200-最顶 1500-超级仰头  1900-正常对人  2250--平视
}

void data_show(void)
{
    uint8_t temperature = 0;
    uint8_t humidity = 0;
    char display_str[20];  // 临时字符串缓冲区
    
    // 1. 读取DHT11数据
    if(DHT11_Read_Data(&temperature, &humidity) == 0)  // 返回0表示成功
    {
        // 2. 显示温度（使用英文字符串代替中文）
        OLED_ShowString(0, 0, "Temp:", OLED_8X16);
        OLED_ShowNum(60, 0, temperature - 4, 2, OLED_8X16);
        OLED_ShowString(85, 0, "C", OLED_8X16);
        
        // 3. 显示湿度
        OLED_ShowString(0, 22, "Humi:", OLED_8X16);
        OLED_ShowNum(60, 22, humidity, 2, OLED_8X16);
        OLED_ShowString(85, 22, "%", OLED_8X16);
        
//        // 4. 显示光照
//        OLED_ShowString(0, 45, "Light:", OLED_8X16);
//        OLED_ShowNum(60, 45, 100 - light_show, 2, OLED_8X16);
//        OLED_ShowString(85, 45, "%", OLED_8X16);
    }
    else
    {
        // 读取失败时显示错误
        OLED_ShowString(0, 45, "DHT11 Error!", OLED_8X16);
    }
    OLED_Update();  // 最后更新显示
}

void face(){       //表情控制       1模式
	  if(mood==0){   //超级喜爱
			if(clear!=0){
		OLED_Clear();	
		}
		clear=0;
		OLED_ShowImage(0,0,128,64,BMP2);
		}
    else if(mood==1){   //喜爱
			if(clear!=1){
		OLED_Clear();	
		}
		OLED_ShowImage(0,0,128,64,BMP5);
		clear=1;
		}
		else if(mood==2){   //一点喜爱
			if(clear!=2){
		OLED_Clear();	
		}
		OLED_ShowImage(0,0,128,64,BMP4);
		clear=2;
		}
	  if(mood==3){   //初始待机表情
		  if(clear!=3){
		OLED_Clear();	
		}
		OLED_ShowImage(0,0,128,64,BMP0);
		clear=3;
		}
		else if(mood==4){   //一级烦躁
			if(clear!=4){
		OLED_Clear();	
		}
		clear=4;
		OLED_ShowImage(0,0,128,64,BMP8);
		}
		else if(mood==5){   //二级烦躁
			if(clear!=5){
		OLED_Clear();	
		}
		clear=5;
		OLED_ShowImage(0,0,128,64,BMP9);
		}
		else if(mood==6){   //三级烦躁
			if(clear!=6){
		OLED_Clear();	
		}
		clear=6;
		OLED_ShowImage(0,0,128,64,BMP10);
		}
		else if(mood==7){   //四级烦躁
			if(clear!=7){
		OLED_Clear();	
		}
		clear=7;
		OLED_ShowImage(0,0,128,64,BMP11);
		}
		else if(mood==8){   //恼羞成怒
			if(clear!=8){
		OLED_Clear();	
		}
		clear=8;
		OLED_ShowImage(0,0,128,64,BMP7);
		}
		else if(mood==9){   //睡觉
			if(clear!=9){
		OLED_Clear();	
		}
		clear=9;
		OLED_ShowImage(0,0,128,64,BMP12);
		}
		else if(mood==10){   //OVER
			if(clear!=10){
		OLED_Clear();	
		}
		clear=10;
		OLED_ShowImage(0,0,128,64,BMP3);
		}
		else if(mood==11){   //晕
			if(clear!=11){
		OLED_Clear();	
		}
		clear=11;
		OLED_ShowImage(0,0,128,64,BMP1);}
//		if(dizzy_time>=30){
//		  mood=2;
//			dizzy_time=0;
//		}
//		}
		else if(mood==12){   //环境检测
		  if(clear!=12){
			OLED_Clear();	
			}
			clear=12;
			data_show();
		}
		//OLED_Refresh();
	OLED_Update();

}

// 修改后的 data_show() 函数



void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_1);  // 初始熄灭
}
int main(void)
{
	LED_Init();  
	Delay_Init();
	I2C_GPIO_Init();
  OLED_Init();
	DHT11_Init();
	OLED_Clear();
	Timer_Init();
	TIM3_PWM_Init();

	
    
    while(1)
    {
       body(1000,0);
			delay_ms(500);
			body(1500,0);
			delay_ms(500);
    }
	  
    
   
}


void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		static uint16_t cnt = 0;
        cnt++;
        if(cnt >= 500)  // 每 500ms 翻转一次
        {
					
            cnt = 0;
            // 假设 PA1 接了 LED
            GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1)));
        }
		Key_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

