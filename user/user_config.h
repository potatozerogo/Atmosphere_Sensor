#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H

#define __ATMOSPHERE_SENSOR_7IN1__

#include "stm32f1xx_hal.h"


enum _SENSOR_TYPE
{
	_TEMPERATURE,
	_HUMIDITY,
	_PM25,
	_PM10,
	_ECO2,
	_TVOC,
	_CH2O,
//	_ALL_SENSOR,
	_END_OF_DISPALY_TYPE
};

#endif //#endif __USER_CONFIG_H
