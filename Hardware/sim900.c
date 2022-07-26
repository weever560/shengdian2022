#include "sim900.h"
#include "usart.h"
#include "string.h"
#include <stdio.h>
 
extern uint8_t Uart1_Rx_Cnt;
extern char RxBuffer2[256];

extern uint8_t USART1_RXbuff[15];
char dispbuf[64];
 
uint8_t ByteSend=0x1A;//短信发送

char unicode_phone[48]={0};

/**********************************************************************
描述: ASCII 转 unicode      比如 '1'  转成 "0031"
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


//发送指令
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
 
//常规指令
void Sim900A_SendAT(void)
{
	Sim900A_SendAtCmd("AT+CPIN?\r\n");//查询 SIM 卡的状态
	HAL_Delay(1000);
	Sim900A_SendAtCmd("AT+CSQ\r\n");//查询信号质量
	HAL_Delay(1000);
	Sim900A_SendAtCmd("AT+COPS?\r\n");//查询当前运营商
	HAL_Delay(1000);
}
 
//拨打电话
void Sim900A_SendMsg1(void)
{
	Sim900A_SendAtCmd("ATD+\"*********\";\r\n");//电话号码的unicode码
	HAL_Delay(1000);
}
 
//发送中午短信，功能实现版。
void Sim900A_SendMsg2(char *message,char *phonenumber)
{
	Sim900A_SendAtCmd("AT+CMGF=1\r\n");//设置为文本模式
	HAL_Delay(1000);
	Sim900A_SendAtCmd("AT+CSMP=17,167,0,8\r\n");//设置文本模式参数
	HAL_Delay(1000);
	Sim900A_SendAtCmd("AT+CSCS=\"UCS2\"\r\n");//设置为 UCS2 编码字符集
	HAL_Delay(1000);
	
	ASCII_TO_Unicode(phonenumber,unicode_phone);
	sprintf(dispbuf,"AT+CMGS=\"%s\"\r\n",unicode_phone);
	Sim900A_SendAtCmd(dispbuf);
	memset(unicode_phone,'\0',sizeof(unicode_phone));
	memset(dispbuf,'\0',sizeof(dispbuf));
//	Sim900A_SendAtCmd("AT+CMGS=\"00310033003000320032003000340035003400320037\"\r\n");//电话号码的unicode码
	HAL_Delay(1000);
	
	Sim900A_SendAtCmd(message);
//	Sim900A_SendAtCmd("30104E0953EA5C0F732A30114EB27231768475286237FF0C68C06D4B523060A876849A7E9A76884C4E3A5B5857285B89516898CE9669FF0C4E3A4E8660A8548C4ED64EBA76845E78798FFF0C8BF76CE8610F4EA4901A5B8951683002795D60A84E008DEF5E735B89FF01\r\n");//发送短信内容unicode码
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart1, &ByteSend, 1,0xFFFF);
}

