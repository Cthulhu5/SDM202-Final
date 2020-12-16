#ifndef CAN_ROTATEANGLE_H
#define CAN_ROTATEANGLE_H

#include "struct_typedef.h"

#define CHASSIS_CAN hcan1
#define GIMBAL_CAN hcan2

extern void gimbal_RotateAngle(uint16_t motor5_angle, uint16_t motor6_angle, uint16_t motor7_angle, uint16_t motor8_angle);

#endif
