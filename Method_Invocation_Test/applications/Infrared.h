#ifndef INFRARED_H
#define INFRARED_H

#include "struct_typedef.h"
#include "main.h"

typedef struct
{
	uint16_t num1_normal;
	uint16_t num2_normal;
	uint16_t num3_normal;
	uint16_t num1_inverse;
	uint16_t num2_inverse;
	uint16_t num3_inverse;
}
Infrared_Byte;

typedef struct
{
	uint16_t v1;
	uint16_t v2;
	uint16_t v3;
	uint16_t v4;
	uint16_t num;
}
Infrared_Num;

extern uint16_t Infrared_Status;
extern uint16_t  Infrared_Pointer;
extern Infrared_Byte Infrared_ctrl;
extern uint16_t Infrared_ValidStatus;
extern int16_t Infrared_Cmd;


extern void Infrared_Receive_Start(void);

extern uint16_t Infrared_ReadVolt(uint16_t time);

extern uint16_t Infrared_ReadNum(void);

extern void Infrared_ByteStrCon(uint16_t pointer, uint16_t num);

extern void Infrared_Validation(void);

#endif
