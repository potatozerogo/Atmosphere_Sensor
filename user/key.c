#include "key.h"
#include "main.h"	

uint8_t Key_State = 0;

uint8_t Read_Key(void)	
{
	uint8_t key_Value = 0;
	switch(Key_State) 
	{
		case 0:
		if(HAL_GPIO_ReadPin(TOUCH_KEY_GPIO_Port,TOUCH_KEY_Pin) == 0 ||
		HAL_GPIO_ReadPin(SYS_KEY1_GPIO_Port,SYS_KEY1_Pin) == 0 ||
		HAL_GPIO_ReadPin(SYS_KEY2_GPIO_Port,SYS_KEY2_Pin) == 0 )
			Key_State++;
		break;
		case 1:
		if(HAL_GPIO_ReadPin(TOUCH_KEY_GPIO_Port,TOUCH_KEY_Pin) == 0 ||
		HAL_GPIO_ReadPin(SYS_KEY1_GPIO_Port,SYS_KEY1_Pin) == 0 ||
		HAL_GPIO_ReadPin(SYS_KEY2_GPIO_Port,SYS_KEY2_Pin) == 0 )
		{
			Key_State++;
			if(HAL_GPIO_ReadPin(TOUCH_KEY_GPIO_Port,TOUCH_KEY_Pin) == 0)
				key_Value = 3;
			else
				if(HAL_GPIO_ReadPin(SYS_KEY1_GPIO_Port,SYS_KEY1_Pin) == 0)
					key_Value = 1;
				else
					if(HAL_GPIO_ReadPin(SYS_KEY2_GPIO_Port,SYS_KEY2_Pin) == 0)
						key_Value = 2;
		}
		else
		{
			Key_State = 0;
		}
		break;
		case 2:
		if(HAL_GPIO_ReadPin(TOUCH_KEY_GPIO_Port,TOUCH_KEY_Pin) != 0 &&
		HAL_GPIO_ReadPin(SYS_KEY1_GPIO_Port,SYS_KEY1_Pin) != 0 &&
		HAL_GPIO_ReadPin(SYS_KEY2_GPIO_Port,SYS_KEY2_Pin) != 0 )
		{
			Key_State = 0;
		}
		break;	
	}
	return (key_Value);
}	

