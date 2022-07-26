#include "syn6288.h"
#include "stdio.h"
#include "main.h"
#include "usart.h"
#include "string.h"
/***********************************************************
* ��    �ƣ� YS-SYN6288 �ı��ϳɺ���
UART_WORDLENGTH_8B!!��������Ϊ8λ
UART_PARITY_NONE    ��У��λ
SYN6288����ģ��
//!!!!!!!!!!!!!!!!�����ʱ���9600!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
**********************************************************/

void SYN_FrameInfo(uint8_t *Voice)   //������������
{
		//  uint8_t   Voice[] ="[v1][t5]�ƾ�";			
		//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
		//m[0~16]:0��������Ϊ������16���������������
		//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
		//t[0~5]:0�ʶ�����������5�ʶ��������
	unsigned  char  HZ_Length = strlen((char*)Voice); 	//������sizeof��
	unsigned  char  ecc  = 0;  			//����У���ֽ�
	unsigned  int i=0; 
	unsigned  char  Frame_Info[5]={0xfd,0x00,10+3,0x01,0x00}; 
   Frame_Info[2]=3+HZ_Length;//������������
	/*******************У�������***************************************/		 
	for(i = 0; i<5; i++)   			//���η��͹���õ�5��֡ͷ�ֽ�
	{  
		ecc=ecc^(Frame_Info[i]);		//���У��	
	}
	
	for(i= 0; i<HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
	{  
		ecc=ecc^(Voice[i]); 				//���У��		
	}
	//�����ڽ����ô��ڣ���HAL���ȷ���5�����ٷ������֣��ٸ���ecc(У��λ)
	 HAL_UART_Transmit(&huart5, (uint8_t *)Frame_Info,5, 0xFFFF);
   HAL_UART_Transmit(&huart5,                 Voice, HZ_Length, 0xFFFF);
	 HAL_UART_Transmit(&huart5, (uint8_t *)&ecc,1, 0xFFFF);
}
//void SYN_FrameInfo2(void)
//{
///****************��Ҫ���͵��ı�**********************************/
//  uint8_t   Voice[] ="����";
//	unsigned  char  HZ_Length=sizeof(Voice);
//	unsigned  char  ecc  = 0;  			//����У���ֽ�
//	unsigned  int i=0; 
//	unsigned  char  Frame_Info[5]={0xfd,0x00,10+3,0x01,0x00};         
//  Frame_Info[2]=3+HZ_Length;
//	/*******************У�������***************************************/		 
//	for(i = 0; i<5; i++)   			//���η��͹���õ�5��֡ͷ�ֽ�
//	{  
//		ecc=ecc^(Frame_Info[i]);		//�Է��͵��ֽڽ������У��	
//	}
//	
//	for(i= 0; i<HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
//	{  
//		ecc=ecc^(Voice[i]); 				//�Է��͵��ֽڽ������У��		
//	}
//	 //�����ڽ����ô��ڣ���
//	 HAL_UART_Transmit(&huart5, (uint8_t *)Frame_Info,5, 0xFFFF);
//   HAL_UART_Transmit(&huart5,                 Voice, HZ_Length, 0xFFFF);
//	 HAL_UART_Transmit(&huart5, (uint8_t *)&ecc,1, 0xFFFF);
//}



