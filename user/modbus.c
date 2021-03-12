#include "modbus.h"

uint8_t  Modbus_Rx_Buff[MODBUS_BUFFER_SIZE];//���ջ���Rx_uart1_buff->Modbus_Rx_Buff
uint8_t  Modbus_Tx_Buff[MODBUS_BUFFER_SIZE];//���ͻ���Tx_uart1_buff->Modbus_Tx_Buff
uint8_t  Modbus_Rx_Buff_Index = 0;//��������ָ��Rx_uart1_buff_index->Modbus_Rx_Buff_Index
uint8_t  Modbus_Rx_Timer = 0;//��������֡������ʱ,�����ж�һ֡���ݽ�������λ100us Rx_uart1_Timer->Modbus_Rx_Timer
uint8_t  Modbus_Rx_Timer_Limiter = 0;//��������֡������ʱ����,���ݲ����ʵ���,��λ100us���磺9600��ʱ2ms��
//uint8_t  Modbus_Receive_Complete_Flag = 0;//���յ���������֡��ͬ�������źű�־(�����ź���ͬ��)
uint8_t  Modbus_Receive_Length = 0;//�������ݵĳ���
uint8_t  Modbus_Reply_Delay_Time = 0;//�ظ�������ʱ����λms���������ݴ���̫�죬�������ظ��������֡���ݺϲ������ݲ����ʼ��� 

uint8_t  Modbus_New_Receive_Start_Flag = 0;//���¿�ʼ�µ�һ֡�ı�־New_Receive_Start_Flag->Modbus_New_Receive_Start_Flag
uint8_t  Modbus_Receive_Last_Index = 0;//��һ֡��һ�ֽ����ݻᱻ��¼����һ֡��β�ٺ�����ֽ�����ڽ����ݰ��˵���һ֡��֡ͷ��Receive_Last_Index->Modbus_Receive_Last_Index

extern osSemaphoreId MODBUS_RX_CPLHandle;//���յ���������֡��ͬ���Ķ�ֵ�ź���

//uint16_t Modbus_Memory_List[MODBUS_MEMORY_QUANTITY];//���ݴ洢�ռ�
uint16_t * Modbus_Memory_Address;//modbus���ݴ洢�ռ��׵�ַ
uint8_t My_Modbus_Address;//modbus�豸��ַ

extern uint16_t Temperature_Data;

/**************************
* @brief MODBUS��ʼ��
* @param ��
* 
* @retval ��
**************************/
void Init_Modbus(void)
{
	My_Modbus_Address = 0x02;
	Modbus_Rx_Timer_Limiter = 3840/(MODBUS_UART_DEV.Init.BaudRate/100);//����һ֡���ݽ���ʱ�䳤��(3.5*(λ��)/BaudRate)*(1000000us/100us)
	Modbus_Reply_Delay_Time = Modbus_Rx_Timer_Limiter/7;//����ظ�������ʱ
}


/*************************
* @brief MODBUS���յ�һ���ֽ����ݴ�������uart�����ж��
* @param ��
* 
* @retval ��
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
* @brief ����MODBUS������һ֡���ݺ��л�485����״̬������uart��������ж��
* @param ��
* 
* @retval ��
***************************/
void Modbus_Uart_Tx_Complete_Process(void)
{
	SET_MODBUS_RX();//485����	
}

/**************************
* @brief MODBUS������ʱ�����������ж�һ֡��������֡��ʼ������100ms��ʱ�
* @param ��
* 
* @retval ��
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
//				LED1_ON();//============���Դ���ͬ���ٶ�===============
		}
		Modbus_New_Receive_Start_Flag = 1;
		Modbus_Rx_Buff_Index = 0;
	}
}


/******************************************
* @brief ��������modbus���ܵ�����
* @param ��������Ԫ�ؽṹ��ָ��
			���յ��������ֽ���
* 
* @retval �Ƿ���Ҫ�ظ� NO_RACALL 0;RACALL 1
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
	
	
	//��ַ����ȷ
	if(rx_element->Modbus_Equipment_Address != My_Modbus_Address)
	{
		return (NO_CALL_BACK);
	}
	
	//У�����
	CRC_Result = Get_CRC16(Modbus_Rx_Buff,data_len-2);
	if((Modbus_Rx_Buff[data_len-2] != (CRC_Result & 0x00ff)) || (Modbus_Rx_Buff[data_len-1] != (CRC_Result >> 8)))
	{
		rx_element->Recall_Type = CRC_ERROR;
		return (CALL_BACK);
	}
	
	//��Ч����
	if(rx_element->Modbus_Commend != 0x03)
	{
		rx_element->Recall_Type = COMMEND_ERROR;
		return (CALL_BACK);
	}
	
	//0x03�����
	if(rx_element->Modbus_Commend == 0x03)
	{
		if( (rx_element->Modbus_Memory_Address >= MODBUS_MEMORY_START_ADDRESS) 
			&& ( rx_element->Modbus_Memory_Address + rx_element->Modbus_Memory_len <= (MODBUS_MEMORY_START_ADDRESS + MODBUS_MEMORY_QUANTITY)) )
		{
			rx_element->Recall_Type = NO_ERROR;
			return (CALL_BACK);
		}
		else// ��ȡ����Χ
		{
			rx_element->Recall_Type = MEMORY_ERROR;
			return (CALL_BACK);
		}
	}
	
	return (NO_CALL_BACK);
}


/************************************
* @brief ����modbus��������
* @param ���ݴ洢�ռ��׵�ַ
*		 ��������Ԫ�ؽṹ��
* 
* @retval ���͵����ֽ���
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
	SET_MODBUS_TX();	//����Ϊ485����ģʽ
	HAL_UART_Transmit_DMA(&MODBUS_UART_DEV,buff,len);
	//���û�485����ģʽ�ڴ��ڷ�������ж������
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
//			LED1_OFF();//============���Դ���ͬ���ٶ�===============

			temp = Analysis_Modbus_Rx_Data(&Modbus_Receive_Element,Modbus_Receive_Length);
			if(temp == CALL_BACK)
			{
				len = Build_Modbus_Tx_Data(Modbus_Memory_Address,&Modbus_Receive_Element);
				osDelay(Modbus_Reply_Delay_Time);//�ظ�������ʱ����λms���������ݴ���̫�죬�������ظ��������֡���ݺϲ������ݲ����ʼ���
				Modbus_Send_Data(Modbus_Tx_Buff,len);
			}
//		}
//		osDelay(1);
	}
}
