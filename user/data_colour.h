#ifndef __DATA_CLOUR_H__
#define __DATA_CLOUR_H__

#include "color565.h"
#include "stm32f1xx_hal.h"

typedef struct
{
	int16_t Data_Low;
	int16_t Data_Up;
	uint16_t Colour;
}_DATA_TO_COLOUR;

typedef struct
{
	_DATA_TO_COLOUR Class_Nice;
	_DATA_TO_COLOUR Class_Good;
	_DATA_TO_COLOUR Class_Medium;
	_DATA_TO_COLOUR Class_Bad;
	_DATA_TO_COLOUR Class_Danger;
}_AQI_COLOUR_TABLE;

typedef struct
{
	_DATA_TO_COLOUR Class_Frozen;
	_DATA_TO_COLOUR Class_Cold;
	_DATA_TO_COLOUR Class_Cool;
	_DATA_TO_COLOUR Class_Warm;
	_DATA_TO_COLOUR Class_Hot;
}_TEMPERATURE_COLOUR_TABLE;

uint16_t Set_Aqi_Colour(uint16_t data, _AQI_COLOUR_TABLE *type);
uint16_t Set_Temperature_Colour(int16_t data, _TEMPERATURE_COLOUR_TABLE *type);

#endif
