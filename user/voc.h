#ifndef __VOC_H
#define __VOC_H

#include "stm32f1xx_hal.h"

int8_t Voc_Set_Read_Mod(void);
int8_t Init_Voc(void);
int8_t Get_Voc(uint8_t *buff);

#endif //#endif __VOC_H
