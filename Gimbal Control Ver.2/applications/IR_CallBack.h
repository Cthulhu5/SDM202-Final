# include "main.h"
#ifndef IR_CALLBACK_H

extern void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
extern uint16_t IR_identification();
extern uint16_t IR_ControlMode();

#endif