#ifndef __MODBUS_H
#define __MODBUS_H

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "usart.h"
#include "crc16.h"
#include "user_config.h"

#ifdef __ATMOSPHERE_SENSOR_7IN1__
#define MODBUS_UART_DEV huart1 //7合1modbus使用huart1
#endif

#ifdef __T_H_SENSOR__
#define MODBUS_UART_DEV huart1 //温湿度modbus使用huart1
#endif

#define SET_MODBUS_TX() HAL_GPIO_WritePin(CTRL485_GPIO_Port,CTRL485_Pin,GPIO_PIN_SET)
#define SET_MODBUS_RX() HAL_GPIO_WritePin(CTRL485_GPIO_Port,CTRL485_Pin,GPIO_PIN_RESET)


//保持寄存器起始
#define MODBUS_MEMORY_START_ADDRESS 0 
//保持寄存器数量
#define MODBUS_MEMORY_QUANTITY 7
//收发缓存大小
#define MODBUS_BUFFER_SIZE 32



typedef struct
{
	uint8_t Modbus_Equipment_Address;//设备地址
	uint8_t Modbus_Commend;//命令
	uint16_t Modbus_Memory_Address;//读取的数据地址
	uint16_t Modbus_Memory_len;//读取的长度，字（2byte）
	uint8_t Recall_Type; //回复类型，正常或各种错误
}MODBUS_RX_ELEMENT;


enum MODBUS_ERROR_CODE
{ 
	NO_ERROR = 0,
	COMMEND_ERROR,
	MEMORY_ERROR,
	CRC_ERROR,
};


//#define    

void Init_Modbus(void);
void Modbus_Uart_Rx_Int_Process(void);
void Modbus_Uart_Tx_Complete_Process(void);
void Modbus_Timer_Process(void);
void Task_Modbus(void);

#endif //#endif __MODBUS_H
