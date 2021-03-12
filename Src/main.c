/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "st7735.h"
#include "modbus.h"
#include "tvoc.h"
#include "pm25.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t Temperature_Data;
uint16_t Humidity_Data;
uint16_t Pm25_Data;
uint16_t Pm10_Data;
uint16_t Eco2_Data;
uint16_t Tvoc_Data;
uint16_t Ch2o_Data;

unsigned int Display_Sensor_Type = 0;//显示传感器类型
char *String_Display_Sensor_Type[8] = {"Temperature"," Humidity  ","    PM25   ","    PM10   ","    eCO2   ","    TVOC   ","    HCHO   ","    All"};
char Lcd_Display_Buff[8];

//===========外部变量_START================
extern osSemaphoreId VOC_RX_CPLHandle;
extern osSemaphoreId PM25_RX_CPLHandle;

extern char Lcd_Display_Buff[8];

extern uint8_t Tvoc_Data_Buff[6];
extern uint8_t Pm25_Data_Buff[4];
extern int16_t Actual_Temperature_Data; 
extern int16_t Actual_Humidity_Data;

extern uint16_t * Modbus_Memory_Address;//modbus数据存储空间首地址modbus.c
//===========外部变量_END================

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//串口接收完成回调
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//======Uart1_START========	
	if(huart->Instance == USART1)
	{
		Modbus_Uart_Rx_Int_Process();
	}
//======Uart1_END========	

//======Uart2_START========	
	if(huart->Instance == USART2)
	{
		osSemaphoreRelease(PM25_RX_CPLHandle);
	}
//======Uart2_END========	

//======Uart3_START========	
	if(huart->Instance == USART3)
	{
		osSemaphoreRelease(VOC_RX_CPLHandle);
	}	
//======Uart3_END========
}

//串口发送完成回调
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)
	{
		Modbus_Uart_Tx_Complete_Process();	
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
//	HAL_UART_Transmit(&huart2,String_Statr,strlen((char *)String_Statr),10000);
//	HAL_UART_Receive_IT(&huart2,Rx_uart2_buff,4);

	Eco2_Data = 0;
	Tvoc_Data = 0;
	Ch2o_Data = 0;
	Temperature_Data = 0;
	Humidity_Data = 0;
	Pm25_Data = 0;
	Pm10_Data = 0;
	
	Modbus_Memory_Address = &Temperature_Data;
	Init_Modbus();
	HAL_TIM_Base_Start_IT(&htim4);//启动定时器4

	ST7735_Init();
	ST7735_Clear(COLOR565_BLACK);
	ST7735_Clear(COLOR565_BLACK);
	ST7735_Rect(0,0,159,127,COLOR565_RED);
	ST7735_Rect(1,1,158,126,COLOR565_RED);
	ST7735_HLine(1, 158, 26,COLOR565_RED);
	
//	ST7735_Rect(0,0,159,127,COLOR565_RED);
//	ST7735_Rect(1,1,158,126,COLOR565_RED);
//	ST7735_HLine(1, 158, 26,COLOR565_RED);
//	ST7735_HLine(1, 158, 51,COLOR565_RED);
//	ST7735_HLine(1, 158, 76,COLOR565_RED);
//	ST7735_HLine(1, 158, 101,COLOR565_RED);
//	ST7735_VLine(79, 76, 126,COLOR565_RED);

//	ST7735_PutStr7x11(5,  8, "Temperature:", COLOR565_RED,COLOR565_BLACK);
//	ST7735_PutStr7x11(5,  33, "Humidity:", COLOR565_RED,COLOR565_BLACK);
//	ST7735_PutStr7x11(5,  58, "eCO2:", COLOR565_RED,COLOR565_BLACK);
//	ST7735_PutStr7x11(5,  83, "TVOC:", COLOR565_RED,COLOR565_BLACK);
//	ST7735_PutStr7x11(82,  83, "CH2O:", COLOR565_RED,COLOR565_BLACK);
//	ST7735_PutStr7x11(5,  108, "PM2.5:", COLOR565_RED,COLOR565_BLACK);
//	ST7735_PutStr7x11(82,  108, "PM10:", COLOR565_RED,COLOR565_BLACK);
	
	LCD_LED_ON();
	
	HAL_Delay(5000);
	Init_Tvoc();
	Init_Pm25();
	HAL_Delay(500);
	
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init(); 
  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	LED_SYS_OFF();	  
	if(Get_Tvoc(Tvoc_Data_Buff) >= 0)
	{
		Eco2_Data = Tvoc_Data_Buff[0];
		Eco2_Data = Eco2_Data << 8;
		Eco2_Data += Tvoc_Data_Buff[1];

		Tvoc_Data = Tvoc_Data_Buff[2];
		Tvoc_Data = Tvoc_Data << 8;
		Tvoc_Data += Tvoc_Data_Buff[3];	

		Ch2o_Data = Tvoc_Data_Buff[4];
		Ch2o_Data = Ch2o_Data << 8;
		Ch2o_Data += Tvoc_Data_Buff[5];
	}
	else
	{
		Eco2_Data = 9999;
		Tvoc_Data = 9999;
		Ch2o_Data = 9999;
	}

//  HAL_Delay(1000);
	LED_SYS_ON();
	  
	if(Get_Pm25(Pm25_Data_Buff) >= 0 )
	{
		Pm25_Data = Pm25_Data_Buff[0];
		Pm25_Data = Pm25_Data << 8;
		Pm25_Data += Pm25_Data_Buff[1];
		  
		Pm10_Data = Pm25_Data_Buff[2];
		Pm10_Data = Pm10_Data << 8;
		Pm10_Data += Pm25_Data_Buff[3];	
	}
	else
	{
		Pm25_Data = 9999;
		Pm10_Data = 9999;
	}
	
    HAL_Delay(1000);
	
	LED_SYS_ON();
	
	sprintf((char *)Lcd_Display_Buff,"%.1f C",(float)Actual_Temperature_Data/10); 
	ST7735_PutStr7x11(104,  8,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	sprintf((char *)Lcd_Display_Buff,"%.1f %%",(float)Actual_Humidity_Data/10);  
	ST7735_PutStr7x11(88,  33,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	  
	sprintf((char *)Lcd_Display_Buff,"%d ppm",Eco2_Data);
	ST7735_PutStr7x11(48,  58,"        ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	ST7735_PutStr7x11(48,  58,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	sprintf((char *)Lcd_Display_Buff,"%d",Tvoc_Data);
	ST7735_PutStr7x11(47,  83,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	ST7735_PutStr7x11(47,  83,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	sprintf((char *)Lcd_Display_Buff,"%d",Ch2o_Data);
	ST7735_PutStr7x11(124,  83,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	ST7735_PutStr7x11(124,  83,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	  
	sprintf((char *)Lcd_Display_Buff,"%d",Pm25_Data);
	ST7735_PutStr7x11(47,  108,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	ST7735_PutStr7x11(47,  108,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	sprintf((char *)Lcd_Display_Buff,"%d",Pm10_Data);
	ST7735_PutStr7x11(117,  108,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	ST7735_PutStr7x11(117,  108,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
    HAL_Delay(1000);
	  
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  
	//定时器4溢出中断回调
	if(htim->Instance == TIM4)
	{
		Modbus_Timer_Process();
	}
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
