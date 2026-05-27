#include "adc.h"

static volatile uint16_t adc_raw_value = 0;
static volatile uint16_t light_percent = 50;

/**
 * @brief ADC1 初始化
 */
void My_ADC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    
    // 2. 配置 PA0 为模拟输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 配置 ADC
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);  // 这里是标准库的函数，没问题
    
    // 4. 配置通道
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    
    // 5. 配置中断
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 使能ADC转换完成中断
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    
    // 6. 校准
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

/**
 * @brief 启动一次ADC转换
 */
void My_ADC_Start(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/**
 * @brief 获取最新ADC原始值
 */
uint16_t My_ADC_GetValue(void)
{
    return adc_raw_value;
}

/**
 * @brief 获取光照百分比
 */
uint16_t My_ADC_GetLight(void)
{
    return light_percent;
}


static uint8_t adc_sample_cnt = 0;
void ADC1_Tick(void)
{
			adc_sample_cnt++;
    if(adc_sample_cnt >= 20)  // 20ms 采样一次
    {
        adc_sample_cnt = 0;
        
        // 启动转换
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        
        // 等待完成（ADC 转换很快，几十微秒）
        while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
        
        // 读取数值
        uint16_t value = ADC_GetConversionValue(ADC1);
        
        // 处理
        if(value >= 3000) value = 3000;
        light_percent = value / 30;
        if(light_percent == 0) light_percent = 1;
    }
}

uint16_t ADC_GetLight(void)
{
    return light_percent;

}
/**
 * @brief ADC1中断服务函数
 */
//void ADC1_2_IRQHandler(void)
//{
//    if(ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET)
//    {
//        adc_raw_value = ADC_GetConversionValue(ADC1);
//        
//        // 限幅 0~3000
//        if(adc_raw_value >= 3000) {
//            adc_raw_value = 3000;
//        }
//        
//        // 转换 0~100
//        light_percent = adc_raw_value / 30;
//        if(light_percent == 0) {
//            light_percent = 1;
//        }
//        
//        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
//    }
//}