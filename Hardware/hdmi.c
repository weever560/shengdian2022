#include "hdmi.h"
#include "usart.h"

uint8_t ByteSend_hdmi=0xff;	//œ‘ æ∆¡∑¢ÀÕ
void SendEnd(){

	HAL_UART_Transmit(&huart2, &ByteSend_hdmi, 1,0xFFFF);
	HAL_UART_Transmit(&huart2, &ByteSend_hdmi, 1,0xFFFF);
	HAL_UART_Transmit(&huart2, &ByteSend_hdmi, 1,0xFFFF);
}

void Hdmi_SendAtCmd(char* pCmd)
{
	char c;
	while(*pCmd!='\0')
	{
		c = *pCmd;
		HAL_UART_Transmit(&huart2, (uint8_t *)&c, 1,0xFFFF);
		pCmd++;
	}
}