#include "st7735.h"
#include "display_number.h"

/************************************
* @brief ST7735���Ƶ�ɫͼ��
* @param Start_X	��ʼX����
* @param Start_Y	��ʼY����
* @param Size_X	ͼ��X�����С
* @param Size_Y	ͼ��Y�����С
* @param *address	ͼ����ŵ�ַ
* @param color	ǰ��ɫ
* @param bgcolor	����ɫ
* @retval �޷���
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
* @brief ST7735���ƴ�����
* @param X	��ʼX����
* @param Y	��ʼY����
* @param number	��ʾ������ֵ
* @param color	ǰ��ɫ
* @param bgcolor	����ɫ
* @retval �޷���
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
* @brief ST7735����4λ������
* @param X	��ʼX����
* @param Y	��ʼY����
* @param number	��ʾ����ֵ
* @param color	ǰ��ɫ
* @param bgcolor	����ɫ
* @retval �޷���
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
* @brief ST7735����3λ�����֣���С����͸��ţ���������ʾ�¶Ⱥ�ʪ�ȡ�
* @param X	��ʼX����
* @param Y	��ʼY����
* @param number	��ʾ����ֵ
* @param color	ǰ��ɫ
* @param bgcolor	����ɫ
* @retval �޷���
************************************/
void ST7735_Put_3Bits_Big_Percent_Number(uint8_t X, uint8_t Y, int16_t data, uint16_t color, uint16_t bgcolor)
{
	uint8_t i = 0;
	
	/*����λ*/
	if(data < 0)
	{
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, 11, color, bgcolor);//��ʾ����
		data = - data;//����ת����������洦��
	}
	else
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, 10, color, bgcolor);
	
	/*ʮλ*/
	if(data/100 == 0)
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, 10, color, bgcolor);
	else
		ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, data/100, color, bgcolor);
	
	/*��λ*/
	ST7735_PutChar_Big_Number(X + i++ *FONT_SIZE_X, Y, data%100/10, color, bgcolor);
	
	/*С����*/
	ST7735_Draw_Monochromatic_Graph(X + i++ *FONT_SIZE_X, Y, 16, 36, (unsigned char*)Point_Graph, color, bgcolor);	//С����
	
	/*0.1λ*/
	ST7735_PutChar_Big_Number(X + i *FONT_SIZE_X - 16, Y, data%10, color, bgcolor);	
}


