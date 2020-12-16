#include "main.h"

#include "Infrared.h"

/** Private Variables **/

uint16_t Infrared_Status = 0;

uint16_t Infrared_Pointer = 0;

uint16_t Infrared_Valid = 0;

Infrared_Byte Infrared_ctrl;

int16_t Infrared_Cmd = 0;

/** Infrared_Receive_Start()
  * 函数说明：红外通信引导码(1111 0000)验证，并刷新状态指示器
  */

void Infrared_Receive_Start()
{
	Infrared_Num TitleNum;
	
	TitleNum.v1 = Infrared_ReadVolt(0);
	
	TitleNum.v2 = Infrared_ReadVolt(2);
	
	TitleNum.v3 = Infrared_ReadVolt(2);
	
	TitleNum.v4 = Infrared_ReadVolt(2);
	
	if((TitleNum.v1 == 0 &&
			TitleNum.v3 == 0) ||
		(TitleNum.v2 == 0 &&
			TitleNum.v4 == 0))
	{
		Infrared_Status = 1;
	
		Infrared_Pointer = 0;
	
		Infrared_Valid = 0;
	}
	else
	{
		
	}
}

/** Infrared_ReadVolt(uint16_t time)
  * 函数说明：延时(time)毫秒后读取电平状态
  */

uint16_t Infrared_ReadVolt(uint16_t time)
{
	if(time > 0)
	{
		HAL_Delay(time);
	}
	GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_0);
	
	if (pinState == GPIO_PIN_RESET)
	{
		return 0;
	}
	else return 1;
}

/** Infrared_ReadNum()
  * 函数说明：根据电平状态读取逻辑值，并存入字节拼接数组
  */

uint16_t Infrared_ReadNum()
{
	Infrared_Pointer++;
	
	Infrared_Num NumSet;
	
	NumSet.v1 = Infrared_ReadVolt(0);
	
	NumSet.v2 = Infrared_ReadVolt(2);
	
	NumSet.num = NumSet.v1 + NumSet.v1 + NumSet.v2;
	
	if(NumSet.num == 0 || NumSet.num == 1)
	{
		Infrared_ByteStrCon(Infrared_Pointer, NumSet.num);
		return NumSet.num;
	}
	else
	{ 
		Infrared_ByteStrCon(Infrared_Pointer, 4);
		return 4;
	}
}

void Infrared_ByteStrCon(uint16_t pointer, uint16_t num)
{
	switch(pointer)
	{
		case 1:
			Infrared_ctrl.num1_normal = num;
		case 2:
			Infrared_ctrl.num2_normal = num;
		case 3:
			Infrared_ctrl.num3_normal = num;
		case 4:
			Infrared_ctrl.num1_inverse = num;
		case 5:
			Infrared_ctrl.num2_inverse = num;
		case 6:
			Infrared_ctrl.num3_inverse = num;
		default:
			break;
	}
}

void Infrared_Validation()
{
	if(	Infrared_ctrl.num1_normal != Infrared_ctrl.num1_inverse &&
			Infrared_ctrl.num2_normal != Infrared_ctrl.num2_inverse &&
			Infrared_ctrl.num3_normal != Infrared_ctrl.num3_inverse &&
			Infrared_ctrl.num1_normal != 4 &&
			Infrared_ctrl.num2_normal != 4 &&
			Infrared_ctrl.num3_normal != 4 &&
			Infrared_ctrl.num1_inverse != 4 &&
			Infrared_ctrl.num2_inverse != 4 &&
			Infrared_ctrl.num3_inverse != 4)
	{
		Infrared_Valid = 1;
		Infrared_Cmd = 
		4 * (int16_t)Infrared_ctrl.num1_normal +
		2 * (int16_t)Infrared_ctrl.num2_normal +
		1 * (int16_t)Infrared_ctrl.num3_normal;
	}
}
