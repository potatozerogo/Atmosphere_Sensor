#include "pm25.h"
#include "main.h"
#include "usart.h"
#include "cmsis_os.h"


extern osSemaphoreId PM25_RX_CPLHandle;//���յ���������֡��ͬ���Ķ�ֵ�ź���

uint8_t PM25_Data_Buff[4];//������ݽӿ�ȫ�ֱ���
uint8_t PM25_Read_Uart_Buff[34];//���ڶ�ȡ���ݻ���
uint8_t PM25_DMA_Command_Buff[] = {0x42,0x4d,0xe2,0x00,0x00,0x01,0x71};//����������

/**************************************************************************
�������ܣ� ��У�麯����
����˵����arr_buff����ҪУ������鼯
len����ҪУ�����ݵĳ���
���ز�����unsigned int ���ͣ���λ�ֽ�Ϊ��λ��ǰ����λ�ں�
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
�������ܣ� ����PM25 Ϊ����ģʽ
**************************************************************************/
int8_t PM25_Set_Read_Mod(void)
{
	__HAL_UART_CLEAR_FEFLAG(&huart2);
	__HAL_UART_CLEAR_NEFLAG(&huart2);
	__HAL_UART_CLEAR_PEFLAG(&huart2);
	__HAL_UART_CLEAR_OREFLAG(&huart2);
	__HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TC);
	uint8_t Command_Buff[] = {0x42,0x4d,0xe1,0x00,0x00,0x01,0x70};//����ģʽ����	
	HAL_UART_Transmit(&huart2,Command_Buff,7,10000);
	
	__HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_RXNE);
	HAL_UART_Receive(&huart2,PM25_Read_Uart_Buff,8,200);//�ȴ���ʽ��ȡ��

	if(PM25_Read_Uart_Buff[0] == 0x42 && PM25_Read_Uart_Buff[1] == 0x4d && PM25_Read_Uart_Buff[4] == 0xe1)
	{
		return 0;
	}
	return -1;	
}

/**************************************************************************
�������ܣ� ��ʼ��PM25 Ϊ����ģʽ
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
�������ܣ� PM25 Ϊ����ģʽ
**************************************************************************/
int8_t Standby_PM25(void)
{
	//HAL_GPIO_WritePin(PM_RST_GPIO_Port,PM_RST_Pin,GPIO_PIN_RESET);	
	uint8_t Command_Buff[] = {0x42,0x4d,0xe4,0x00,0x00,0x01,0X73};//����ģʽ����	
	HAL_UART_Transmit(&huart2,Command_Buff,7,10000);
	return 0;
}

/**************************************************************************
�������ܣ� PM25 Ϊ����ģʽ
**************************************************************************/
int8_t Work_PM25(void)
{
	//HAL_GPIO_WritePin(PM_RST_GPIO_Port,PM_RST_Pin,GPIO_PIN_RESET);	
	uint8_t Command_Buff[] = {0x42,0x4d,0xe4,0x00,0x01,0x01,0X74};//����ģʽ����	
	HAL_UART_Transmit(&huart2,Command_Buff,7,10000);
	return 0;
}


/**************************************************************************
�������ܣ� PM25 ���Ͷ�ȡ������������ã�
**************************************************************************/
int8_t Read_PM25(void)
{
	uint8_t Command_Buff[] = {0x42,0x4d,0xe2,0x00,0x00,0x01,0x71};//����������
	HAL_UART_Transmit(&huart2,Command_Buff,7,10000);
	return 0;
}



/**
* @brief ��PM25����
* @param uint8_t *buff
* 
* @retval 0�ɹ���-1ʧ��
*/
int8_t Get_PM25(uint8_t *buff)
{
//	uint8_t Command_Buff[] = {0x42,0x4d,0xe2,0x00,0x00,0x01,0x71};//����������
	uint16_t Sum_Result;
	uint8_t i;
	
	/* ��1��������������ָ�������ȡ */
	for(i=0;i<32;i++)
		PM25_Read_Uart_Buff[i] = 0;
	__HAL_UART_CLEAR_FEFLAG(&huart2);
	__HAL_UART_CLEAR_NEFLAG(&huart2);
	__HAL_UART_CLEAR_PEFLAG(&huart2);
	__HAL_UART_CLEAR_OREFLAG(&huart2);
	__HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TC);
//	HAL_UART_Transmit(&huart2,Command_Buff,7,10000);//�ȴ���ʽ���͡�
	HAL_UART_Transmit_DMA(&huart2,PM25_DMA_Command_Buff,7);//DMA��ʽ���͡�
	
//	__HAL_UART_CLEAR_FEFLAG(&huart2);
//	__HAL_UART_CLEAR_NEFLAG(&huart2);
//	__HAL_UART_CLEAR_PEFLAG(&huart2);
//	__HAL_UART_CLEAR_OREFLAG(&huart2);
	__HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_RXNE);
//	HAL_UART_Receive(&huart2,PM25_Read_Uart_Buff,32,200);//�ȴ���ʽ��ȡ��
	HAL_UART_Receive_IT(&huart2,PM25_Read_Uart_Buff,32);//�ж���ʽ��ȡ��

	/* ��2�����ȴ���ȡ���� */
	osSemaphoreWait(PM25_RX_CPLHandle,200);	
	 
	/* ��3������ȡ���ݺ�У�� */
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








