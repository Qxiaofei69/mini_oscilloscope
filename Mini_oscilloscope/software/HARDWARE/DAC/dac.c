#include "dac.h"
#include "math.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//DAC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/8
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//正弦波单个周期的点数
//#define POINT_NUM 32

/* 波形数据 ---------------------------------------------------------*/
//const uint16_t Sine12bit[POINT_NUM] = {
//	2048	, 2460	, 2856	, 3218	, 3532	, 3786	, 3969	, 4072	,
//	4093	, 4031	, 3887	, 3668	, 3382	, 3042	,	2661	, 2255	, 
//	1841	, 1435	, 1054	, 714		, 428		, 209		, 65		, 3			,
//	24		, 127		, 310		, 564		, 878		, 1240	, 1636	, 2048
//};
//uint32_t DualSine12bit[POINT_NUM];
#define PI  3.14159
#define DAC_DHR12R2_ADDRESS     0x40007414
uint32_t DualSine12bit[POINT_NUM];

//DAC通道1输出初始化
void Dac2_Init(void)
{
  
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //使能PORTA通道时钟
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //模拟输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=0;			
	DAC_InitType.DAC_Trigger=DAC_Trigger_T2_TRGO;	//使用触发功能 TIM2作为触发源
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
   
	DAC_Init(DAC_Channel_2,&DAC_InitType);	 //初始化DAC通道1

	
	DAC_Cmd(DAC_Channel_2, ENABLE);  //使能通道1 由PA5输出
	

   
     /* 使能DAC的DMA请求   对 DAC1 和 2 的操作是同步的，所以只要把 DMA 与
     DAC 通道 2 关联起来即可，当使用 DMA 设置通道 2 的数据值时， 同时更新通道 1 的内容*/
     DAC_DMACmd(DAC_Channel_2, ENABLE);
}
static void DAC_TIM_Config(void)
{
	
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	/* 使能TIM2时钟，TIM2CLK 为72M */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* TIM2基本定时器配置 */
	TIM_TimeBaseStructure.TIM_Period = (20-1);       	//定时周期 2000 
	TIM_TimeBaseStructure.TIM_Prescaler = (72-1);       	//预分频，不分频 72M / (0+1) = 72M
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    //时钟分频系数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* 配置TIM2触发源 */
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	/* 使能TIM2 */
	TIM_Cmd(TIM2, DISABLE);

}

static void DAC_DMA_Config(void)
{	
	DMA_InitTypeDef  DMA_InitStructure;

	/* 使能DMA2时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

	/* 配置DMA2 */
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R2_ADDRESS;					//外设数据地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&DualSine12bit ;				//内存数据地址 DualSine12bit
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											//数据传输方向内存至外设
	DMA_InitStructure.DMA_BufferSize = POINT_NUM;																	//缓存大小为POINT_NUM字节	
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//外设数据地址固定	
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//内存数据地址自增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//外设数据以字为单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;					//内存数据以字为单位	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//高DMA通道优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//非内存至内存模式	

	DMA_Init(DMA2_Channel4, &DMA_InitStructure);

	/* 使能DMA2-4通道 */
	DMA_Cmd(DMA2_Channel4, ENABLE);
}

void DAC_Mode_Init_2(void)
{
	u16 n;
	float outdata=0;
	Dac2_Init();
	DAC_TIM_Config();
	for(n=0;n<POINT_NUM;n++)
	{
		outdata = (sin(2*PI*n/POINT_NUM)+1)*2047;
        //printf("%f\r\n",outdata);
        DualSine12bit[n]=outdata;		
	}	
	DAC_DMA_Config();
}
