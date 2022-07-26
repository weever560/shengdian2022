#include "sim900.h"
#include "usart.h"
#include "string.h"
#include <stdio.h>
 
extern uint8_t Uart1_Rx_Cnt;
extern char RxBuffer2[256];

extern uint8_t USART1_RXbuff[15];
char dispbuf[64];
 
uint8_t ByteSend=0x1A;//���ŷ���

char unicode_phone[48]={0};

/**********************************************************************
����: ASCII ת unicode      ���� '1'  ת�� "0031"
***********************************************************************/
void ASCII_TO_Unicode(char *ASCII,char *Unicode)
{
    int length;
    int i = 0;
    int j = 0;
		memset(Unicode,'\0',sizeof(Unicode));
    length = strlen(ASCII);
 
    for(i=0;i<length;i++)
    {
        Unicode[j++] = '0';
        Unicode[j++] = '0';
 
        Unicode[j++] = (ASCII[i] / 16) + 0x30;
        Unicode[j++] = (ASCII[i] % 16) + 0x30;
    }
 
}


//����ָ��
void Sim900A_SendAtCmd(char* pCmd)
{
	char c;
	while(*pCmd!='\0')
	{
		c = *pCmd;
		HAL_UART_Transmit(&huart1, (uint8_t *)&c, 1,0xFFFF);
		pCmd++;
	}
}
 
//����ָ��
void Sim900A_SendAT(void)
{
	Sim900A_SendAtCmd("AT+CPIN?\r\n");//��ѯ SIM ����״̬
	HAL_Delay(1000);
	Sim900A_SendAtCmd("AT+CSQ\r\n");//��ѯ�ź�����
	HAL_Delay(1000);
	Sim900A_SendAtCmd("AT+COPS?\r\n");//��ѯ��ǰ��Ӫ��
	HAL_Delay(1000);
}
 
//����绰
void Sim900A_SendMsg1(void)
{
	Sim900A_SendAtCmd("ATD+\"*********\";\r\n");//�绰�����unicode��
	HAL_Delay(1000);
}
 
//����������ţ�����ʵ�ְ档
void Sim900A_SendMsg2(char *message,char *phonenumber)
{
	Sim900A_SendAtCmd("AT+CMGF=1\r\n");//����Ϊ�ı�ģʽ
	HAL_Delay(1000);
	Sim900A_SendAtCmd("AT+CSMP=17,167,0,8\r\n");//�����ı�ģʽ����
	HAL_Delay(1000);
	Sim900A_SendAtCmd("AT+CSCS=\"UCS2\"\r\n");//����Ϊ UCS2 �����ַ���
	HAL_Delay(1000);
	
	ASCII_TO_Unicode(phonenumber,unicode_phone);
	sprintf(dispbuf,"AT+CMGS=\"%s\"\r\n",unicode_phone);
	Sim900A_SendAtCmd(dispbuf);
	memset(unicode_phone,'\0',sizeof(unicode_phone));
	memset(dispbuf,'\0',sizeof(dispbuf));
//	Sim900A_SendAtCmd("AT+CMGS=\"00310033003000320032003000340035003400320037\"\r\n");//�绰�����unicode��
	HAL_Delay(1000);
	
	Sim900A_SendAtCmd(message);
//	Sim900A_SendAtCmd("30104E0953EA5C0F732A30114EB27231768475286237FF0C68C06D4B523060A876849A7E9A76884C4E3A5B5857285B89516898CE9669FF0C4E3A4E8660A8548C4ED64EBA76845E78798FFF0C8BF76CE8610F4EA4901A5B8951683002795D60A84E008DEF5E735B89FF01\r\n");//���Ͷ�������unicode��
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart1, &ByteSend, 1,0xFFFF);
}

