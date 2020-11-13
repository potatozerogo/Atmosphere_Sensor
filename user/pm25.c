#include "pm25.h"
#include "main.h"
#include "usart.h"
#include "cmsis_os.h"


extern osSemaphoreId PM25_RX_CPLHandle;//接收到完整数据帧后，同步的二值信号量

uint8_t PM25_Data_Buff[4];//输出数据接口全局变量
uint8_t PM25_Read_Uart_Buff[34];//串口读取数据缓存
uint8_t PM25_DMA_Command_Buff[] = {0x42,0x4d,0xe2,0x00,0x00,0x01,0x71};//读数据命令

/**************************************************************************
函数功能： 和校验函数，
参数说明：arr_buff：需要校验的数组集
len：需要校验数据的长度
返回参数：unsigned int 类型，高位字节为高位在前，低位在后
**************************************************************************/
unsigned int Sum_Compute ( unsigned char *arr_buff, unsigned char len)
{
	unsigned int Result = 0;
	unsigned char i;
	for ( i=0; i < len; i++)
	{
		Result += arr_buff[i];
	}
	return (Result);
}


/**************************************************************************
函数功能： 设置PM25 为被动模式
**************************************************************************/
int8_t PM25_Set_Read_Mod(void)
{
	__HAL_UART_CLEAR_FEFLAG(&huart2);
	__HAL_UART_CLEAR_NEFLAG(&huart2);
	__HAL_UART_CLEAR_PEFLAG(&huart2);
	__HAL_UART_CLEAR_OREFLAG(&huart2);
	__HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TC);
	uint8_t Command_Buff[] = {0x42,0x4d,0xe1,0x00,0x00,0x01,0x70};//被动模式命令	
	HAL_UART_Transmit(&huart2,Command_Buff,7,10000);
	
	__HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_RXNE);
	HAL_UART_Receive(&huart2,PM25_Read_Uart_Buff,8,200);//等待形式读取。

	if(PM25_Read_Uart_Buff[0] == 0x42 && PM25_Read_Uart_Buff[1] == 0x4d && PM25_Read_Uart_Buff[4] == 0xe1)
	{
		return 0;
	}
	return -1;	
}

/**************************************************************************
函数功能： 初始化PM25 为被动模式
**************************************************************************/
int8_t Init_PM25(void)
{
	//HAL_GPIO_WritePin(PM_RST_GPIO_Port,PM_RST_Pin,GPIO_PIN_RESET);	
	uint8_t i;
	for (i=0;i<10;i++)
	{
		if(PM25_Set_Read_Mod() >= 0)
			return 0;
	}
	return -1;
}


/**************************************************************************
函数功能： PM25 为待机模式
**************************************************************************/
int8_t Standby_PM25(void)
{
	//HAL_GPIO_WritePin(PM_RST_GPIO_Port,PM_RST_Pin,GPIO_PIN_RESET);	
	uint8_t Command_Buff[] = {0x42,0x4d,0xe4,0x00,0x00,0x01,0X73};//待机模式命令	
	HAL_UART_Transmit(&huart2,Command_Buff,7,10000);
	return 0;
}

/**************************************************************************
函数功能： PM25 为工作模式
**************************************************************************/
int8_t Work_PM25(void)
{
	//HAL_GPIO_WritePin(PM_RST_GPIO_Port,PM_RST_Pin,GPIO_PIN_RESET);	
	uint8_t Command_Buff[] = {0x42,0x4d,0xe4,0x00,0x01,0x01,0X74};//工作模式命令	
	HAL_UART_Transmit(&huart2,Command_Buff,7,10000);
	return 0;
}


/**************************************************************************
函数功能： PM25 发送读取数据命令（调试用）
**************************************************************************/
int8_t Read_PM25(void)
{
	uint8_t Command_Buff[] = {0x42,0x4d,0xe2,0x00,0x00,0x01,0x71};//读数据命令
	HAL_UART_Transmit(&huart2,Command_Buff,7,10000);
	return 0;
}



/**
* @brief 读PM25数据
* @param uint8_t *buff
* 
* @retval 0成功，-1失败
*/
int8_t Get_PM25(uint8_t *buff)
{
//	uint8_t Command_Buff[] = {0x42,0x4d,0xe2,0x00,0x00,0x01,0x71};//读数据命令
	uint16_t Sum_Result;
	uint8_t i;
	
	/* 第1步：发出读数据指令，开启读取 */
	for(i=0;i<32;i++)
		PM25_Read_Uart_Buff[i] = 0;
	__HAL_UART_CLEAR_FEFLAG(&huart2);
	__HAL_UART_CLEAR_NEFLAG(&huart2);
	__HAL_UART_CLEAR_PEFLAG(&huart2);
	__HAL_UART_CLEAR_OREFLAG(&huart2);
	__HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TC);
//	HAL_UART_Transmit(&huart2,Command_Buff,7,10000);//等待形式发送。
	HAL_UART_Transmit_DMA(&huart2,PM25_DMA_Command_Buff,7);//DMA形式发送。
	
//	__HAL_UART_CLEAR_FEFLAG(&huart2);
//	__HAL_UART_CLEAR_NEFLAG(&huart2);
//	__HAL_UART_CLEAR_PEFLAG(&huart2);
//	__HAL_UART_CLEAR_OREFLAG(&huart2);
	__HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_RXNE);
//	HAL_UART_Receive(&huart2,PM25_Read_Uart_Buff,32,200);//等待形式读取。
	HAL_UART_Receive_IT(&huart2,PM25_Read_Uart_Buff,32);//中断形式读取。

	/* 第2步：等待读取数据 */
	osSemaphoreWait(PM25_RX_CPLHandle,200);	
	 
	/* 第3步：获取数据后校验 */
	if(PM25_Read_Uart_Buff[0] == 0x42 && PM25_Read_Uart_Buff[1] == 0x4d)
	{
		Sum_Result = Sum_Compute(PM25_Read_Uart_Buff,30);
		if((PM25_Read_Uart_Buff[31] == (Sum_Result & 0x00ff)) && (PM25_Read_Uart_Buff[30] == (Sum_Result >> 8)))
		{
			buff[0] = PM25_Read_Uart_Buff[6];
			buff[1] = PM25_Read_Uart_Buff[7];
			buff[2] = PM25_Read_Uart_Buff[8];
			buff[3] = PM25_Read_Uart_Buff[9];
			return 0;
		}
	}
	return -1;
}








