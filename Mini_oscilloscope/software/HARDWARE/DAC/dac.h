#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	    
#define POINT_NUM 256								    
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
//#define DAC_DHR12RD_ADDRESS      (DAC_BASE+0x20)								    

void Dac2_Init(void);//回环模式初始化		 	 
void DAC_Mode_Init_2(void);
#endif

















