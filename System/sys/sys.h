#ifndef __SYS_H
#define __SYS_H	 

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "FOC.h"
#include "Capture.h"



#include <string.h> 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "IQmathLib.h"

//常量宏定义
#define _2_SQRT3 1.15470053838
#define _SQRT3 1.73205080757
#define _1_SQRT3 0.57735026919
#define _SQRT3_2 0.86602540378
#define _SQRT2 1.41421356237
#define _120_D2R 2.09439510239
#define PI 3.14159265359
#define _PI 3.14159265359
#define _PI_2 1.57079632679
#define _PI_3 1.0471975512
#define _2PI 6.28318530718
#define _3PI_2 4.71238898038
#define _PI_6 0.52359877559

#define Udc		24


#define RUN			1			
#define STOP		0
#define cw			1		//顺时针
#define ccw			0		//逆时针

typedef void(*pctr) (void);

#define PWM_PERIOD		1800


void NVIC_Config(void);

#endif
