#ifndef EMA_CONTROL_H
#define EMA_CONTROL_H

#include "struct_typedef.h"

#define CHASSIS_CAN hcan1
#define GIMBAL_CAN hcan2

typedef struct
{

	float currentAngle[2];
	int16_t currentSpeed[2];
	
	int16_t lastAngle[2];
	
} motor_InfoSet;

typedef struct
{
    int16_t speed_err[2];
    int16_t angle_diff[2];

    float pout[2];
    float iout[2];
    float dout[2];
    float out[2];
} pid;

extern void M3508_SetAngle(int16_t angle);

extern void M3508_initialize(void);

extern void M3508_update(void);

extern void M3508_pid_calculate(int16_t target_angle, int16_t max_speed, int16_t maxout, float kp, float ki, float kd, float freq);

extern int16_t M3508_GetTargetSpeed(int16_t set_angle, int16_t cur_angle, int16_t max_speed);

extern void M3508_SetTimeRotate(int16_t speed, int16_t time_ms);

extern void M2006_SetTimeRotate(int16_t speed, int16_t time_ms);

extern void Cylinder_Set(uint8_t num, uint8_t set_state, uint16_t time_ms);

#endif
