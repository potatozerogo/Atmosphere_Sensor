#ifndef __PM25_H
#define __PM25_H

#include "stm32f1xx_hal.h"

int8_t PM25_Set_Read_Mod(void);
int8_t Init_PM25(void);
int8_t Get_PM25(uint8_t *buff);
int8_t Standby_PM25(void);
int8_t Work_PM25(void);
int8_t Read_PM25(void);

#endif //#endif __PM25_H
