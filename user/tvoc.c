#include "tvoc.h"

extern osSemaphoreId TVOC_RX_CPLHandle;//接收到完整数据帧后，同步的二值信号量

uint8_t Tvoc_Data_Buff[6];//输出数据接口全局变量
uint8_t Tvoc_Read_Uart_Buff[20];//串口读取数据缓存
uint8_t Tvoc_DMA_Command_Buff[9] = {0x42,0x86,0x04,0x00,0x00,0x00,0x00,0x00,0xff};//读数据命令;


/**************************************************************************
函数功能：CRC 校验函数，生成 CRC
参数说明：arr_buff：需要校验的数组集
len：需要校验数据的长度
返回参数：CRC 为 unsigned int 类型，高位字节为高位在前，低位在后
**************************************************************************/
unsigned int CRC_Compute ( unsigned char *arr_buff, unsigned char len)
{
unsigned int crc=0xFFFF;
unsigned char i, j;
for ( j=0; j <len;j++)
{
crc=crc ^*arr_buff++;
for ( i=0; i<8; i++)
{
if( ( crc&0x0001) >0)
{
crc=crc>>1;
crc=crc^ 0xa001;
}
else
crc=crc>>1;
}
}
return ( crc);
}




/**************************************************************************
函数功能： 设置VOC为被动模式
**************************************************************************/
int8_t Tvoc_Set_Read_Mod(void)
{
	__HAL_UART_CLEAR_FEFLAG(&TVOC_UART_DEV);
	__HAL_UART_CLEAR_NEFLAG(&TVOC_UART_DEV);
	__HAL_UART_CLEAR_PEFLAG(&TVOC_UART_DEV);
	__HAL_UART_CLEAR_OREFLAG(&TVOC_UART_DEV);
	__HAL_UART_CLEAR_FLAG(&TVOC_UART_DEV, UART_FLAG_TC);
	uint8_t Command_Buff[] = {0x42,0x78,0x04,0x00,0x00,0x00,0x00,0x00,0xff};//被动模式命令	
	HAL_UART_Transmit(&TVOC_UART_DEV,Command_Buff,9,10000);
	
	__HAL_UART_CLEAR_FLAG(&TVOC_UART_DEV,UART_FLAG_RXNE);
	HAL_UART_Receive(&TVOC_UART_DEV,Tvoc_Read_Uart_Buff,9,300);//等待形式读取。
	
	if(Tvoc_Read_Uart_Buff[0] == 0x42 && Tvoc_Read_Uart_Buff[1] == 0x78 && Tvoc_Read_Uart_Buff[2] == 0x01)
	{
		return 0;
	}
	return -1;
}

/**************************************************************************
* @brief 初始化VOC
* @param 
* 
* @retval 0成功，-1失败
**************************************************************************/
int8_t Init_Tvoc(void)
{
#ifdef __ATMOSPHERE_SENSOR_7IN1__
	for (uint8_t i=0; i<10; i++)
#endif
		if(Tvoc_Set_Read_Mod() == 0)
					return 0;
	return -1;
}


/*********************************************************************
* @brief 读VOC
* @param uint8_t *buff
* 
* @retval 0成功，-1失败
*********************************************************************/
int8_t Get_Tvoc(uint8_t *buff)
{
//	uint8_t Command_Buff[] = {0x42,0x86,0x04,0x00,0x00,0x00,0x00,0x00,0xff};//读数据命令
	uint16_t CRC_Result;
	uint8_t i;
	
	/* 第1步：发出读数据指令，开启读取 */
	for(i=0;i<17;i++)
		Tvoc_Read_Uart_Buff[i] = 0;
	__HAL_UART_CLEAR_FEFLAG(&TVOC_UART_DEV);
	__HAL_UART_CLEAR_NEFLAG(&TVOC_UART_DEV);
	__HAL_UART_CLEAR_PEFLAG(&TVOC_UART_DEV);
	__HAL_UART_CLEAR_OREFLAG(&TVOC_UART_DEV);
	__HAL_UART_CLEAR_FLAG(&TVOC_UART_DEV, UART_FLAG_TC);
//	HAL_UART_Transmit(&TVOC_UART_DEV,Command_Buff,9,10000);//等待形式发送。
	HAL_UART_Transmit_DMA(&TVOC_UART_DEV,Tvoc_DMA_Command_Buff,9);//DMA形式发送。
	
//	__HAL_UART_CLEAR_FEFLAG(&TVOC_UART_DEV);
//	__HAL_UART_CLEAR_NEFLAG(&TVOC_UART_DEV);
//	__HAL_UART_CLEAR_PEFLAG(&TVOC_UART_DEV);
//	__HAL_UART_CLEAR_OREFLAG(&TVOC_UART_DEV);
	__HAL_UART_CLEAR_FLAG(&TVOC_UART_DEV,UART_FLAG_RXNE);
//	HAL_UART_Receive(&TVOC_UART_DEV,Tvoc_Read_Uart_Buff,17,200);//等待形式读取。
	HAL_UART_Receive_IT(&TVOC_UART_DEV,Tvoc_Read_Uart_Buff,17);//中断形式读取。

	/* 第2步：等待读取数据 */
	osSemaphoreWait(TVOC_RX_CPLHandle,200);	
	 
	/* 第3步：获取数据后校验 */
	if(Tvoc_Read_Uart_Buff[0] == 0x42 && Tvoc_Read_Uart_Buff[1] == 0x4d)
	{
		CRC_Result = CRC_Compute( & Tvoc_Read_Uart_Buff[2],13);
		if((Tvoc_Read_Uart_Buff[15] == (CRC_Result & 0x00ff)) && (Tvoc_Read_Uart_Buff[16] == (CRC_Result >> 8)))
		{
			buff[0] = Tvoc_Read_Uart_Buff[9];
			buff[1] = Tvoc_Read_Uart_Buff[10];
			buff[2] = Tvoc_Read_Uart_Buff[11];
			buff[3] = Tvoc_Read_Uart_Buff[12];
			buff[4] = Tvoc_Read_Uart_Buff[13];
			buff[5] = Tvoc_Read_Uart_Buff[14];
			return 0;
		}
	}
	return -1;
}








