#include "main.h"
#include "stm32f1xx_hal.h"
#include "st7735.h"

//========================================================
extern SPI_HandleTypeDef hspi1;
//========================================================

//========================================================
void HOLD_DISP ()
{
//   while(!Step);
	HAL_Delay(1000);
}
//========================================================
//========================================================
#if 0
void SPI_WriteData(uint8_t dat)
{
uint8_t i;
	TFT_CSB_ENABLE;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)
			TFT_SDA_SET;
		else
			TFT_SDA_CLR;
		TFT_SCK_CLR;
		TFT_SCK_SET;
		dat <<= 1;	
	}
	TFT_CSB_DISENABLE;		
}
#else
void SPI_WriteData(uint8_t dat)
{
//uint8_t i;
	TFT_CSB_ENABLE;
	HAL_SPI_Transmit(&hspi1, &dat, 1, 1);
	TFT_CSB_DISENABLE;		
}

#endif

void  LCD_WriteCommand(uint8_t c)
{
	TFT_A0_CMD;
	SPI_WriteData(c);
}

//========================================================
void LCD_WriteData(uint8_t dat)
{
	TFT_A0_DATA;
	SPI_WriteData(dat);
}
//========================================================
void LCD_Write_16BitData(uint16_t dat16)
{
	LCD_WriteData(dat16>>8);
	LCD_WriteData(dat16&0x00ff);	
}
//========================================================


void DISPLAY_RGB(void)
{
uint8_t i,j,k;
	for (i=Height/3;i>0;i--)
	{
		for (j=Width;j>0;j--)
			LCD_Write_16BitData(RED);
	}
  for (i=Height/3;i>0;i--)
	{
		for (j=Width;j>0;j--)
		{
			LCD_Write_16BitData(GREEN);
		}
	}
  for (k=(Height-Height/3-Height/3);k>0;k--)
	{
		for (j=Width;j>0;j--)
		{
			LCD_Write_16BitData(BLUE);
		}
	}
	HOLD_DISP ();
}
//========================================================
void Frame(void)
{
uint8_t	i,j,k;
	DISP_WINDOWS();
	for (i=Width;i>0;i--)
	{
		LCD_Write_16BitData(WHITE);
	}
	for (j=Height-2;j>0;j--)
	{
		LCD_Write_16BitData(WHITE);
		for (k=Width-2;k>0;k--)
		{
			LCD_Write_16BitData(BLACK);
		}
		LCD_Write_16BitData(WHITE);
	}
	for (i=Width;i>0;i--)
	{
		LCD_Write_16BitData(WHITE);
	}
	HOLD_DISP ();
}
//========================================================
void DISP_WINDOWS(void)
{
	LCD_WriteCommand(0x2A);
	LCD_WriteData(0x00);
//	LCD_WriteData(0x02);
	LCD_WriteData(0x01);
	LCD_WriteData(0x00);
	LCD_WriteData(Width);

	LCD_WriteCommand(0x2B);
	LCD_WriteData(0x00);
	LCD_WriteData(0x01);
	LCD_WriteData(0x00);
	LCD_WriteData(Height);
	LCD_WriteCommand(0x2C);
}

//========================================================

void ST7735_INITIAL(void)
{ 

	//------------------------------------ST7735R Reset Sequence-----------------------------------------// 
	TFT_RESET_UNACT; 
	HAL_Delay(1);                                                             	//Delay 1ms 
	TFT_RESET_ACT; 
	HAL_Delay(1);                                                               		//Delay 1ms 
	TFT_RESET_UNACT; 
	HAL_Delay(120);                                                             		 //Delay 120ms 
	//--------------------------------End ST7735R Reset Sequence --------------------------------------// 

	//--------------------------------End ST7735S Reset Sequence --------------------------------------// 
	LCD_WriteCommand(0x11); 			//Sleep out 
	HAL_Delay(120);              //Delay 120ms 

	//------------------------------------ST7735S Frame Rate-----------------------------------------// 
	LCD_WriteCommand(0xB1); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteCommand(0xB2); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteCommand(0xB3); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x05); 
	LCD_WriteData(0x3C); 
	LCD_WriteData(0x3C); 
	//------------------------------------End ST7735S Frame Rate---------------------------------// 
	LCD_WriteCommand(0xB4); //Dot inversion 
	LCD_WriteData(0x03); 
	//------------------------------------ST7735S Power Sequence---------------------------------// 
	LCD_WriteCommand(0xC0); 
	LCD_WriteData(0x28); 
	LCD_WriteData(0x08); 
	LCD_WriteData(0x04); 
	LCD_WriteCommand(0xC1); 
	LCD_WriteData(0XC0); 
	LCD_WriteCommand(0xC2); 
	LCD_WriteData(0x0D); 
	LCD_WriteData(0x00); 
	LCD_WriteCommand(0xC3); 
	LCD_WriteData(0x8D); 
	LCD_WriteData(0x2A); 
	LCD_WriteCommand(0xC4); 
	LCD_WriteData(0x8D); 
	LCD_WriteData(0xEE); 
	//---------------------------------End ST7735S Power Sequence-------------------------------------// 
	LCD_WriteCommand(0xC5); //VCOM 
	LCD_WriteData(0x1A); 
	LCD_WriteCommand(0x36); //MX, MY, RGB mode //Just this line(C0/00/A0/60����C8/68/A8/08)
#ifndef ROTATE_90
	LCD_WriteData(0xC0);	// ���򣺿�128����160���ӿ����¡���C0��C8������)
//	LCD_WriteData(0x00); 		// ����180��
#else
//	LCD_WriteData(0xA0); 		// ����90�㣬�ӿ�����	,��ʾ�������⣨ԭ��ֵ����
	LCD_WriteData(0x60); 		// ����90�㣬�ӿ�����	,��ʾ�������⣨ԭ��ֵ����		
#endif	
	//------------------------------------ST7735S Gamma Sequence---------------------------------// 
	LCD_WriteCommand(0xE0); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x22); 
	LCD_WriteData(0x07); 
	LCD_WriteData(0x0A); 
	LCD_WriteData(0x2E); 
	LCD_WriteData(0x30); 
	LCD_WriteData(0x25); 
	LCD_WriteData(0x2A); 
	LCD_WriteData(0x28); 
	LCD_WriteData(0x26); 
	LCD_WriteData(0x2E); 
	LCD_WriteData(0x3A); 
	LCD_WriteData(0x00); 
	LCD_WriteData(0x01); 
	LCD_WriteData(0x03); 
	LCD_WriteData(0x13); 
	LCD_WriteCommand(0xE1); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x16); 
	LCD_WriteData(0x06); 
	LCD_WriteData(0x0D); 
	LCD_WriteData(0x2D); 
	LCD_WriteData(0x26); 
	LCD_WriteData(0x23); 
	LCD_WriteData(0x27); 
	LCD_WriteData(0x27); 
	LCD_WriteData(0x25); 
	LCD_WriteData(0x2D); 
	LCD_WriteData(0x3B); 
	LCD_WriteData(0x00); 
	LCD_WriteData(0x01); 
	LCD_WriteData(0x04); 
	LCD_WriteData(0x13); 
	//------------------------------------End ST7735S Gamma Sequence-----------------------------// 
	LCD_WriteCommand(0x3A); //65k mode 
	LCD_WriteData(0x05); 
	LCD_WriteCommand(0x29); //Display on 

} 

/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void LCD_SetRegion(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end)
{		
	LCD_WriteCommand(0x2a);
	LCD_WriteData(0x00);
	LCD_WriteData(x_start+1);
	LCD_WriteData(0x00);
	LCD_WriteData(x_end+1);

	LCD_WriteCommand(0x2b);
	LCD_WriteData(0x00);
	LCD_WriteData(y_start+2);
	LCD_WriteData(0x00);
	LCD_WriteData(y_end+2);	
	LCD_WriteCommand(0x2c);

}

/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void LCD_SetXY(uint8_t x,uint8_t y)
{
  	LCD_SetRegion(x,y,x,y);
}

	
/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/
void Gui_DrawPoint(uint8_t x,uint8_t y,uint16_t Data)
{
	LCD_SetRegion(x,y,x+1,y+1);
	LCD_Write_16BitData(Data);
}    

/*****************************************
 �������ܣ���TFTĳһ�����ɫ                          
 ���ڲ�����color  ����ɫֵ                                 
******************************************/
uint16_t LCD_ReadPoint(uint8_t x,uint8_t y)
{
uint16_t Data;
  LCD_SetXY(x,y);

  //LCD_ReadData();//���������ֽ�
  //Data=LCD_ReadData();
  LCD_WriteData(Data);
  return Data;
}
/*************************************************
��������LCD_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/
void LCD_Clear(uint16_t Color)               
{	
uint8_t i,m;
	LCD_SetRegion(0,0,Width-1,Height-1);
	LCD_WriteCommand(0x2C);
	for(i=0;i<Width;i++)
	{
    for(m=0;m<Height;m++)
	  	LCD_Write_16BitData(Color);
	}   
}

void LCD_Clear_Region(uint8_t x_start,uint8_t y_start,uint8_t x_end,uint8_t y_end,uint16_t Color) 
{
uint8_t i,m;
	LCD_SetRegion(x_start,y_start,x_end,y_end);	
	LCD_WriteCommand(0x2C);
	for(i=0;i<x_end-x_start;i++)
	{
    for(m=0;m<y_end-y_start;m++)
	  	LCD_Write_16BitData(Color);
	} 	
}

