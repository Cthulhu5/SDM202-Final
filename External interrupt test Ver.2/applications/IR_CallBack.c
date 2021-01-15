# include "main.h"
#include "CAN_RotateAngle.h"


int32_t last_time = 0;
int32_t time = 0;
int32_t id1 = 0;
int32_t id2 = 0;
int32_t id3 = 0;
int32_t inf1 = 0;
int32_t inf2 = 0;
int32_t counter = 0;

//int32_t flag1 = 0;
//int32_t flag2 = 0;
//int32_t flag3 = 0;
//int32_t flag4 = 0;
//int32_t flag5 = 0;
//int32_t flag6 = 0;
//int32_t flag7 = 0;
//int32_t flag8 = 0;
//int32_t flag9 = 0;
//int32_t flag10 = 0;
/**
  * @brief          exit callback function
  * @param[in]      GPIO_Pin:gpio pin
  * @retval         none
  */
/**
  * @brief          外部中断回调
  * @param[in]      GPIO_Pin:引脚号
  * @retval         none
  */
	
uint16_t IR_ControlMode()
{
	if(inf1 == 0)
		{
			if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET)
			{
				if(40<(time-last_time) & (time-last_time)<60)
				{
					inf1 = 1;
				}
				if(90<(time-last_time) & (time-last_time)<110)
				{
					inf1 = 2;
				}
			}
		}
		if(inf1 != 0)
		{
			if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET)
			{
				if(40<(time-last_time) & (time-last_time)<60)
				{
					inf2 = 1;
				}
				if(90<(time-last_time) & (time-last_time)<110)
				{
					inf2 = 2;
				}
			}
		}	
		
		if(inf1 == 1 & inf2 ==1 )
		{
			return 1;
		}
		if(inf1 == 2 & inf2 ==2 )
		{
			return 2;
		}
		else{
			return 0;
		}
}

uint16_t IR_identification()
{
	if(id1 == 0 | id2 == 0 | id3 == 0)
		{
			if(id1 == 0)
			{
				if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET)
				{
					if(90<(time-last_time) & (time-last_time)<110)
					{
						id1 = 1;
					}
					else
					{
						id1 = 0;
					}
				}
			}
			if(id1 == 1)
			{
				if( HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET)
				{
					if(90<(time-last_time) & (time-last_time)<110)
					{
						id2 = 1;
					}
					else
					{
						id1 = 0;
						id2 = 0;
					}
				}
			}
			if(id1 ==1 & id2 == 1)
			{
				if(190<(time-last_time) & (time-last_time)<210)
				{
					if(190<(time-last_time) & (time-last_time)<210)
					{
						id3 = 1;
					}
					else
					{
						id1 = 0;
						id2 = 0;
						id3 = 0;
					}
				}
			}
		}
		if(id1 == 1 & id2 == 1 & id3 == 1)
		{
			return 1;
		}
		else{
			return 0;
		}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		last_time = time;
		time = HAL_GetTick();
		counter = counter + 1;
		if(GPIO_Pin == KEY_Pin)
		{
				HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_SET);   //绿色LED灯亮0.2s后关闭
				HAL_Delay(200);
				HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, GPIO_PIN_RESET);
//			 if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
//			 {
//				 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9,GPIO_PIN_RESET);
//			 }
//				if( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
//			 {
//				 HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9,GPIO_PIN_SET);
//			 }
		}
    if(GPIO_Pin == IR_Pin)
    {
			if(IR_identification() == 1) 
			{
				if(IR_ControlMode() == 1)
				{
					HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_SET);   //蓝色LED灯亮0.2s后关闭
					HAL_Delay(200);
					HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, GPIO_PIN_RESET);
					id1 = 0;
					id2 = 0;
					id3 = 0;
					inf1 = 0;
					inf2 = 0;
					time = 0;
					last_time = 0;
				}
				
				if(IR_ControlMode() == 2)
				{
//					HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_SET);   //红色LED灯亮0.2s后关闭
//					HAL_Delay(200);
//					HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, GPIO_PIN_RESET);
					gimbal_RotateAngle(1, 2048, 1, 1);
					id1 = 0;
					id2 = 0;
					id3 = 0;
					inf1 = 0;
					inf2 = 0;		
					time = 0;
					last_time = 0;
				}
			}	
		}
}
