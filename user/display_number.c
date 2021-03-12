#include "st7735.h"
#include "display_number.h"

/************************************
* @brief ST7735绘制单色图案
* @param Start_X	开始X坐标
* @param Start_Y	开始Y坐标
* @param Size_X	图案X方向大小
* @param Size_Y	图案Y方向大小
* @param *address	图案存放地址
* @param color	前景色
* @param bgcolor	背景色
* @retval 无返回
************************************/
void ST7735_Draw_Monochromatic_Graph(uint16_t Start_X, uint16_t Start_Y, uint16_t Size_X, uint16_t Size_Y, unsigned char *address, uint16_t color, uint16_t bgcolor)
{
  uint16_t i,j;
  uint8_t buffer;
  uint8_t CH = color >> 8;
  uint8_t CL = (uint8_t)color;
  uint8_t BCH = bgcolor >> 8;
  uint8_t BCL = (uint8_t)bgcolor;

	CS_L();

	ST7735_AddrSet(Start_X, Start_Y, Start_X + Size_X -1 , Start_Y + Size_Y -1);
	A0_H();
	for (j = 0; j < Size_X*Size_Y/8; j++)
	{
	  buffer = *address++;
	  for (i = 0; i < 8; i++)
	  {
		if ((buffer << i) & 0x80)
		{
		  ST7735_write(CH);
		  ST7735_write(CL);
		}
		else
		{
		  ST7735_write(BCH);
		  ST7735_write(BCL);
		}
	  }
	}
	
	CS_H();
}


/************************************
* @brief ST7735绘制大数字
* @param X	开始X坐标
* @param Y	开始Y坐标
* @param number	显示的数字值
* @param color	前景色
* @param bgcolor	背景色
* @retval 无返回
************************************/
void ST7735_PutChar_Big_Number(uint16_t X, uint16_t Y, uint8_t number, uint16_t color, uint16_t bgcolor)
{
  uint16_t i,j;
  uint8_t buffer;
  uint8_t CH = color >> 8;
  uint8_t CL = (uint8_t)color;
  uint8_t BCH = bgcolor >> 8;
  uint8_t BCL = (uint8_t)bgcolor;

	CS_L();

	ST7735_AddrSet(X, Y, X + FONT_SIZE_X -1 , Y + FONT_SIZE_Y -1);
	A0_H();
	for (j = 0; j < FONT_ALL_PIXELS/8; j++)
	{
	  buffer = FONT_TABLE[number][j];
	  for (i = 0; i < 8; i++)
	  {
		if ((buffer << i) & 0x80)
		{
		  ST7735_write(CH);
		  ST7735_write(CL);
		}
		else
		{
		  ST7735_write(BCH);
		  ST7735_write(BCL);
		}
	  }
	}

	CS_H();
}

/************************************
* @brief ST7735绘制4位大数字
* @param X	开始X坐标
* @param Y	开始Y坐标
* @param number	显示的数值
* @param color	前景色
* @param bgcolor	背景色
* @retval 无返回
************************************/
void ST7735_Put_4Bits_Big_Number(uint8_t X, uint8_t Y, uint16_t data, uint16_t color, uint16_t bgcolor)
{
	uint8_t i = 0;
	
	if(data/1000 == 0)
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, 10, color, bgcolor);
	else
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, data/1000, color, bgcolor);
	
	if(data/100 == 0)
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, 10, color, bgcolor);
	else
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, data%1000/100, color, bgcolor);
	
	if(data/10 == 0)
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, 10, color, bgcolor);
	else
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, data%100/10, color, bgcolor);
	
	ST7735_PutChar_Big_Number(X+i*FONT_SIZE_X, Y, data%10, color, bgcolor);
}

/************************************
* @brief ST7735绘制3位大数字（带小数点和负号），用于显示温度和湿度。
* @param X	开始X坐标
* @param Y	开始Y坐标
* @param number	显示的数值
* @param color	前景色
* @param bgcolor	背景色
* @retval 无返回
************************************/
void ST7735_Put_3Bits_Big_Percent_Number(uint8_t X, uint8_t Y, int16_t data, uint16_t color, uint16_t bgcolor)
{
	uint8_t i = 0;
	
	/*符号位*/
	if(data < 0)
	{
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, 11, color, bgcolor);//显示负号
		data = - data;//将负转正，方面后面处理
	}
	else
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, 10, color, bgcolor);
	
	/*十位*/
	if(data/100 == 0)
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, 10, color, bgcolor);
	else
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, data/100, color, bgcolor);
	
	/*个位*/
	ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, data%100/10, color, bgcolor);
	
	/*小数点*/
	ST7735_Draw_Monochromatic_Graph(X + i++ *FONT_SIZE_X, Y, 16, 36, (unsigned char*)Point_Graph, color, bgcolor);	//小数点
	
	/*0.1位*/
	ST7735_PutChar_Big_Number(X + i *FONT_SIZE_X - 16, Y, data%10, color, bgcolor);	
}


