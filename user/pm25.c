#include "pm25.h"
#include "main.h"
#include "usart.h"
#include "cmsis_os.h"

#ifdef __GAS_SENSOR__
#include "gas_sensor.h"
#endif

extern osSemaphoreId PM25_RX_CPLHandle;//���յ���������֡��ͬ���Ķ�ֵ�ź���

uint8_t Pm25_Data_Buff[4];//������ݽӿ�ȫ�ֱ���
uint8_t Pm25_Read_Uart_Buff[34];//���ڶ�ȡ���ݻ���
uint8_t Pm25_DMA_Command_Buff[] = {0x42,0x4d,0xe2,0x00,0x00,0x01,0x71};//����������

#ifndef __GAS_SENSOR__  //ʹ��ds_co2.h
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
#endif


/**************************************************************************
�������ܣ� ����PM25 Ϊ����ģʽ
**************************************************************************/
int8_t Pm25_Set_Read_Mod(void)
{
	__HAL_UART_CLEAR_FEFLAG(&PM25_UART_DEV);
	__HAL_UART_CLEAR_NEFLAG(&PM25_UART_DEV);
	__HAL_UART_CLEAR_PEFLAG(&PM25_UART_DEV);
	__HAL_UART_CLEAR_OREFLAG(&PM25_UART_DEV);
	__HAL_UART_CLEAR_FLAG(&PM25_UART_DEV, UART_FLAG_TC);
	uint8_t Command_Buff[] = {0x42,0x4d,0xe1,0x00,0x00,0x01,0x70};//����ģʽ����	
	HAL_UART_Transmit(&PM25_UART_DEV,Command_Buff,7,10000);
	
	__HAL_UART_CLEAR_FLAG(&PM25_UART_DEV,UART_FLAG_RXNE);
	HAL_UART_Receive(&PM25_UART_DEV,Pm25_Read_Uart_Buff,8,200);//�ȴ���ʽ��ȡ��

	if(Pm25_Read_Uart_Buff[0] == 0x42 && Pm25_Read_Uart_Buff[1] == 0x4d && Pm25_Read_Uart_Buff[4] == 0xe1)
	{
		return 0;
	}
	return -1;	
}

/**************************************************************************
�������ܣ� ��ʼ��PM25 Ϊ����ģʽ
**************************************************************************/
int8_t Init_Pm25(void)
{
	//HAL_GPIO_WritePin(PM_RST_GPIO_Port,PM_RST_Pin,GPIO_PIN_RESET);	
#ifdef __ATMOSPHERE_SENSOR_7IN1__
	for (uint8_t i=0; i<10; i++)
#endif
		if(Pm25_Set_Read_Mod() == 0)
			return 0;
	return -1;
}


/**************************************************************************
�������ܣ� PM25 Ϊ����ģʽ
**************************************************************************/
int8_t Standby_Pm25(void)
{
	//HAL_GPIO_WritePin(PM_RST_GPIO_Port,PM_RST_Pin,GPIO_PIN_RESET);	
	uint8_t Command_Buff[] = {0x42,0x4d,0xe4,0x00,0x00,0x01,0X73};//����ģʽ����	
	HAL_UART_Transmit(&PM25_UART_DEV,Command_Buff,7,10000);
	return 0;
}

/**************************************************************************
�������ܣ� PM25 Ϊ����ģʽ
**************************************************************************/
int8_t Work_Pm25(void)
{
	//HAL_GPIO_WritePin(PM_RST_GPIO_Port,PM_RST_Pin,GPIO_PIN_RESET);	
	uint8_t Command_Buff[] = {0x42,0x4d,0xe4,0x00,0x01,0x01,0X74};//����ģʽ����	
	HAL_UART_Transmit(&PM25_UART_DEV,Command_Buff,7,10000);
	return 0;
}


/**************************************************************************
�������ܣ� PM25 ���Ͷ�ȡ������������ã�
**************************************************************************/
int8_t Read_Pm25(void)
{
	uint8_t Command_Buff[] = {0x42,0x4d,0xe2,0x00,0x00,0x01,0x71};//����������
	HAL_UART_Transmit(&PM25_UART_DEV,Command_Buff,7,10000);
	return 0;
}



/**
* @brief ��PM25����
* @param uint8_t *buff
* 
* @retval 0�ɹ���-1ʧ��
*/
int8_t Get_Pm25(uint8_t *buff)
{
//	uint8_t Command_Buff[] = {0x42,0x4d,0xe2,0x00,0x00,0x01,0x71};//����������
	uint16_t Sum_Result;
	uint8_t i;
	
	/* ��1��������������ָ�������ȡ */
	for(i=0;i<32;i++)
		Pm25_Read_Uart_Buff[i] = 0;
	__HAL_UART_CLEAR_FEFLAG(&PM25_UART_DEV);
	__HAL_UART_CLEAR_NEFLAG(&PM25_UART_DEV);
	__HAL_UART_CLEAR_PEFLAG(&PM25_UART_DEV);
	__HAL_UART_CLEAR_OREFLAG(&PM25_UART_DEV);
	__HAL_UART_CLEAR_FLAG(&PM25_UART_DEV, UART_FLAG_TC);
//	HAL_UART_Transmit(&PM25_UART_DEV,Command_Buff,7,10000);//�ȴ���ʽ���͡�
	HAL_UART_Transmit_DMA(&PM25_UART_DEV,Pm25_DMA_Command_Buff,7);//DMA��ʽ���͡�
	
//	__HAL_UART_CLEAR_FEFLAG(&PM25_UART_DEV);
//	__HAL_UART_CLEAR_NEFLAG(&PM25_UART_DEV);
//	__HAL_UART_CLEAR_PEFLAG(&PM25_UART_DEV);
//	__HAL_UART_CLEAR_OREFLAG(&PM25_UART_DEV);
	__HAL_UART_CLEAR_FLAG(&PM25_UART_DEV,UART_FLAG_RXNE);
//	HAL_UART_Receive(&PM25_UART_DEV,PM25_Read_Uart_Buff,32,200);//�ȴ���ʽ��ȡ��
	HAL_UART_Receive_IT(&PM25_UART_DEV,Pm25_Read_Uart_Buff,32);//�ж���ʽ��ȡ��

	/* ��2�����ȴ���ȡ���� */
	osSemaphoreWait(PM25_RX_CPLHandle,200);	
	 
	/* ��3������ȡ���ݺ�У�� */
	if(Pm25_Read_Uart_Buff[0] == 0x42 && Pm25_Read_Uart_Buff[1] == 0x4d)
	{
		Sum_Result = Sum_Compute(Pm25_Read_Uart_Buff,30);
		if((Pm25_Read_Uart_Buff[31] == (Sum_Result & 0x00ff)) && (Pm25_Read_Uart_Buff[30] == (Sum_Result >> 8)))
		{
			buff[0] = Pm25_Read_Uart_Buff[6];
			buff[1] = Pm25_Read_Uart_Buff[7];
			buff[2] = Pm25_Read_Uart_Buff[8];
			buff[3] = Pm25_Read_Uart_Buff[9];
			return 0;
		}
	}
	return -1;
}








