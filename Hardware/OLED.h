#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include "OLED_Data.h"

/*参数宏定义*********************/

/*FontSize参数取值*/
/*此参数值不仅用于判断，而且用于计算横向字符偏移，默认值为字体像素宽度*/
#define OLED_8X16				8
#define OLED_6X8				6

/*IsFilled参数数值*/
#define OLED_UNFILLED			0
#define OLED_FILLED				1

/*********************参数宏定义*/

/*引脚宏定义*********************/

// OLED 引脚配置（I2C 接口）
// 根据实际接线修改下面的宏定义

// 方案1：PB8/PB9（当前使用）
#define OLED_SCL_PORT      GPIOB
#define OLED_SCL_PIN       GPIO_Pin_0
#define OLED_SDA_PORT      GPIOB
#define OLED_SDA_PIN       GPIO_Pin_1
#define OLED_GPIO_CLK      RCC_APB2Periph_GPIOB

// 方案2：PB6/PB7（I2C1 默认引脚，取消注释即可使用）
// #define OLED_SCL_PORT      GPIOB
// #define OLED_SCL_PIN       GPIO_Pin_6
// #define OLED_SDA_PORT      GPIOB
// #define OLED_SDA_PIN       GPIO_Pin_7
// #define OLED_GPIO_CLK      RCC_APB2Periph_GPIOB

// 方案3：PA8/PA9
// #define OLED_SCL_PORT      GPIOA
// #define OLED_SCL_PIN       GPIO_Pin_8
// #define OLED_SDA_PORT      GPIOA
// #define OLED_SDA_PIN       GPIO_Pin_9
// #define OLED_GPIO_CLK      RCC_APB2Periph_GPIOA

// 引脚电平操作宏
#define OLED_SCL_HIGH()    GPIO_SetBits(OLED_SCL_PORT, OLED_SCL_PIN)
#define OLED_SCL_LOW()     GPIO_ResetBits(OLED_SCL_PORT, OLED_SCL_PIN)
#define OLED_SDA_HIGH()    GPIO_SetBits(OLED_SDA_PORT, OLED_SDA_PIN)
#define OLED_SDA_LOW()     GPIO_ResetBits(OLED_SDA_PORT, OLED_SDA_PIN)
#define OLED_SDA_READ()    GPIO_ReadInputDataBit(OLED_SDA_PORT, OLED_SDA_PIN)

/*********************引脚宏定义*/
/*函数声明*********************/

/*初始化函数*/
void OLED_Init(void);

/*更新函数*/
void OLED_Update(void);
void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显存控制函数*/
void OLED_Clear(void);
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);
void OLED_Reverse(void);
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显示函数*/
void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);
void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize);
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...);

/*绘图函数*/
void OLED_DrawPoint(int16_t X, int16_t Y);
uint8_t OLED_GetPoint(int16_t X, int16_t Y);
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);
void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);
void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);

/*********************函数声明*/

#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
