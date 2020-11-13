/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "st7735.h"
#include "voc.h"
#include "pm25.h"
#include "sht2x_i2c.h"
#include "key.h"

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
/* USER CODE BEGIN Variables */
extern uint8_t Rx_uart2_buff[32];
extern uint8_t Tx_uart2_buff[32];
extern uint8_t UART2_Transmit_Flag;

extern uint8_t Voc_Data_Buff[6];
extern uint8_t PM25_Data_Buff[4];
extern uint16_t ECO2_Data;
extern uint16_t TVOC_Data;
extern uint16_t CH2O_Data;
extern uint16_t PM25_Data;
extern uint16_t PM10_Data;
extern int16_t Actual_Temperature_Data; 
extern int16_t Actual_Humidity_Data;

////===========调试用===============
uint16_t Test_Counter;//调试累计
////===========调试用===============

char Lcd_Display_Buff[8];

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId LCD_DisplayHandle;
osThreadId TH_SensorHandle;
osThreadId AtmosphereHandle;
osThreadId ModbusHandle;
osSemaphoreId MODBUS_RX_CPLHandle;
osStaticSemaphoreDef_t MODBUS_RX_CPL_ControlBlock;
osSemaphoreId VOC_RX_CPLHandle;
osStaticSemaphoreDef_t VOC_RX_CPLControlBlock;
osSemaphoreId PM25_RX_CPLHandle;
osStaticSemaphoreDef_t PM25_RX_CPLControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask_LCD_Display(void const * argument);
void StartTask_TH_Sensor(void const * argument);
void StartTask_Atmosphere(void const * argument);
void StartTask_Modbus(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of MODBUS_RX_CPL */
  osSemaphoreStaticDef(MODBUS_RX_CPL, &MODBUS_RX_CPL_ControlBlock);
  MODBUS_RX_CPLHandle = osSemaphoreCreate(osSemaphore(MODBUS_RX_CPL), 1);

  /* definition and creation of VOC_RX_CPL */
  osSemaphoreStaticDef(VOC_RX_CPL, &VOC_RX_CPLControlBlock);
  VOC_RX_CPLHandle = osSemaphoreCreate(osSemaphore(VOC_RX_CPL), 1);

  /* definition and creation of PM25_RX_CPL */
  osSemaphoreStaticDef(PM25_RX_CPL, &PM25_RX_CPLControlBlock);
  PM25_RX_CPLHandle = osSemaphoreCreate(osSemaphore(PM25_RX_CPL), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 64);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LCD_Display */
  osThreadDef(LCD_Display, StartTask_LCD_Display, osPriorityNormal, 0, 128);
  LCD_DisplayHandle = osThreadCreate(osThread(LCD_Display), NULL);

  /* definition and creation of TH_Sensor */
  osThreadDef(TH_Sensor, StartTask_TH_Sensor, osPriorityNormal, 0, 128);
  TH_SensorHandle = osThreadCreate(osThread(TH_Sensor), NULL);

  /* definition and creation of Atmosphere */
  osThreadDef(Atmosphere, StartTask_Atmosphere, osPriorityNormal, 0, 128);
  AtmosphereHandle = osThreadCreate(osThread(Atmosphere), NULL);

  /* definition and creation of Modbus */
  osThreadDef(Modbus, StartTask_Modbus, osPriorityAboveNormal, 0, 128);
  ModbusHandle = osThreadCreate(osThread(Modbus), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	uint8_t one_second_counter = 0;
	uint8_t key_data;
  /* Infinite loop */
  for(;;)
  {
	key_data = Read_Key();
	switch(key_data)
	{
		case 1:
			Standby_PM25();
			break;
		case 2:
			Work_PM25();
			break;
		case 3:
			HAL_GPIO_TogglePin(LCD_LED_GPIO_Port,LCD_LED_Pin);
			break;
	}
	key_data = 0;
	if(++one_second_counter >= 10)
	{
		one_second_counter = 0;	
//		LED_SYS_TOGGLE();
	}
	osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask_LCD_Display */
/**
* @brief Function implementing the LCD_Display thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_LCD_Display */
void StartTask_LCD_Display(void const * argument)
{
  /* USER CODE BEGIN StartTask_LCD_Display */
  /* Infinite loop */
  for(;;)
  {
	  
	sprintf((char *)Lcd_Display_Buff,"%.1f C",(float)Actual_Temperature_Data/10); 
	ST7735_PutStr7x11(104,  8,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	sprintf((char *)Lcd_Display_Buff,"%.1f %%",(float)Actual_Humidity_Data/10);  
	ST7735_PutStr7x11(88,  33,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
//    osDelay(1000);
	  
	sprintf((char *)Lcd_Display_Buff,"%d ppm",ECO2_Data);
	ST7735_PutStr7x11(48,  58,"        ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	ST7735_PutStr7x11(48,  58,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	sprintf((char *)Lcd_Display_Buff,"%d",TVOC_Data);
	ST7735_PutStr7x11(47,  83,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	ST7735_PutStr7x11(47,  83,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	sprintf((char *)Lcd_Display_Buff,"%d",CH2O_Data);
	ST7735_PutStr7x11(124,  83,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	ST7735_PutStr7x11(124,  83,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
//    osDelay(1000);
	  
	sprintf((char *)Lcd_Display_Buff,"%d",PM25_Data);
	ST7735_PutStr7x11(47,  108,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	ST7735_PutStr7x11(47,  108,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
	sprintf((char *)Lcd_Display_Buff,"%d",PM10_Data);
	ST7735_PutStr7x11(117,  108,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK);
	ST7735_PutStr7x11(117,  108,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
    osDelay(2000);

	  
  }
  /* USER CODE END StartTask_LCD_Display */
}

/* USER CODE BEGIN Header_StartTask_TH_Sensor */
/**
* @brief Function implementing the TH_Sensor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_TH_Sensor */
void StartTask_TH_Sensor(void const * argument)
{
  /* USER CODE BEGIN StartTask_TH_Sensor */
  /* Infinite loop */
  for(;;)
  {
	Task_TH_Sensor();
    osDelay(1);
  }
  /* USER CODE END StartTask_TH_Sensor */
}

/* USER CODE BEGIN Header_StartTask_Atmosphere */
/**
* @brief Function implementing the Atmosphere thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Atmosphere */
void StartTask_Atmosphere(void const * argument)
{
  /* USER CODE BEGIN StartTask_Atmosphere */
  /* Infinite loop */
  for(;;)
  {
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
		ECO2_Data = 999;
		TVOC_Data = 999;
		CH2O_Data = 999;
	}
    osDelay(1000);
	
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
		PM25_Data = 999;
		PM10_Data = 999;
	}
    osDelay(1000);
  }
  /* USER CODE END StartTask_Atmosphere */
}

/* USER CODE BEGIN Header_StartTask_Modbus */
/**
* @brief Function implementing the Modbus thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Modbus */
void StartTask_Modbus(void const * argument)
{
  /* USER CODE BEGIN StartTask_Modbus */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask_Modbus */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
