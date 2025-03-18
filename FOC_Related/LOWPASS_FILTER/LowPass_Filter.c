#include "LOWPASS_FILTER.h"

float y_prev;

/***********************************************
��ͨ�˲���:
***********************************************/
float LowPass_Filter(float x)
{
	float y = 0.9*y_prev + 0.1*x;
	
	y_prev=y;
	
	return y;
}

/*******************************************
SysTick��ʼ��(ģ��micros������ʼ��)��

��ע��     
	1.��0xFFFFFF��0ѭ������
	2.�������޷���ʹ��delay��ʱ����
*******************************************/
void Systick_CountMode(void)
{
	SysTick->LOAD = 0xFFFFFF-1;
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
}
