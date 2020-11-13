/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SYS_KEY1_Pin GPIO_PIN_13
#define SYS_KEY1_GPIO_Port GPIOC
#define SYS_KEY2_Pin GPIO_PIN_14
#define SYS_KEY2_GPIO_Port GPIOC
#define TOUCH_KEY_Pin GPIO_PIN_15
#define TOUCH_KEY_GPIO_Port GPIOC
#define PM_RST_Pin GPIO_PIN_4
#define PM_RST_GPIO_Port GPIOA
#define TEST_PORT_Pin GPIO_PIN_12
#define TEST_PORT_GPIO_Port GPIOB
#define SYS_LED_Pin GPIO_PIN_8
#define SYS_LED_GPIO_Port GPIOA
#define CTRL485_Pin GPIO_PIN_11
#define CTRL485_GPIO_Port GPIOA
#define LCD_LED_Pin GPIO_PIN_12
#define LCD_LED_GPIO_Port GPIOA
#define LCD_CS_Pin GPIO_PIN_15
#define LCD_CS_GPIO_Port GPIOA
#define LCD_A0_Pin GPIO_PIN_6
#define LCD_A0_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_7
#define LCD_RST_GPIO_Port GPIOB
#define I2C1_SCL_Pin GPIO_PIN_8
#define I2C1_SCL_GPIO_Port GPIOB
#define I2C1_SDA_Pin GPIO_PIN_9
#define I2C1_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

#define LED_SYS_ON()     HAL_GPIO_WritePin(SYS_LED_GPIO_Port,SYS_LED_Pin,GPIO_PIN_RESET)
#define LED_SYS_OFF()    HAL_GPIO_WritePin(SYS_LED_GPIO_Port,SYS_LED_Pin,GPIO_PIN_SET)
#define LED_SYS_TOGGLE() HAL_GPIO_TogglePin(SYS_LED_GPIO_Port,SYS_LED_Pin)

#define TEST_PORT_ON()     HAL_GPIO_WritePin(TEST_PORT_GPIO_Port,TEST_PORT_Pin,GPIO_PIN_RESET)
#define TEST_PORT_OFF()    HAL_GPIO_WritePin(TEST_PORT_GPIO_Port,TEST_PORT_Pin,GPIO_PIN_SET)
#define TEST_PORT_TOGGLE() HAL_GPIO_TogglePin(TEST_PORT_GPIO_Port,TEST_PORT_Pin)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
