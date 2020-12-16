#include "CAN_receive.h"
#include "main.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

int16_t gimbal_initialAngle[4];
int16_t gimbal_offsetAngle[4];
int16_t gimbal_targetAngle[4];
int16_t gimbal_currentAngle[4];
int16_t loopIndex = 0;
int16_t initialIndex = 0;


void gimbal_RotateAngle(int16_t motor5_angle, int16_t motor6_angle, int16_t motor7_angle, int16_t motor8_angle)
{
  while (loopIndex < 51)
  {
		CAN_cmd_gimbal(0,5000,0,0);
		HAL_Delay(1);
		for(uint16_t i=0; i<4; i++)
		{
			HAL_CAN_RxFifo0MsgPendingCallback(&hcan2);
			gimbal_currentAngle[i] = (int16_t)get_gimbal_motor_measure_point(i) -> ecd;
		}
		if(initialIndex == 0)
		{
			for(uint16_t k=0; k<4; k++)
			{
				gimbal_initialAngle[k] = gimbal_currentAngle[k];
				gimbal_offsetAngle[k] = + 90 * 8192 / 360;
				gimbal_targetAngle[k] = gimbal_initialAngle[k] + gimbal_offsetAngle[k];
				HAL_Delay(1000);
				initialIndex++;
			}
		}
		for(uint16_t j=0; j<4; j++)
		{
			if(gimbal_targetAngle[j] > 8191 && gimbal_currentAngle[j] < gimbal_initialAngle[j])
			{
				gimbal_targetAngle[j] -= 8191;
			}
		
			if(gimbal_currentAngle[j] > gimbal_targetAngle[j])
			{
				CAN_cmd_gimbal(0, 0, 0, 0);
			
				HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12|GPIO_PIN_11|GPIO_PIN_10, GPIO_PIN_SET);
				HAL_Delay(1000);
				HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12|GPIO_PIN_11|GPIO_PIN_10, GPIO_PIN_RESET);
				gimbal_targetAngle[j] += gimbal_offsetAngle[j];
				loopIndex ++;
			}
		}
  }
}
