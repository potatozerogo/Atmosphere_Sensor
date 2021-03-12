#include "data_colour.h"

const _AQI_COLOUR_TABLE Co2_Colour_Table = {{0,500,COLOR565_SPRING_GREEN},
											{500,800,COLOR565_GREEN},
											{800,1200,COLOR565_YELLOW},
											{1200,2000,COLOR565_RED},
											{2000,5000,COLOR565_VIOLET}};

const _AQI_COLOUR_TABLE Co_Colour_Table =  {{0,10,COLOR565_SPRING_GREEN},
											{10,35,COLOR565_GREEN},
											{35,200,COLOR565_YELLOW},
											{200,400,COLOR565_RED},
											{400,1000,COLOR565_VIOLET}};

const _AQI_COLOUR_TABLE Hcho_Colour_Table =  {{0,10,COLOR565_SPRING_GREEN},
											{10,35,COLOR565_GREEN},
											{35,200,COLOR565_YELLOW},
											{200,400,COLOR565_RED},
											{400,1000,COLOR565_VIOLET}};

const _AQI_COLOUR_TABLE Tvoc_Colour_Table =  {{0,50,COLOR565_SPRING_GREEN},
											{50,100,COLOR565_GREEN},
											{100,200,COLOR565_YELLOW},
											{200,500,COLOR565_RED},
											{500,1000,COLOR565_VIOLET}};

const _AQI_COLOUR_TABLE Pm25_Colour_Table =  {{0,35,COLOR565_SPRING_GREEN},
											{35,75,COLOR565_GREEN},
											{75,115,COLOR565_YELLOW},
											{115,150,COLOR565_RED},
											{150,500,COLOR565_VIOLET}};

const _AQI_COLOUR_TABLE Pm10_Colour_Table =  {{0,50,COLOR565_SPRING_GREEN},
											{50,150,COLOR565_GREEN},
											{150,250,COLOR565_YELLOW},
											{250,350,COLOR565_RED},
											{350,500,COLOR565_VIOLET}};

const _TEMPERATURE_COLOUR_TABLE Temperature_Colour_Table =  {{-600,0,COLOR565_VIOLET},
															{0,100,COLOR565_BLUE},
															{100,180,COLOR565_GREEN},
															{180,280,COLOR565_ORANGE},
															{280,1000,COLOR565_RED}};

const _TEMPERATURE_COLOUR_TABLE Humidity_Colour_Table =  {{0,100,COLOR565_RED},
														{100,200,COLOR565_ORANGE},
														{200,300,COLOR565_GREEN},
														{300,400,COLOR565_BLUE},
														{400,1000,COLOR565_VIOLET}};											

/************************************
* @brief 查表得出显示数据的颜色（温湿度）
* @param data	输入的数值
* @param *type	对应的样色等级表
* @retval 颜色代码
************************************/										
uint16_t Set_Temperature_Colour(int16_t data, _TEMPERATURE_COLOUR_TABLE *type)
{
	if(data >= type->Class_Frozen.Data_Low && data <= type->Class_Frozen.Data_Up)
		return type->Class_Frozen.Colour;
	if(data >= type->Class_Cold.Data_Low && data <= type->Class_Cold.Data_Up)
		return type->Class_Cold.Colour;
	if(data >= type->Class_Cool.Data_Low && data <= type->Class_Cool.Data_Up)
		return type->Class_Cool.Colour;
	if(data >= type->Class_Warm.Data_Low && data <= type->Class_Warm.Data_Up)
		return type->Class_Warm.Colour;
	if(data >= type->Class_Hot.Data_Low && data <= type->Class_Hot.Data_Up)
		return type->Class_Hot.Colour;
	
	return 0;
}

/************************************
* @brief 查表得出显示数据的颜色(空气质量)
* @param data	输入的数值
* @param *type	对应的样色等级表
* @retval 颜色代码
************************************/										
uint16_t Set_Aqi_Colour(uint16_t data, _AQI_COLOUR_TABLE *type)
{
	if(data >= type->Class_Nice.Data_Low && data <= type->Class_Nice.Data_Up)
		return type->Class_Nice.Colour;
	if(data >= type->Class_Good.Data_Low && data <= type->Class_Good.Data_Up)
		return type->Class_Good.Colour;
	if(data >= type->Class_Medium.Data_Low && data <= type->Class_Medium.Data_Up)
		return type->Class_Medium.Colour;
	if(data >= type->Class_Bad.Data_Low && data <= type->Class_Bad.Data_Up)
		return type->Class_Bad.Colour;
	if(data >= type->Class_Danger.Data_Low && data <= type->Class_Danger.Data_Up)
		return type->Class_Danger.Colour;
	
	return 0;
}
	
