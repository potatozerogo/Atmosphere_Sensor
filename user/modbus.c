#include "modbus.h"

uint8_t  Modbus_Rx_Buff[MODBUS_BUFFER_SIZE];//接收缓存Rx_uart1_buff->Modbus_Rx_Buff
uint8_t  Modbus_Tx_Buff[MODBUS_BUFFER_SIZE];//发送缓存Tx_uart1_buff->Modbus_Tx_Buff
uint8_t  Modbus_Rx_Buff_Index = 0;//接收数据指针Rx_uart1_buff_index->Modbus_Rx_Buff_Index
uint8_t  Modbus_Rx_Timer = 0;//接收数据帧结束延时,用于判读一帧数据结束，单位100us Rx_uart1_Timer->Modbus_Rx_Timer
uint8_t  Modbus_Rx_Timer_Limiter = 0;//接收数据帧结束延时限制,根据波特率调节,单位100us，如：9600延时2ms。
//uint8_t  Modbus_Receive_Complete_Flag = 0;//接收到完整数据帧后，同步传递信号标志(改用信号量同步)
uint8_t  Modbus_Receive_Length = 0;//接收数据的长度
uint8_t  Modbus_Reply_Delay_Time = 0;//回复主机延时，单位ms，由于数据处理太快，避免过早回复，造成两帧数据合并。根据波特率计算 

uint8_t  Modbus_New_Receive_Start_Flag = 0;//重新开始新的一帧的标志New_Receive_Start_Flag->Modbus_New_Receive_Start_Flag
uint8_t  Modbus_Receive_Last_Index = 0;//新一帧第一字节数据会被记录在上一帧结尾再后面的字节里，用于将数据搬运到新一帧的帧头。Receive_Last_Index->Modbus_Receive_Last_Index

extern osSemaphoreId MODBUS_RX_CPLHandle;//接收到完整数据帧后，同步的二值信号量

//uint16_t Modbus_Memory_List[MODBUS_MEMORY_QUANTITY];//数据存储空间
uint16_t * Modbus_Memory_Address;//modbus数据存储空间首地址
uint8_t My_Modbus_Address;//modbus设备地址

extern uint16_t Temperature_Data;

/**************************
* @brief MODBUS初始化
* @param 无
* 
* @retval 无
**************************/
void Init_Modbus(void)
{
	My_Modbus_Address = 0x02;
	Modbus_Rx_Timer_Limiter = 3840/(MODBUS_UART_DEV.Init.BaudRate/100);//计算一帧数据结束时间长度(3.5*(位数)/BaudRate)*(1000000us/100us)
	Modbus_Reply_Delay_Time = Modbus_Rx_Timer_Limiter/7;//计算回复主机延时
}


/*************************
* @brief MODBUS接收到一个字节数据处理，放在uart接收中断里。
* @param 无
* 
* @retval 无
***********************/
void Modbus_Uart_Rx_Int_Process(void)
{
	if(Modbus_New_Receive_Start_Flag == 1)
	{
		Modbus_New_Receive_Start_Flag = 0;
		Modbus_Rx_Buff[0] = Modbus_Rx_Buff[Modbus_Receive_Last_Index];
		Modbus_Rx_Buff_Index = 0;
	}
	Modbus_Rx_Buff_Index++;
	Modbus_Receive_Last_Index = Modbus_Rx_Buff_Index;
	HAL_UART_Receive_IT(&MODBUS_UART_DEV,&Modbus_Rx_Buff[Modbus_Rx_Buff_Index],1);
	Modbus_Rx_Timer = 0;
}

/**************************
* @brief 用于MODBUS发送完一帧数据后，切换485总线状态，放在uart发送完成中断里。
* @param 无
* 
* @retval 无
***************************/
void Modbus_Uart_Tx_Complete_Process(void)
{
	SET_MODBUS_RX();//485接收	
}

/**************************
* @brief MODBUS辅助计时器处理，用于判断一帧结束和新帧开始，放在100ms定时里。
* @param 无
* 
* @retval 无
***************************/
void Modbus_Timer_Process(void)
{
	if(++Modbus_Rx_Timer >= Modbus_Rx_Timer_Limiter)
	{
		Modbus_Rx_Timer = 0;
		if(Modbus_Rx_Buff_Index >= 8)
		{
			Modbus_Receive_Length = Modbus_Rx_Buff_Index;
			//Modbus_Receive_Complete_Flag = 1;
			osSemaphoreRelease(MODBUS_RX_CPLHandle);
//				LED1_ON();//============测试窗口同步速度===============
		}
		Modbus_New_Receive_Start_Flag = 1;
		Modbus_Rx_Buff_Index = 0;
	}
}


/******************************************
* @brief 解析处理modbus接受到数据
* @param 接收数据元素结构体指针
			接收到的数据字节数
* 
* @retval 是否需要回复 NO_RACALL 0;RACALL 1
*******************************************/
#define NO_CALL_BACK 0
#define CALL_BACK 1

uint8_t Analysis_Modbus_Rx_Data(MODBUS_RX_ELEMENT* rx_element,uint8_t data_len)
{
	uint16_t CRC_Result;
	rx_element->Modbus_Equipment_Address = Modbus_Rx_Buff[0];
	rx_element->Modbus_Commend = Modbus_Rx_Buff[1];
	rx_element->Modbus_Memory_Address = Modbus_Rx_Buff[2];
	rx_element->Modbus_Memory_Address = rx_element->Modbus_Memory_Address << 8;
	rx_element->Modbus_Memory_Address += Modbus_Rx_Buff[3];
	rx_element->Modbus_Memory_len = Modbus_Rx_Buff[4];
	rx_element->Modbus_Memory_len = rx_element->Modbus_Memory_len << 8;
	rx_element->Modbus_Memory_len += Modbus_Rx_Buff[5];
	
	
	//地址不正确
	if(rx_element->Modbus_Equipment_Address != My_Modbus_Address)
	{
		return (NO_CALL_BACK);
	}
	
	//校验错误
	CRC_Result = Get_CRC16(Modbus_Rx_Buff,data_len-2);
	if((Modbus_Rx_Buff[data_len-2] != (CRC_Result & 0x00ff)) || (Modbus_Rx_Buff[data_len-1] != (CRC_Result >> 8)))
	{
		rx_element->Recall_Type = CRC_ERROR;
		return (CALL_BACK);
	}
	
	//无效命令
	if(rx_element->Modbus_Commend != 0x03)
	{
		rx_element->Recall_Type = COMMEND_ERROR;
		return (CALL_BACK);
	}
	
	//0x03命令处理
	if(rx_element->Modbus_Commend == 0x03)
	{
		if( (rx_element->Modbus_Memory_Address >= MODBUS_MEMORY_START_ADDRESS) 
			&& ( rx_element->Modbus_Memory_Address + rx_element->Modbus_Memory_len <= (MODBUS_MEMORY_START_ADDRESS + MODBUS_MEMORY_QUANTITY)) )
		{
			rx_element->Recall_Type = NO_ERROR;
			return (CALL_BACK);
		}
		else// 读取超范围
		{
			rx_element->Recall_Type = MEMORY_ERROR;
			return (CALL_BACK);
		}
	}
	
	return (NO_CALL_BACK);
}


/************************************
* @brief 建立modbus发送数据
* @param 数据存储空间首地址
*		 接收数据元素结构体
* 
* @retval 发送的总字节数
*************************************/
uint8_t Build_Modbus_Tx_Data(uint16_t* memory_add,MODBUS_RX_ELEMENT* element)
{
	uint8_t i;
	uint16_t CRC_Result;
	if(element->Recall_Type == NO_ERROR)
	{
		Modbus_Tx_Buff[0] = My_Modbus_Address;
		Modbus_Tx_Buff[1] = element->Modbus_Commend;
		Modbus_Tx_Buff[2] = element->Modbus_Memory_len * 2;
		for(i=0;i<element->Modbus_Memory_len;i++)
		{
			Modbus_Tx_Buff[i*2+3] = memory_add[element->Modbus_Memory_Address+i] >> 8;
			Modbus_Tx_Buff[i*2+1+3] = memory_add[element->Modbus_Memory_Address+i] & 0x00ff;
		}
		CRC_Result = Get_CRC16(Modbus_Tx_Buff,element->Modbus_Memory_len*2+3);
		Modbus_Tx_Buff[element->Modbus_Memory_len*2+3] = CRC_Result & 0x00ff;
		Modbus_Tx_Buff[element->Modbus_Memory_len*2+1+3] = CRC_Result >> 8;
		return (element->Modbus_Memory_len*2+3+2);
	}
	else
	{
		Modbus_Tx_Buff[0] = My_Modbus_Address;
		Modbus_Tx_Buff[1] = element->Modbus_Commend | 0x80;
		Modbus_Tx_Buff[2] = element->Recall_Type; 
		CRC_Result = Get_CRC16(Modbus_Tx_Buff,3);
		Modbus_Tx_Buff[3] = CRC_Result & 0x00ff;
		Modbus_Tx_Buff[4] = CRC_Result >> 8;
		return (5);
	}
}


void Modbus_Send_Data(uint8_t *buff,uint8_t len)
{
	SET_MODBUS_TX();	//设置为485发送模式
	HAL_UART_Transmit_DMA(&MODBUS_UART_DEV,buff,len);
	//设置回485接收模式在串口发送完成中断中完成
}

void Task_Modbus(void)
{
	uint8_t temp;
	uint8_t len;
	MODBUS_RX_ELEMENT Modbus_Receive_Element;
	
	Modbus_Rx_Buff_Index = 0;
	HAL_UART_Receive_IT(&MODBUS_UART_DEV,&Modbus_Rx_Buff[Modbus_Rx_Buff_Index],1);
	
	while(1)
	{
		
		osSemaphoreWait(MODBUS_RX_CPLHandle,portMAX_DELAY);
//		if(Modbus_Receive_Complete_Flag == 1)
//		{
//			Modbus_Receive_Complete_Flag = 0;
//			LED1_OFF();//============测试窗口同步速度===============

			temp = Analysis_Modbus_Rx_Data(&Modbus_Receive_Element,Modbus_Receive_Length);
			if(temp == CALL_BACK)
			{
				len = Build_Modbus_Tx_Data(Modbus_Memory_Address,&Modbus_Receive_Element);
				osDelay(Modbus_Reply_Delay_Time);//回复主机延时，单位ms，由于数据处理太快，避免过早回复，造成两帧数据合并。根据波特率计算
				Modbus_Send_Data(Modbus_Tx_Buff,len);
			}
//		}
//		osDelay(1);
	}
}
