#ifndef __DISPLAY_NUMBER_H__
#define __DISPLAY_NUMBER_H__

#include "stm32f1xx_hal.h"
#include "st7735.h"
#include "font32x36.h"

#define FONT_SIZE_X 32 //字模宽度
#define FONT_SIZE_Y 36 //字模高度
#define FONT_TABLE Font32x36 //字模表
#define FONT_ALL_PIXELS FONT_SIZE_X*FONT_SIZE_Y //字模总像素

void ST7735_Draw_Monochromatic_Graph(uint16_t Start_X, uint16_t Start_Y, uint16_t Size_X, uint16_t Size_Y, unsigned char *address, uint16_t color, uint16_t bgcolor);
void ST7735_PutChar_Big_Number(uint16_t X, uint16_t Y, uint8_t number, uint16_t color, uint16_t bgcolor);
void ST7735_Put_4Bits_Big_Number(uint8_t X, uint8_t Y, uint16_t data, uint16_t color, uint16_t bgcolor);
void ST7735_Put_3Bits_Big_Percent_Number(uint8_t X, uint8_t Y, int16_t data, uint16_t color, uint16_t bgcolor);

#endif

