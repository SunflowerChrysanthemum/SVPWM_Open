#include "sys.h"

void ipark(FOC *foc)
{
    // 将直角坐标系下的电压转换为αβ坐标系下的电压
    // 通过正弦和余弦函数计算
    float sine = sinf(foc->theta);   // 使用sinf函数代替sin函数
    float cosine = cosf(foc->theta); // 使用cosf函数代替cos函数
    foc->u_alpha = foc->u_d * cosine - foc->u_q * sine;
    foc->u_beta = foc->u_q * cosine + foc->u_d * sine;
}

void ipark2(FOC *foc)
{
  // 将直角坐标系下的电压转换为αβ坐标系下的电压
  // 直接使用cos和sin函数计算
  foc->u_alpha = foc->u_d * cos(foc->theta) - foc->u_q * sin(foc->theta);
  foc->u_beta = foc->u_q * cos(foc->theta) + foc->u_d * sin(foc->theta);
}

void clarke(FOC *foc)
{
  // 将三相电流转换为αβ坐标系下的电流
  // 根据公式进行计算
  foc->i_alpha = foc->i_a;
  foc->i_beta = (foc->i_a + 2 * foc->i_b) * 0.5773502691896257; // 1/√3 ≈ 0.57735
}

void park(FOC *foc)
{
  // 将αβ坐标系下的电流转换为直角坐标系下的电流
  // 通过正弦和余弦函数计算
  float sine = sin(foc->theta);
  float cosine = cos(foc->theta);
  foc->i_d = foc->i_alpha * cosine + foc->i_beta * sine;
  foc->i_q = foc->i_beta * cosine - foc->i_alpha * sine;
}


//K=1即Udc = sqrt(3)*Ts			调制指数最大		线电压最大Uline_max = sqrt(3)/2 * Udc
void svpwm(FOC *foc)
{
	const float ts = 1e-4; // SVPWM的采样周期
	
	// 计算SVPWM算法中的三个控制电压u1、u2和u3
	float u1 = foc->u_beta;
	float u2 = -0.8660254037844386 * foc->u_alpha - 0.5 * foc->u_beta; // sqrt(3)/2 ≈ 0.86603
	float u3 = 0.8660254037844386 * foc->u_alpha - 0.5 * foc->u_beta;
	
	float K = _SQRT3 * ts / Udc;
	
	// 根据u1、u2和u3的正负情况确定所处的扇区
	uint8_t sector = (u1 > 0.0) + ((u2 > 0.0) << 1) + ((u3 > 0.0) << 2);
	
	// 根据扇区的不同，计算对应的t_a、t_b和t_c的值，表示生成的三相电压的时间
	if (sector == 5)
	{
		// 扇区5的计算
		float t4 = u3 * K;
		float t6 = u1 * K;
		float sum = t4 + t6;
		if (sum > ts)
		{
		float k_svpwm = ts / sum; // 计算缩放系数
		t4 = k_svpwm * t4;
		t6 = k_svpwm * t6;
		}
		float t7 = (ts - t4 - t6) / 2;
		foc->t_a = t4 + t6 + t7;
		foc->t_b = t6 + t7;
		foc->t_c = t7;
	}
	else if (sector == 1)
	{
		// 扇区1的计算
		float t2 = -u3 * K;
		float t6 = -u2 * K;
		float sum = t2 + t6;
		if (sum > ts)
		{
		float k_svpwm = ts / sum; // 计算缩放系数
		t2 = k_svpwm * t2;
		t6 = k_svpwm * t6;
		}
		float t7 = (ts - t2 - t6) / 2;
		foc->t_a = t6 + t7;
		foc->t_b = t2 + t6 + t7;
		foc->t_c = t7;
	}
	else if (sector == 3)
	{
		// 扇区3的计算
		float t2 = u1 * K;
		float t3 = u2 * K;
		float sum = t2 + t3;
		if (sum > ts)
		{
		float k_svpwm = ts / sum; // 计算缩放系数
		t2 = k_svpwm * t2;
		t3 = k_svpwm * t3;
		}
		float t7 = (ts - t2 - t3) / 2;
		foc->t_a = t7;
		foc->t_b = t2 + t3 + t7;
		foc->t_c = t3 + t7;
	}
	else if (sector == 2)
	{
		// 扇区2的计算
		float t1 = -u1 * K;
		float t3 = -u3 * K;
		float sum = t1 + t3;
		if (sum > ts)
		{
		float k_svpwm = ts / sum; // 计算缩放系数
		t1 = k_svpwm * t1;
		t3 = k_svpwm * t3;
		}
		float t7 = (ts - t1 - t3) / 2;
		foc->t_a = t7;
		foc->t_b = t3 + t7;
		foc->t_c = t1 + t3 + t7;
	}
	else if (sector == 6)
	{
		// 扇区6的计算
		float t1 = u2 * K;
		float t5 = u3 * K;
		float sum = t1 + t5;
		if (sum > ts)
		{
		float k_svpwm = ts / sum; // 计算缩放系数
		t1 = k_svpwm * t1;
		t5 = k_svpwm * t5;
		}
		float t7 = (ts - t1 - t5) / 2;
		foc->t_a = t5 + t7;
		foc->t_b = t7;
		foc->t_c = t1 + t5 + t7;
	}
	else if (sector == 4)
	{
		// 扇区4的计算
		float t4 = -u2 * K;
		float t5 = -u1 * K;
		float sum = t4 + t5;
		if (sum > ts)
		{
		float k_svpwm = ts / sum; // 计算缩放系数
		t4 = k_svpwm * t4;
		t5 = k_svpwm * t5;
		}
		float t7 = (ts - t4 - t5) / 2;
		foc->t_a = t4 + t5 + t7;
		foc->t_b = t7;
		foc->t_c = t5 + t7;
	}
//	TIM_SetCompare1(TIM1, foc->t_a);
//	TIM_SetCompare1(TIM1, foc->t_b);
//	TIM_SetCompare1(TIM1, foc->t_c);
}

