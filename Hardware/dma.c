#include "dma.h"
extern uint8_t rx ;//串口接收字符
extern int receive_num,receive_sign,receive_double,receive_time,useful_num;//接收的数据   数据间隔
extern int state,mode,mood;
//void My_DMA_Init(void)
//{
//    // 1. 使能 DMA1 时钟
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//    
//    // 2. 如果需要 DMA 传输完成中断（可选）
//    NVIC_InitTypeDef NVIC_InitStructure;
//    
//    // ADC1 使用 DMA1 通道1
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}
//------------------------------------------------------------------初始化
void USART3_DMA_Init(void)
{
    // 1. 使能 DMA 时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    // 2. 配置 DMA1 通道3（USART3 RX）
	DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&rx;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    
    // 3. 配置 DMA 中断（可选）
	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 4. 使能 DMA 通道
    DMA_Cmd(DMA1_Channel3, ENABLE);
    
    // 5. 使能 USART3 的 DMA 接收请求（不是 USART 中断）
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
}
//----------------------------------------------------------------中断
void DMA1_Channel3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC3) == SET)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC3);
        // 处理 rx 中的语音指令
        if(rx=='1'){                    //数字处理
		receive_num=1;
		if(receive_time>10){
		receive_sign=receive_num;
		receive_double=0;
		}
		else if(receive_time<10){
		receive_double=10*receive_sign+receive_num;
		receive_sign=0;
		}
		}
		else if(rx=='2'){
		receive_num=2;
		if(receive_time>10){
		receive_sign=receive_num;
		receive_double=0;
		}
		else if(receive_time<10){
		receive_double=10*receive_sign+receive_num;
		receive_sign=0;
		}
		}
		else if(rx=='3'){
		receive_num=3;
		if(receive_time>10){
		receive_sign=receive_num;
		receive_double=0;
		}
		else if(receive_time<10){
		receive_double=10*receive_sign+receive_num;
		receive_sign=0;
		}
		}
		else if(rx=='4'){
		receive_num=4;
		if(receive_time>10){
		receive_sign=receive_num;
		receive_double=0;
		}
		else if(receive_time<10){
		receive_double=10*receive_sign+receive_num;
		receive_sign=0;
		}
		}
		else if(rx=='5'){
		receive_num=5;
		if(receive_time>10){
		receive_sign=receive_num;
		receive_double=0;
		}
		else if(receive_time<10){
		receive_double=10*receive_sign+receive_num;
		receive_sign=0;
		}
		}
		else if(rx=='6'){
		receive_num=6;
		if(receive_time>10){
		receive_sign=receive_num;
		receive_double=0;
		}
		else if(receive_time<10){
		receive_double=10*receive_sign+receive_num;
		receive_sign=0;
		}
		}
		else if(rx=='7'){
		receive_num=7;
		if(receive_time>10){
		receive_sign=receive_num;
		receive_double=0;
		}
		else if(receive_time<10){
		receive_double=10*receive_sign+receive_num;
		receive_sign=0;
		}
		}
		else if(rx=='8'){
		receive_num=8;
		if(receive_time>10){
		receive_sign=receive_num;
		receive_double=0;
		}
		else if(receive_time<10){
		receive_double=10*receive_sign+receive_num;
		receive_sign=0;
		}
		}
		else if(rx=='9'){
		receive_num=9;
		if(receive_time>10){
		receive_sign=receive_num;
		receive_double=0;
		}
		else if(receive_time<10){
		receive_double=10*receive_sign+receive_num;
		receive_sign=0;
		}
		}
		else if(rx=='A'){state=11;}//唤醒
		else if(rx=='B'){state=0;}
		else if(rx=='C'){state=1;}
		else if(rx=='D'){state=2;}
		else if(rx=='E'){state=3;}
		else if(rx=='F'){state=4;}
		else if(rx=='G'){state=5;}
		else if(rx=='H'){state=6;}
		else if(rx=='I'){state=7;}
		else if(rx=='J'){state=8;}
		else if(rx=='K'){state=9;}
		else if(rx=='L'){state=10;}
		else if(rx=='M'){mode=1;mood=12;}//环境检测
		else if(rx=='P'){mode=4;}//自由模式
		else if(rx=='Q'){mode=2;}//游戏模式
		else if(rx=='R'){mode=1;}//开心一点
		receive_time=0;
        // 重新启动 DMA
        DMA_Cmd(DMA1_Channel3, DISABLE);
        DMA_SetCurrDataCounter(DMA1_Channel3, 1);
        DMA_Cmd(DMA1_Channel3, ENABLE);
    }
}