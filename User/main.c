#include "sys.h"

//���Ϊ4����

PWM_ENCODER pwm_encod;

int main(void)
{
	uart3_init(115200);//����3��ʼ��
	NVIC_Config();//�ж����ȼ�����
	delay_init();
//  Systick_CountMode();//Systick��ʱ��ʼ��(ͬʱ������ʱ����)
	pwm_Init();
	
	FOC foc;;
    float theta;
    float tolerance = 1e-5;
	
	while(1)
	{
		
		theta += 5e-3;
		foc.u_d = 0;
		foc.u_q = 0.2;
		foc.theta = theta;
		ipark(&foc);
		svpwm(&foc);
		float u_a = foc.t_a - 0.5 * (foc.t_b + foc.t_c);
		float u_b = foc.t_b - 0.5 * (foc.t_a + foc.t_c);
		float u_c = -(u_a + u_b);
		printf("%f,%f,%f\n", u_a, u_b, u_c);
		
	}
}


void TIM3_IRQHandler(void)
{
	 if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	 {
		 TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		 
		 // ��ȡͨ��1����ֵ����ʾ����
		 pwm_encod.period = TIM_GetCapture1(TIM3);
		 // ����Ƶ�ʣ���ʱ��Ƶ��Ϊ1 MHz��
		 pwm_encod.frq = pwm_encod.period*0.1f;
		 //����ռ�ձ�
		 pwm_encod.duty = (float)pwm_encod.high_time / (float)pwm_encod.period;
	 }
	 if(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	 {
		 TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		 
		 // ��ȡͨ��2����ֵ����ʾ�ߵ�ƽʱ��
		 pwm_encod.high_time = TIM_GetCapture2(TIM3);
	 }
}

