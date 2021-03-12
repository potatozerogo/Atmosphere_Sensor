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
#include "modbus.h"
#include "tvoc.h"
#include "pm25.h"
#include "sht2x_i2c.h"
#include "key.h"
#include "display_number.h"
#include "data_colour.h"
#include "user_config.h"

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
extern uint8_t Tvoc_Data_Buff[6];
extern uint8_t Pm25_Data_Buff[4];
extern uint16_t Temperature_Data;
extern uint16_t Humidity_Data;
extern uint16_t Eco2_Data;
extern uint16_t Tvoc_Data;
extern uint16_t Ch2o_Data;
extern uint16_t Pm25_Data;
extern uint16_t Pm10_Data;
extern int16_t Actual_Temperature_Data; 
extern int16_t Actual_Humidity_Data;
extern uint16_t Modbus_Memory_List[];//数据存储空间//modbus.c

extern _AQI_COLOUR_TABLE Co2_Colour_Table;//气体颜色对应表//data_colour.c 
extern _AQI_COLOUR_TABLE Co_Colour_Table;//气体颜色对应表//data_colour.c 
extern _AQI_COLOUR_TABLE Hcho_Colour_Table;//气体颜色对应表//data_colour.c 
extern _AQI_COLOUR_TABLE Tvoc_Colour_Table;//气体颜色对应表//data_colour.c
extern _AQI_COLOUR_TABLE Pm25_Colour_Table;//AQI颜色对应表//data_colour.c
extern _AQI_COLOUR_TABLE Pm10_Colour_Table;//AQI颜色对应表//data_colour.c
extern _TEMPERATURE_COLOUR_TABLE Temperature_Colour_Table;//温度颜色对应表//data_colour.c
extern _TEMPERATURE_COLOUR_TABLE Humidity_Colour_Table;//湿度颜色对应表//data_colour.c	

extern unsigned int Display_Sensor_Type;//显示传感器类型main.c
extern char *String_Display_Sensor_Type[7];//main.c
extern char Lcd_Display_Buff[8];//main.c

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
osSemaphoreId RUSH_DISPLAY_REQUESTHandle;
osStaticSemaphoreDef_t RUSH_DISPLAY_REQUESTControlBlock;

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

  /* definition and creation of RUSH_DISPLAY_REQUEST */
  osSemaphoreStaticDef(RUSH_DISPLAY_REQUEST, &RUSH_DISPLAY_REQUESTControlBlock);
  RUSH_DISPLAY_REQUESTHandle = osSemaphoreCreate(osSemaphore(RUSH_DISPLAY_REQUEST), 1);

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
  osThreadDef(Modbus, StartTask_Modbus, osPriorityAboveNormal, 0, 192);
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
			Standby_Pm25();
			break;
		case 2:
			Work_Pm25();
			break;
		case 3:
			if(++Display_Sensor_Type >= _END_OF_DISPALY_TYPE)
				Display_Sensor_Type = _TEMPERATURE;
			osSemaphoreRelease(RUSH_DISPLAY_REQUESTHandle);
			break;
	}
	key_data = 0;
	if(++one_second_counter >= 10)
	{
		one_second_counter = 0;	
//		LED_SYS_TOGGLE();
	}
	Temperature_Data = Actual_Temperature_Data - 20;
	Humidity_Data = Actual_Humidity_Data;
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
	
	ST7735_PutStr7x11(33,  8, String_Display_Sensor_Type[Display_Sensor_Type], COLOR565_WHITE_SMOKE,COLOR565_BLACK); 

	switch(Display_Sensor_Type)
	{
		case _TEMPERATURE:
			ST7735_PutStr7x11(110,  82,"   'C", COLOR565_GREEN,COLOR565_BLACK);
			ST7735_Put_3Bits_Big_Percent_Number(3, 40, (Temperature_Data), Set_Temperature_Colour((Temperature_Data), &Temperature_Colour_Table), COLOR565_BLACK);
			break;
		case _HUMIDITY:
			ST7735_PutStr7x11(110,  82,"    %", COLOR565_GREEN,COLOR565_BLACK); 
			ST7735_Put_3Bits_Big_Percent_Number(3, 40, Humidity_Data, Set_Temperature_Colour(Humidity_Data, &Temperature_Colour_Table), COLOR565_BLACK);
			break;
		case _PM25:
			ST7735_PutStr7x11(110,  82,"ug/m3", COLOR565_GREEN,COLOR565_BLACK); 
			ST7735_Put_4Bits_Big_Number(19, 40, Pm25_Data, Set_Aqi_Colour(Pm25_Data, &Pm25_Colour_Table), COLOR565_BLACK);
			break;
		case _PM10:
			ST7735_PutStr7x11(110,  82,"ug/m3", COLOR565_GREEN,COLOR565_BLACK); 
			ST7735_Put_4Bits_Big_Number(19, 40, Pm10_Data, Set_Aqi_Colour(Pm10_Data, &Pm10_Colour_Table), COLOR565_BLACK);
			break;
		case _ECO2:
			ST7735_PutStr7x11(110,  82,"  ppm", COLOR565_GREEN,COLOR565_BLACK); 
			ST7735_Put_4Bits_Big_Number(19, 40, Eco2_Data, Set_Aqi_Colour(Eco2_Data, &Co2_Colour_Table), COLOR565_BLACK);
			break;
		case _TVOC:
			ST7735_PutStr7x11(110,  82,"  ppb", COLOR565_GREEN,COLOR565_BLACK); 
			ST7735_Put_4Bits_Big_Number(19, 40, Tvoc_Data, Set_Aqi_Colour(Tvoc_Data, &Tvoc_Colour_Table), COLOR565_BLACK);
			break;
		case _CH2O:
			ST7735_PutStr7x11(110,  82,"ug/m3", COLOR565_GREEN,COLOR565_BLACK); 
			ST7735_Put_4Bits_Big_Number(19, 40, Ch2o_Data, Set_Aqi_Colour(Ch2o_Data, &Hcho_Colour_Table), COLOR565_BLACK);
			break;
//		case _ALL_SENSOR:
//			sprintf((char *)Lcd_Display_Buff,"%.1f'C",(float)Actual_Temperature_Data/10); 
//			ST7735_PutStr7x11(104,  8,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
//			sprintf((char *)Lcd_Display_Buff,"%.1f %%",(float)Actual_Humidity_Data/10);  
//			ST7735_PutStr7x11(88,  33,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
//			sprintf((char *)Lcd_Display_Buff,"%d ppm",Eco2_Data);
//			ST7735_PutStr7x11(48,  58,"        ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
//			ST7735_PutStr7x11(48,  58,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
//			sprintf((char *)Lcd_Display_Buff,"%d",Tvoc_Data);
//			ST7735_PutStr7x11(47,  83,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
//			ST7735_PutStr7x11(47,  83,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
//			sprintf((char *)Lcd_Display_Buff,"%d",Ch2o_Data);
//			ST7735_PutStr7x11(124,  83,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK);
//			ST7735_PutStr7x11(124,  83,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
//			sprintf((char *)Lcd_Display_Buff,"%d",Pm25_Data);
//			ST7735_PutStr7x11(47,  108,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
//			ST7735_PutStr7x11(47,  108,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK); 
//			sprintf((char *)Lcd_Display_Buff,"%d",Pm10_Data);
//			ST7735_PutStr7x11(117,  108,"    ", COLOR565_WHITE_SMOKE,COLOR565_BLACK);
//			ST7735_PutStr7x11(117,  108,Lcd_Display_Buff, COLOR565_WHITE_SMOKE,COLOR565_BLACK);
//			break;
		default:
			break;
	}
	
    osSemaphoreWait(RUSH_DISPLAY_REQUESTHandle,2000);
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
		Eco2_Data = 999;
		Tvoc_Data = 999;
		Ch2o_Data = 999;
	}
    osDelay(1000);
	
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
		Pm25_Data = 999;
		Pm10_Data = 999;
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
	Task_Modbus();
    osDelay(1);
  }
  /* USER CODE END StartTask_Modbus */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
