#include "adc.h"

static volatile uint16_t adc_raw_value = 0;
static volatile uint16_t light_percent = 50;
extern int left_x, left_y, right_x, right_y;

/**
 * @brief ADC1 初始化
 */
 
 volatile uint16_t ADC_Values[4] = {0};  // ADC 采样值存储
void My_ADC_Init(void)
{
  



    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    
    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    // 2. 配置 GPIO：PA0, PA1, PA2, PA3 为模拟输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 配置 DMA：ADC1 自动搬运数据到 ADC_Values 数组
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Values;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 4;           // 4个通道
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  // 循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    // 4. 配置 ADC：扫描模式 + 连续转换
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;     // 扫描模式（多通道）
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // 连续转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 4;          // 4个通道
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // 5. 配置 ADC 通道和转换顺序
    // 顺序1: 通道0 (左摇杆 X)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    // 顺序2: 通道1 (左摇杆 Y)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    // 顺序3: 通道2 (右摇杆 X)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
    // 顺序4: 通道3 (右摇杆 Y)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
    
    // 6. 使能 DMA 请求
    ADC_DMACmd(ADC1, ENABLE);
    
    // 7. 校准 ADC
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    
    // 8. 启动 ADC 转换（DMA 会自动搬运）
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}



//-------------数据转换-------------------
void ADC_data(void)
{
    if(ADC_Values[0] <= 1000)      left_x = 1;
    else if(ADC_Values[0] >= 3000) left_x = 2;
    else                           left_x = 0;
    
    if(ADC_Values[1] <= 1000)      left_y = 1;
    else if(ADC_Values[1] >= 3000) left_y = 2;
    else                           left_y = 0;
    
    if(ADC_Values[2] <= 1000)      right_x = 1;
    else if(ADC_Values[2] >= 3000) right_x = 2;
    else                           right_x = 0;
    
    if(ADC_Values[3] <= 1000)      right_y = 1;
    else if(ADC_Values[3] >= 3000) right_y = 2;
    else                           right_y = 0;
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