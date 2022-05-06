#ifndef _SBQ_H
#define _SBQ_H
#include "sys.h"

typedef struct 
{
	u16 buffer[128];//数据存放区
	u8 	flag;		//是否空闲
	u32  tim;		//一格代表几us
    float resolut;  //每个AD原始值代表多大的电压(单位为mv)
	float a;		//坐标轴参数a
	float b;		//坐标轴参数b
}_oscill_dev;

void DrawOscillogram(void);
void Draw_Menu(void);

#endif
