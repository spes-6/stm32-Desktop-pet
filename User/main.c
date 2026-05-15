#include "stm32f10x.h"
#include "delay.h"
#include "dht11.h"
#include "my_usart.h"
#include "OLED_Data.h"
#include "OLED.h"
#include "my_gpio.h"

//----------------------------------全局变量定义-------------------------------------------------------------------------
int feel_left=0,feel_right=0,anger=0;//灭灯为1,anger为怒气值
int turn_left_A,turn_right_A;//暂停使用标志位
int mood=7,clear=0;//表示心情  清屏
//----------------------------------end-------------------------------------------------------------------------

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
		OLED_ShowImage(0,0,128,64,BMP1);
//		if(dizzy_time>=30){
//		  mood=2;
//			dizzy_time=0;
//		}
//		}
//		else if(mood==12){   //环境检测
//		  if(clear!=12){
//			OLED_Clear();	
//			}
//			clear=12;
//			data_show();
//		}
//		OLED_Refresh();
}}
int main(void)
{
	I2C_GPIO_Init();
  OLED_Init();
	OLED_Clear();
	OLED_ShowImage(0,0,128,64,BMP11);
	OLED_Update();
	delay_s(5);
	face();
	OLED_Update();
    while(1)
    {
        
    }
}


