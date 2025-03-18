#ifndef __CAPTURE_H
#define __CAPTURE_H

#include "sys.h"

typedef struct 
{
	uint32_t period;
	uint16_t frq;
	uint32_t high_time;
	float duty;
	float angle_du;   //0~360
	float angle_rad; //ª°∂»÷∆
	float electronic_angle;
	float angle_rad_offset;
}PWM_ENCODER;

extern PWM_ENCODER pwm_encod;


void pwm_Init(void);
void Tim3_CaptureInit(void);

#endif
