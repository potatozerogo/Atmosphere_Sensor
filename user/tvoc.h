#ifndef __TVOC_H
#define __TVOC_H

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "cmsis_os.h"
#include "user_config.h"

#ifdef __ATMOSPHERE_SENSOR_7IN1__
#define TVOC_UART_DEV huart3 //7合1使用huart3,气体传感器使用huart2
#define TVOC_RX_CPLHandle VOC_RX_CPLHandle//7合1定义为VOC_RX_CPLHandle
#endif

#ifdef __GAS_SENSOR__
#define TVOC_UART_DEV huart2 //7合1使用huart3,气体传感器使用huart2
#define TVOC_RX_CPLHandle GAS_SENSOR_RX_CPLHandle//气体定义为GAS_SENSOR_RX_CPLHandle
#endif

int8_t Tvoc_Set_Read_Mod(void);
int8_t Init_Tvoc(void);
int8_t Get_Tvoc(uint8_t *buff);

#endif //#endif __VOC_H
