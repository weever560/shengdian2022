#include "syn6288.h"
#include "stdio.h"
#include "main.h"
#include "usart.h"
#include "string.h"
/***********************************************************
* 名    称： YS-SYN6288 文本合成函数
UART_WORDLENGTH_8B!!串口数据为8位
UART_PARITY_NONE    无校验位
SYN6288语音模块
//!!!!!!!!!!!!!!!!波特率必须9600!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
**********************************************************/

void SYN_FrameInfo(uint8_t *Voice)   //语音播报函数
{
		//  uint8_t   Voice[] ="[v1][t5]酒精";			
		//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
		//m[0~16]:0背景音乐为静音，16背景音乐音量最大
		//v[0~16]:0朗读音量为静音，16朗读音量最大
		//t[0~5]:0朗读语速最慢，5朗读语速最快
	unsigned  char  HZ_Length = strlen((char*)Voice); 	//长度由sizeof求
	unsigned  char  ecc  = 0;  			//定义校验字节
	unsigned  int i=0; 
	unsigned  char  Frame_Info[5]={0xfd,0x00,10+3,0x01,0x00}; 
   Frame_Info[2]=3+HZ_Length;//关于语音长度
	/*******************校验码计算***************************************/		 
	for(i = 0; i<5; i++)   			//依次发送构造好的5个帧头字节
	{  
		ecc=ecc^(Frame_Info[i]);		//异或校验	
	}
	
	for(i= 0; i<HZ_Length; i++)   		//依次发送待合成的文本数据
	{  
		ecc=ecc^(Voice[i]); 				//异或校验		
	}
	//不能在近期用串口！！HAL库先发送5个，再发送文字，再改善ecc(校验位)
	 HAL_UART_Transmit(&huart5, (uint8_t *)Frame_Info,5, 0xFFFF);
   HAL_UART_Transmit(&huart5,                 Voice, HZ_Length, 0xFFFF);
	 HAL_UART_Transmit(&huart5, (uint8_t *)&ecc,1, 0xFFFF);
}
//void SYN_FrameInfo2(void)
//{
///****************需要发送的文本**********************************/
//  uint8_t   Voice[] ="你呢";
//	unsigned  char  HZ_Length=sizeof(Voice);
//	unsigned  char  ecc  = 0;  			//定义校验字节
//	unsigned  int i=0; 
//	unsigned  char  Frame_Info[5]={0xfd,0x00,10+3,0x01,0x00};         
//  Frame_Info[2]=3+HZ_Length;
//	/*******************校验码计算***************************************/		 
//	for(i = 0; i<5; i++)   			//依次发送构造好的5个帧头字节
//	{  
//		ecc=ecc^(Frame_Info[i]);		//对发送的字节进行异或校验	
//	}
//	
//	for(i= 0; i<HZ_Length; i++)   		//依次发送待合成的文本数据
//	{  
//		ecc=ecc^(Voice[i]); 				//对发送的字节进行异或校验		
//	}
//	 //不能在近期用串口！！
//	 HAL_UART_Transmit(&huart5, (uint8_t *)Frame_Info,5, 0xFFFF);
//   HAL_UART_Transmit(&huart5,                 Voice, HZ_Length, 0xFFFF);
//	 HAL_UART_Transmit(&huart5, (uint8_t *)&ecc,1, 0xFFFF);
//}



