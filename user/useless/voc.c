#include "voc.h"
#include "usart.h"
#include "cmsis_os.h"


extern osSemaphoreId VOC_RX_CPLHandle;//���յ���������֡��ͬ���Ķ�ֵ�ź���

uint8_t Voc_Data_Buff[6];//������ݽӿ�ȫ�ֱ���
uint8_t Voc_Read_Uart_Buff[20];//���ڶ�ȡ���ݻ���
uint8_t Voc_DMA_Command_Buff[9] = {0x42,0x86,0x04,0x00,0x00,0x00,0x00,0x00,0xff};//����������;


/**************************************************************************
�������ܣ�CRC У�麯�������� CRC
����˵����arr_buff����ҪУ������鼯
len����ҪУ�����ݵĳ���
���ز�����CRC Ϊ unsigned int ���ͣ���λ�ֽ�Ϊ��λ��ǰ����λ�ں�
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
�������ܣ� ����VOCΪ����ģʽ
**************************************************************************/
int8_t Voc_Set_Read_Mod(void)
{
	__HAL_UART_CLEAR_FEFLAG(&VOC_UART_DEV);
	__HAL_UART_CLEAR_NEFLAG(&VOC_UART_DEV);
	__HAL_UART_CLEAR_PEFLAG(&VOC_UART_DEV);
	__HAL_UART_CLEAR_OREFLAG(&VOC_UART_DEV);
	__HAL_UART_CLEAR_FLAG(&VOC_UART_DEV, UART_FLAG_TC);
	uint8_t Command_Buff[] = {0x42,0x78,0x04,0x00,0x00,0x00,0x00,0x00,0xff};//����ģʽ����	
	HAL_UART_Transmit(&VOC_UART_DEV,Command_Buff,9,10000);
	
	__HAL_UART_CLEAR_FLAG(&VOC_UART_DEV,UART_FLAG_RXNE);
	HAL_UART_Receive(&VOC_UART_DEV,Voc_Read_Uart_Buff,9,300);//�ȴ���ʽ��ȡ��
	
	if(Voc_Read_Uart_Buff[0] == 0x42 && Voc_Read_Uart_Buff[1] == 0x78 && Voc_Read_Uart_Buff[2] == 0x01)
	{
		return 0;
	}
	return -1;
}

/**************************************************************************
�������ܣ� ��ʼ��VOC
**************************************************************************/
int8_t Init_Voc(void)
{
	uint8_t i;
	for (i=0;i<10;i++)
	{
		if(Voc_Set_Read_Mod() >= 0)
			return 0;
	}
	return -1;
}


/**
* @brief ��VOC
* @param 
* 
* @retval 0�ɹ���-1ʧ��
*/
int8_t Get_Voc(uint8_t *buff)
{
//	uint8_t Command_Buff[] = {0x42,0x86,0x04,0x00,0x00,0x00,0x00,0x00,0xff};//����������
	uint16_t CRC_Result;
	uint8_t i;
	
	/* ��1��������������ָ�������ȡ */
	for(i=0;i<17;i++)
		Voc_Read_Uart_Buff[i] = 0;
	__HAL_UART_CLEAR_FEFLAG(&VOC_UART_DEV);
	__HAL_UART_CLEAR_NEFLAG(&VOC_UART_DEV);
	__HAL_UART_CLEAR_PEFLAG(&VOC_UART_DEV);
	__HAL_UART_CLEAR_OREFLAG(&VOC_UART_DEV);
	__HAL_UART_CLEAR_FLAG(&VOC_UART_DEV, UART_FLAG_TC);
//	HAL_UART_Transmit(&VOC_UART_DEV,Command_Buff,9,10000);//�ȴ���ʽ���͡�
	HAL_UART_Transmit_DMA(&VOC_UART_DEV,Voc_DMA_Command_Buff,9);//DMA��ʽ���͡�
	
//	__HAL_UART_CLEAR_FEFLAG(&VOC_UART_DEV);
//	__HAL_UART_CLEAR_NEFLAG(&VOC_UART_DEV);
//	__HAL_UART_CLEAR_PEFLAG(&VOC_UART_DEV);
//	__HAL_UART_CLEAR_OREFLAG(&VOC_UART_DEV);
	__HAL_UART_CLEAR_FLAG(&VOC_UART_DEV,UART_FLAG_RXNE);
//	HAL_UART_Receive(&VOC_UART_DEV,Voc_Read_Uart_Buff,17,200);//�ȴ���ʽ��ȡ��
	HAL_UART_Receive_IT(&VOC_UART_DEV,Voc_Read_Uart_Buff,17);//�ж���ʽ��ȡ��

	/* ��2�����ȴ���ȡ���� */
	osSemaphoreWait(VOC_RX_CPLHandle,200);	
	 
	/* ��3������ȡ���ݺ�У�� */
	if(Voc_Read_Uart_Buff[0] == 0x42 && Voc_Read_Uart_Buff[1] == 0x4d)
	{
		CRC_Result = CRC_Compute( & Voc_Read_Uart_Buff[2],13);
		if((Voc_Read_Uart_Buff[15] == (CRC_Result & 0x00ff)) && (Voc_Read_Uart_Buff[16] == (CRC_Result >> 8)))
		{
			buff[0] = Voc_Read_Uart_Buff[9];
			buff[1] = Voc_Read_Uart_Buff[10];
			buff[2] = Voc_Read_Uart_Buff[11];
			buff[3] = Voc_Read_Uart_Buff[12];
			buff[4] = Voc_Read_Uart_Buff[13];
			buff[5] = Voc_Read_Uart_Buff[14];
			return 0;
		}
	}
	return -1;
}








