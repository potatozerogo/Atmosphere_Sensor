#ifndef __ST7735_H__
#define __ST7735_H__

#include "stm32f1xx_hal.h"

#define	ROTATE_90

#ifdef ROTATE_90
#define Height 128
#define Width  160
#else
#define Height 160
#define Width  128
#endif
#define RED    0xf800
#define GREEN  0x07e0
#define BLUE   0x001f
#define YELLOW 0xffe0
#define WHITE  0xffff
#define BLACK  0x0000
#define PURPLE 0xf81f


#define			TFT_RESET_ACT				HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define			TFT_RESET_UNACT			HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)	 
#define			TFT_A0_CMD					HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_RESET)
#define			TFT_A0_DATA					HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, GPIO_PIN_SET)	 
#define			TFT_CSB_ENABLE			HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define			TFT_CSB_DISENABLE		HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)	
//#define			TFT_RESET_ACT				HAL_GPIO_WritePin(TFT_nRST_GPIO_Port, TFT_nRST_Pin, GPIO_PIN_RESET)
//#define			TFT_RESET_UNACT			HAL_GPIO_WritePin(TFT_nRST_GPIO_Port, TFT_nRST_Pin, GPIO_PIN_SET)	 
//#define			TFT_A0_CMD					HAL_GPIO_WritePin(TFT_A0_GPIO_Port, TFT_A0_Pin, GPIO_PIN_RESET)
//#define			TFT_A0_DATA					HAL_GPIO_WritePin(TFT_A0_GPIO_Port, TFT_A0_Pin, GPIO_PIN_SET)	 
//#define			TFT_CSB_ENABLE			HAL_GPIO_WritePin(TFT_nCS_GPIO_Port, TFT_nCS_Pin, GPIO_PIN_RESET)
//#define			TFT_CSB_DISENABLE		HAL_GPIO_WritePin(TFT_nCS_GPIO_Port, TFT_nCS_Pin, GPIO_PIN_SET)	 
/*
#define			TFT_SCK_SET					HAL_GPIO_WritePin(TFT_SCK_GPIO_Port, TFT_SCK_Pin, GPIO_PIN_SET)	 
#define			TFT_SCK_CLR					HAL_GPIO_WritePin(TFT_SCK_GPIO_Port, TFT_SCK_Pin, GPIO_PIN_RESET)
#define			TFT_SDA_SET					HAL_GPIO_WritePin(TFT_SDA_GPIO_Port, TFT_SDA_Pin, GPIO_PIN_SET)	 
#define			TFT_SDA_CLR					HAL_GPIO_WritePin(TFT_SDA_GPIO_Port, TFT_SDA_Pin, GPIO_PIN_RESET)
*/


void LCD_WriteCommand(uint8_t c);
void LCD_WriteData(uint8_t dat);
void LCD_Write_16BitData(uint16_t dat16);

void DISP_WINDOWS(void);

void ST7735_INITIAL(void);
void Frame(void);
void LCD_SetRegion(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end);
void LCD_Clear_Region(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end,uint16_t Color);
void Gui_DrawPoint(uint8_t x,uint8_t y,uint16_t Data);
void LCD_Clear(uint16_t Color);
void LCD_SetXY(uint8_t x,uint8_t y);


#endif /* __ST7735_H__ */
