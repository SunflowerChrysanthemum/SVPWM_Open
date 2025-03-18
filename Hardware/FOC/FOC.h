#ifndef __FOC_H
#define __FOC_H

#include "sys.h"

#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
		

typedef struct
{              // ���� FOC �ṹ������
	float u_d;   // ���Ʊ��� u_d
	float u_q;   // ���Ʊ��� u_q
	float theta; // �Ƕ� theta
	
	float u_alpha; // �任��Ŀ��Ʊ��� u_alpha
	float u_beta;  // �任��Ŀ��Ʊ��� u_beta
	
	float t_a; // ��ѹʸ�� t_a
	float t_b; // ��ѹʸ�� t_b
	float t_c; // ��ѹʸ�� t_c
	
	float i_a; // ���� i_a
	float i_b; // ���� i_b
	float i_c; // ���� i_c
	
	float i_alpha; // �任��ĵ��� i_alpha
	float i_beta;  // �任��ĵ��� i_beta
	
	float i_d; // d ����� i_d
	float i_q; // q ����� i_q
	
} FOC;       // FOC �ṹ����������


void ipark(FOC *foc);  // Inverse Park �� Park �任��������
void ipark2(FOC *foc); // Inverse Park �� Park �任������������һ��ʵ�ַ�ʽ��
void clarke(FOC *foc); // Clarke �任��������
void park(FOC *foc);   // Park �任��������
void svpwm(FOC *foc);  // Space Vector PWM (SVPWM) ��������

#endif
