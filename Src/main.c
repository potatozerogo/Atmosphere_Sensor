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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "st7735.h"
#include "voc.h"
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

uint16_t ECO2_Data;
uint16_t TVOC_Data;
uint16_t CH2O_Data;
uint16_t Temperature_Data;
uint16_t Humidity_Data;
uint16_t PM25_Data;
uint16_t PM10_Data;

//===========UART_START===============
//uint8_t  New_Receive_Start_Flag = 0;//新一帧的标志
//uint8_t  Rx_uart3_buff[32];//接收缓存
//uint8_t  Receive_Last_Index = 0;//用来转递新一帧第一字节数据，它会被记录在上一帧结尾再后面的字节里。
//uint8_t  Rx_uart3_buff_index = 0;//接收数据指针
//uint8_t  Rx_uart3_Timer = 0;//接收数据帧结束延时,用于判读一帧数据结束，单位100us
//===========UART_END=================

//===========外部变量_START================
extern osSemaphoreId VOC_RX_CPLHandle;
extern osSemaphoreId PM25_RX_CPLHandle;
//===========外部变量_END================

extern char Lcd_Display_Buff[8];
extern uint8_t Voc_Data_Buff[6];
extern uint8_t PM25_Data_Buff[4];
extern uint16_t ECO2_Data;
extern uint16_t TVOC_Data;
extern uint16_t CH2O_Data;
extern uint16_t PM25_Data;
extern uint16_t PM10_Data;
extern int16_t Actual_Temperature_Data; 
extern int16_t Actual_Humidity_Data;


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
//	if(huart->Instance == USART1)
//	{
//		
//		if(New_Receive_Start_Flag == 1)
//		{
//			New_Receive_Start_Flag = 0;
//			Rx_uart1_buff[0] = Rx_uart1_buff[Receive_Last_Index];
//			Rx_uart1_buff_index = 0;
//		}
//		Rx_uart1_buff_index++;
//		Receive_Last_Index = Rx_uart1_buff_index;
//		HAL_UART_Receive_IT(&huart1,&Rx_uart1_buff[Rx_uart1_buff_index],1);
//		Rx_uart1_Timer = 0;
//	}
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
  /* USER CODE BEGIN 2 */
//	HAL_UART_Transmit(&huart2,String_Statr,strlen((char *)String_Statr),10000);
//	HAL_UART_Receive_IT(&huart2,Rx_uart2_buff,4);

	ECO2_Data = 0;
	TVOC_Data = 0;
	CH2O_Data = 0;
	Temperature_Data = 0;
	Humidity_Data = 0;
	PM25_Data = 0;
	PM10_Data = 0;

	ST7735_Init();
	ST7735_Clear(COLOR565_BLACK);
	ST7735_Rect(0,0,159,127,COLOR565_RED);
	ST7735_Rect(1,1,158,126,COLOR565_RED);
	ST7735_HLine(1, 158, 26,COLOR565_RED);
	ST7735_HLine(1, 158, 51,COLOR565_RED);
	ST7735_HLine(1, 158, 76,COLOR565_RED);
	ST7735_HLine(1, 158, 101,COLOR565_RED);
	ST7735_VLine(79, 76, 126,COLOR565_RED);

	ST7735_PutStr7x11(5,  8, "Temperature:", COLOR565_RED,COLOR565_BLACK);
	ST7735_PutStr7x11(5,  33, "Humidity:", COLOR565_RED,COLOR565_BLACK);
	ST7735_PutStr7x11(5,  58, "eCO2:", COLOR565_RED,COLOR565_BLACK);
	ST7735_PutStr7x11(5,  83, "TVOC:", COLOR565_RED,COLOR565_BLACK);
	ST7735_PutStr7x11(82,  83, "CH2O:", COLOR565_RED,COLOR565_BLACK);
	ST7735_PutStr7x11(5,  108, "PM2.5:", COLOR565_RED,COLOR565_BLACK);
	ST7735_PutStr7x11(82,  108, "PM10:", COLOR565_RED,COLOR565_BLACK);
	
	LCD_LED_ON();
	
	HAL_Delay(5000);
	Init_Voc();
	Init_PM25();
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
	if(Get_Voc(Voc_Data_Buff) >= 0)
	{
		ECO2_Data = Voc_Data_Buff[0];
		ECO2_Data = ECO2_Data << 8;
		ECO2_Data += Voc_Data_Buff[1];

		TVOC_Data = Voc_Data_Buff[2];
		TVOC_Data = TVOC_Data << 8;
		TVOC_Data += Voc_Data_Buff[3];	

		CH2O_Data = Voc_Data_Buff[4];
		CH2O_Data = CH2O_Data << 8;
		CH2O_Data += Voc_Data_Buff[5];
	}
	else
	{
		ECO2_Data = 9999;
		TVOC_Data = 9999;
		CH2O_Data = 9999;
	}

//  HAL_Delay(1000);
	LED_SYS_ON();
	  
	if(Get_PM25(PM25_Data_Buff) >= 0 )
	{
		PM25_Data = PM25_Data_Buff[0];
		PM25_Data = PM25_Data << 8;
		PM25_Data += PM25_Data_Buff[1];
		  
		PM10_Data = PM25_Data_Buff[2];
		PM10_Data = PM10_Data << 8;
		PM10_Data += PM25_Data_Buff[3];	
	}
	else
	{
		PM25_Data = 9999;
		PM10_Data = 9999;
	}
	
    HAL_Delay(1000);
	
	LED_SYS_ON();
	
	sprintf((char *)Lcd_Display_Buff,"%.1f C",(float)Actual_Temperature_Data/10); 
	ST7735_PutStr7x11(104,  8,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	sprintf((char *)Lcd_Display_Buff,"%.1f %%",(float)Actual_Humidity_Data/10);  
	ST7735_PutStr7x11(88,  33,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	  
	sprintf((char *)Lcd_Display_Buff,"%d ppm",ECO2_Data);
	ST7735_PutStr7x11(48,  58,"        ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	ST7735_PutStr7x11(48,  58,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	sprintf((char *)Lcd_Display_Buff,"%d",TVOC_Data);
	ST7735_PutStr7x11(47,  83,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	ST7735_PutStr7x11(47,  83,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	sprintf((char *)Lcd_Display_Buff,"%d",CH2O_Data);
	ST7735_PutStr7x11(124,  83,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	ST7735_PutStr7x11(124,  83,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	  
	sprintf((char *)Lcd_Display_Buff,"%d",PM25_Data);
	ST7735_PutStr7x11(47,  108,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	ST7735_PutStr7x11(47,  108,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	sprintf((char *)Lcd_Display_Buff,"%d",PM10_Data);
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
