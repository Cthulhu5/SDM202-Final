#include "CAN_receive.h"
#include "EMA_Control.h"
#include "tim.h"
#include "main.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

motor_InfoSet M3508_Info;
pid M3508_pid;

int16_t direction;
float loop;
int16_t target_angle;
int16_t tar_speed[2];

int16_t cnt_3508;
int16_t cnt_2006;

void M3508_SetAngle(int16_t set_angle)
{
	
	int16_t maxout = 3000;
	float max_speed = 75;
	float kp = 2;
	float ki = 0;
	float kd = 0;
	float freq = 1000;
	//Initialize
	
	for(int16_t i=0; i<2; i++)
	{
		M3508_pid.out[i] = 0;
		M3508_pid.speed_err[i] = 0;
		M3508_pid.angle_diff[i] = 0;
		M3508_pid.pout[i] = 0;
		M3508_pid.iout[i] = 0;
		M3508_pid.dout[i] = 0;
		M3508_pid.out[i] = 0;
	}

	//Rotate Command
	while(1)
	{

		M3508_pid_calculate(set_angle, max_speed, maxout, kp, ki, kd, freq);

		CAN_cmd_gimbal(M3508_pid.out[0],M3508_pid.out[1],0,0);
		HAL_Delay(1);
		
		//Data Feedback
		M3508_update();
		
		//Target Judgement

		if(
			(set_angle > 0 && (M3508_Info.currentAngle[0] >= (int32_t)set_angle || M3508_Info.currentAngle[1] >= (int32_t)set_angle)) ||
			(set_angle < 0 && (M3508_Info.currentAngle[0] <= (int32_t)set_angle || M3508_Info.currentAngle[1] <= (int32_t)set_angle))
		)
		{
//			speed_factor = 0;
			CAN_cmd_gimbal(-500,-500,0,0);
			HAL_Delay(100);
			CAN_cmd_gimbal(0,0,0,0);
			loop = 0;
			break;
		}
	}
}

void M3508_update()
{
	for(int16_t i=0; i<2; i++)
	{
		M3508_Info.currentSpeed[i] = (int16_t)get_gimbal_measure(i) -> speed_rpm;
	}
}

void M3508_pid_calculate(int16_t set_angle, int16_t max_speed, int16_t maxout, float kp, float ki, float kd, float freq)
{

		loop++;
		for(int16_t i=0; i<2; i++)
		{
			tar_speed[i] = M3508_GetTargetSpeed(set_angle, (int16_t)get_gimbal_measure(i) -> ecd, 75);
			
			M3508_pid.speed_err[i] = (float)tar_speed[i] * (3591/187) - get_gimbal_measure(i) -> speed_rpm;
			M3508_pid.angle_diff[i] = (int16_t)get_gimbal_measure(i) -> ecd - (int16_t)get_gimbal_measure(i) -> last_ecd;
			
			if(set_angle > 0 && M3508_pid.angle_diff[i] < -0)
			{
				M3508_pid.angle_diff[i] = 0;
				direction ++;
			}
			if(set_angle < 0 && M3508_pid.angle_diff[i] > +0)
			{
				M3508_pid.angle_diff[i] = 0;
				direction --;
			}
			
			M3508_Info.currentAngle[i] += M3508_pid.angle_diff[i] * 0.005f;
			
			M3508_pid.pout[i] = M3508_pid.speed_err[i] * kp;
			M3508_pid.dout[i] = (target_angle - M3508_Info.currentAngle[i]) * kd;
			
			M3508_pid.out[i] = M3508_pid.pout[i] + M3508_pid.dout[i];
			
			if(M3508_pid.out[i] > maxout)
			{
				M3508_pid.out[i] = maxout;
			}
						
			if(M3508_pid.out[i] < -maxout)
			{
				M3508_pid.out[i] = -maxout;
			}
			
		}
}

int16_t M3508_GetTargetSpeed(int16_t set_angle, int16_t cur_angle, int16_t max_speed)
{
	if(set_angle > 0)
	{
		if(cur_angle < 30 && cur_angle >= -5)
		{
			return (max_speed * (cur_angle + 15) / 30.0f);
		}
		if(cur_angle < set_angle - 30 && cur_angle >= 30)
		{
			return max_speed;
		}
		if(cur_angle <= set_angle && cur_angle >= set_angle - 30)
		{
			return (max_speed * (set_angle - cur_angle) / 30.0f);
		}
	}
	else
	{
		if(cur_angle < 30 && cur_angle >= -5)
		{
			return -(max_speed * cur_angle / 30.0f);
		}
		if(cur_angle < set_angle - 30 && cur_angle >= 30)
		{
			return -max_speed;
		}
		if(cur_angle <= set_angle && cur_angle >= set_angle - 30)
		{
			return -(max_speed * (set_angle - cur_angle) / 30.0f);
		}
	}
}

void M3508_SetTimeRotate(int16_t speed, int16_t time_ms)
{
	cnt_3508 = 0;
	while(cnt_3508 < time_ms)
	{
		CAN_cmd_gimbal(speed,speed,0,0);
		HAL_Delay(1);
		cnt_3508++;
	}
	CAN_cmd_gimbal(0,0,0,0);
}

void M2006_SetTimeRotate(int16_t speed, int16_t time_ms)
{
	cnt_2006 = 0;
	while(cnt_2006 < time_ms)
	{
		CAN_cmd_chassis(0,0,speed,-speed);
		HAL_Delay(1);
		cnt_2006++;
	}
	CAN_cmd_chassis(0,0,0,0);
}

void Cylinder_Set(uint8_t num, uint8_t set_state, uint16_t time_ms)
{
	GPIO_PinState cylinder_state = (GPIO_PinState)(set_state = 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	switch(num)
	{
		case 1:
			HAL_GPIO_WritePin(AC1_GPIO_Port, AC1_Pin, cylinder_state);
			break;
		case 2:
			HAL_GPIO_WritePin(AC2_GPIO_Port, AC2_Pin, cylinder_state);
			break;
		case 3:
			HAL_GPIO_WritePin(AC3_GPIO_Port, AC3_Pin, cylinder_state);
			break;
		case 4:
			HAL_GPIO_WritePin(AC4_GPIO_Port, AC4_Pin, cylinder_state);
			break;
		case 5:
			HAL_GPIO_WritePin(AC5_GPIO_Port, AC5_Pin, cylinder_state);
			break;
		default:
			break;
	}
	HAL_Delay(time_ms);
}

void PWM_SetAngle(int16_t angle)
{
	__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, angle);
}