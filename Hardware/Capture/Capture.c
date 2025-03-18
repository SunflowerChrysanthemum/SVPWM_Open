#include "Capture.h"

void pwm_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	//定时器1通道1、2、3引脚IO初始化
	//控制正极，pwm控制就可以利用占空比控制电机速度
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//定时器1互补通道引脚IO初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//刹车引脚IO初始化， 浮空输入模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	//定时器的频率 1800//72 000 000---40kHZ
	TIM_InternalClockConfig(TIM1);
	TIM_TimeBaseInitTypeDef TIM1_TimeBaseInitStructure;
	TIM1_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM1_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
	TIM1_TimeBaseInitStructure.TIM_Period = PWM_PERIOD - 1;
	TIM1_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; 
	TIM1_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM1_TimeBaseInitStructure);
	
	
	//定时器比较输出模式1
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	
	
	//不要产生更新事件才改变设置;换相会有延迟
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);

	
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}


void Encode_ICInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseInitStructure.TIM_Period = 65536 - 1;
	TIM_BaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_BaseInitStructure);
	
	
	//通道1
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	//通道2
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	
	/* -------------------中断配置------------------------ */
	TIM_ClearFlag(TIM3 ,TIM_FLAG_Update);
	
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);
}
	

