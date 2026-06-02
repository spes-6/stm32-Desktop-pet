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
#include "adc.h"
//----------------------------------全局变量定义-------------------------------------------------------------------------
#define time 1000
int inter=0,feel_inter=0,inter_time=0;//互动标志 互动是否抬头  互动时间
int feel_left=0,feel_right=0,anger=0;//灭灯为1,anger为怒气值
int turn_left_A,turn_right_A;//暂停使用标志位
int mood=0,clear=0;//表示心情  清屏
extern uint8_t Key_Num;
int left_x=0,left_y=0,right_x=0,right_y=0;//遥感方位
int left_x1=0,left_y1=0,right_x1=0,right_y1=0,left_x2=0,left_y2=0,right_x2=0,right_y2=0;//摇杆异常处理
int mode=1,control_base=1500,control_head=1900,control_time=0;//模式  1-互动 2-游戏 3-手柄控制  4-自由模式
static uint16_t stuck_time = 0;//遥感使用时长
static uint8_t last_left_x = 0, last_left_y = 0;
static uint8_t last_right_x = 0, last_right_y = 0;
uint8_t Rx_dat = 0;//串口接收字符
uint8_t rx = 0;//串口接收字符
int receive_num=0,receive_sign=0,receive_double=0,receive_time=0,useful_num=0;//接收的数据   数据间隔
int state=0,chance=1;//动作,标志位

 uint16_t light_show=50;  // 光照值（0-100）
 
//----------------------------------end-------------------------------------------------------------------------
void mood_good(){   //情绪改变，心情变好，一点互动即可
	inter_time=0;
	if(mood<=8&&mood!=0){
	  mood--;
	}
}
void mood_bad(){    //情绪差，长时间不互动，放定时器
	if(mood<=7){
	  mood++;
	}
}


//---------------------------------------------------------------------移动停止
void stop(void)
{
    // 停止 PWM
    TIM_SetCompare3(TIM2, 0);
    TIM_SetCompare4(TIM2, 0);
    
    // 根据方向状态处理引脚
    if(turn_left_A == 1){
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    }
    // turn_left_A == 0 时，引脚已经是低电平，不需要额外处理
    
    if(turn_right_A == 1){
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    }
}

//-------------------------------------------------------------------------舵机控制
void body(uint16_t base, uint16_t head)
{
	if(base>=2500){base=2500;}
	else if(base<=500){base=500;}
	if(head>=2500){head=2500;}
	else if(head<1200){head=1200;}
	 TIM_SetCompare1(TIM3, base);//底部   2500-500 
   TIM_SetCompare2(TIM3, head);//上部   2500-1200 2500-低头 1200-最顶 1500-超级仰头  1900-正常对人  2250--平视
}
//-------------------------------------------------------------------遥感参数初始
void zero_control() {
    // 原有计数器清零
    left_x1 = 0; left_y1 = 0; right_x1 = 0; right_y1 = 0;
    left_x2 = 0; left_y2 = 0; right_x2 = 0; right_y2 = 0;
    
    // 异常检测计时器清零
    stuck_time = 0;
    
    // 状态记录清零（可选）
    last_left_x = 0; last_left_y = 0;
    last_right_x = 0; last_right_y = 0;
}

//---------------------------------------------------------------------遥感异常


void abnormal_deal() {
    // 检查是否有摇杆不在中位，且和上一帧相同
    if((left_x != 0 && left_x == last_left_x) ||
       (left_y != 0 && left_y == last_left_y) ||
       (right_x != 0 && right_x == last_right_x) ||
       (right_y != 0 && right_y == last_right_y)) {
        stuck_time++;
    } 
    else {
        stuck_time = 0;  // 任何变化都清零
    }
    
    // 记录当前状态
    last_left_x = left_x;
    last_left_y = left_y;
    last_right_x = right_x;
    last_right_y = right_y;
    
    // 60秒无变化
    if(stuck_time >= 600) {
        mode = 1;
			stuck_time = 0;
    }
}

//----------------------------------------------------------------------------姿态控制
void state_show(void)
{
    if(state == 0 && chance != 0)           // 左右看
    {
        mood_good();
        stop();
        body(500, 2500);
        delay_ms(time);
        body(2500, 2500);
        delay_ms(time);
        body(1500, 1900);
        chance = 0;
    }
    else if(state == 1)                     // 前进
    {
        if(chance != 1)
        {
            chance = 1;
            mood_good();
        }
        go();
        delay_ms(10);
    }
    else if(state == 2)                     // 后退
    {
        if(chance != 2)
        {
            chance = 2;
            mood_bad();
        }
        inter_time = 0;
        back();
    }
    else if(state == 3 && chance != 3)      // 停止
    {
        chance = 3;
        mood_good();
        body(1500, 1900);
        stop();
    }
    else if(state == 4 && chance != 4)      // 立正
    {
        mood_good();
        stop();
        body(1500, 2500);
        delay_ms(500);
        go();
        delay_ms(1000);
        stop();
        body(1500, 1900);
        chance = 4;
    }
    else if(state == 5 && chance != 5)      // 找尾巴
    {
        mood_good();
        left_wheel(1, 2500);
        right_wheel(0, 2500);
        body(500, 2500);
        chance = 5;
    }
    else if(state == 6)                     // 摇头晃脑
    {
        uint16_t chance_base = 1500;
        uint16_t chance_head = 2500;
        int i;
        
        inter_time = 0;
        mood = 11;
        dizzy_time = 0;
        stop();
        body(1500, 2500);
        
        for(i = 0; i < 50; i++)
        {
            chance_base -= 20;
            chance_head -= 5;
            body(chance_base, chance_head);
            delay_ms(10);
        }
        for(i = 0; i < 50; i++)
        {
            chance_base += 20;
            chance_head -= 15;
            body(chance_base, chance_head);
            delay_ms(10);
        }
        for(i = 0; i < 50; i++)
        {
            chance_base += 20;
            chance_head += 15;
            body(chance_base, chance_head);
            delay_ms(10);
        }
        for(i = 0; i < 50; i++)
        {
            chance_base -= 20;
            chance_head += 4;
            body(chance_base, chance_head);
            delay_ms(10);
        }
    }
    else if(state == 7 && chance != 7)      // 不要，摇头
    {
        int i;
        inter_time = 0;
        stop();
        body(1500, 2250);
        for(i = 0; i < 4; i++)
        {
            body(1600, 2250);
            delay_ms(200);
            body(1400, 2250);
            delay_ms(200);
        }
        body(1500, 2250);
        chance = 7;
    }
    else if(state == 8 && chance != 8)      // 点头
    {
        int i;
        mood_good();
        stop();
        body(1500, 2250);
        for(i = 0; i < 4; i++)
        {
            body(1500, 2250);
            delay_ms(200);
            body(1500, 1800);
            delay_ms(200);
        }
        body(1500, 1800);
        chance = 8;
    }
    else if(state == 9)                     // 睡觉
    {
        inter_time = 0;
        mood = 9;
        body(1500, 2500);
        stop();
    }
    else if(state == 10 && chance != 10)    // 晕
    {
        uint16_t chance_base = 1500;
        int i;
        
        inter_time = 0;
        chance = 10;
        mood = 11;
        face();
        stop();
        body(1500, 1300);
        
        for(i = 10; i > 0; i--)
        {
            body(chance_base + i * 100, 1300);
            delay_ms(time / 5);
            body(chance_base - i * 100, 1300);
            delay_ms(time / 5);
        }
        body(1500, 1900);
    }
    else if(state == 11 && chance != 11)    // 唤醒
    {
        stop();
        body(1500, 2250);
        go();
        delay_ms(500);
        body(1500, 1900);
        chance = 11;
    }
}
//-----------------------------------------------MODE 1---------------------------------------------------------------------------


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
	TIM4_Init();
	TIM3_PWM_Init();
	My_ADC_Init();// 采光

	
    
    while(1)
    {
       body(1000,0);
			delay_ms(500);
			body(1500,0);
			delay_ms(500);
    }
	  
    
   
}


void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		
		Key_Tick();
		ADC1_Tick();
		if(mode == 2) {
            receive_time++;
            if(receive_time > 100) receive_time = 100;//判断两次数字输入的时间间隔
        }
		if(mode == 3) {
            abnormal_deal();
        }
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

