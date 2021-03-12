#ifndef __PM25_H
#define __PM25_H

#include "stm32f1xx_hal.h"
#include "user_config.h"


#ifdef __ATMOSPHERE_SENSOR_7IN1__
#define PM25_UART_DEV huart2 //7合1使用huart2,气体传感器使用huart2
#endif

#ifdef __GAS_SENSOR__
#define PM25_UART_DEV huart2 //7合1使用huart2气体传感器使用huart2
#define PM25_RX_CPLHandle GAS_SENSOR_RX_CPLHandle//气体定义为GAS_SENSOR_RX_CPLHandle
#endif

int8_t Pm25_Set_Read_Mod(void);
int8_t Init_Pm25(void);
int8_t Get_Pm25(uint8_t *buff);
int8_t Standby_Pm25(void);
int8_t Work_Pm25(void);
int8_t Read_Pm25(void);

#endif //#endif __PM25_H
