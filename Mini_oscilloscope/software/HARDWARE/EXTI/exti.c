#include "exti.h"
#include "key.h"
#include "SBQ.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_exti.h"
//外部中断服务程序
void EXTIX_Init(void)
{
 
 	  EXTI_InitTypeDef EXTI_InitStructure;
 	  NVIC_InitTypeDef NVIC_InitStructure;

		KEY_Init();	 //	按键端口初始化
	
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟


  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;

   //GPIOB.5	  中断线以及中断初始化配置 下降沿触发 //KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOB.6	  中断线以及中断初始化配置  下降沿触发	//KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//GPIOB.7	  中断线以及中断初始化配置  下降沿触发	//KEY2
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//GPIOB.8	  中断线以及中断初始化配置  下降沿触发	//KEY3
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0~3所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
} 
u8 KEY_Value;
extern _oscill_dev oscilldev;
void EXTI9_5_IRQHandler(void)
{
		if(KEY0==0)	 //按键KEY0
		{
			KEY_Value = 0;
			EXTI_ClearITPendingBit(EXTI_Line5);
		}
		else if(KEY1 == 0)//按键KEY1
		{
			KEY_Value = 1;
			EXTI_ClearITPendingBit(EXTI_Line6);
		}
		else if(KEY2 == 0)//按键KEY2
		{
			KEY_Value = 2;
			EXTI_ClearITPendingBit(EXTI_Line7);
		}
		else if(KEY3 == 0)//按键KEY3
		{
			KEY_Value = 3;
			EXTI_ClearITPendingBit(EXTI_Line8);
		}
		else  ;
}
