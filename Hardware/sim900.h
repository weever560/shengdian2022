/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIM900A_H__
#define __SIM900A_H__
 
#ifdef __cplusplus
extern "C" {
#endif
 
/* Includes ------------------------------------------------------------------*/
#include "main.h"
 
/* USER CODE BEGIN Includes */
 
/* USER CODE END Includes */
 
/* USER CODE BEGIN Private defines */
 
/* USER CODE END Private defines */
//u8 sim900a_send_cmd(u8 *cmd, u8 *ack, u16 waittime);
//u8* sim900a_check_cmd(u8 *str);
//u8 sim900a_check_status();
//u8 sim900a_sms_test(u8* msisdn);
 
/* USER CODE BEGIN Prototypes */
	
void Sim900A_SendAtCmd(char* pCmd);//∑¢ÀÕ÷∏¡Ó
void Sim900A_SendAT(void);
void Sim900A_SendMsg1(void);
void Sim900A_SendMsg2(char *message,char *phonenumber);
void ASCII_TO_Unicode(char *ASCII,char *Unicode);
void sim900a_send_English_message(char *message,char *phonenumber);
void sim900a_send_Chinese_message(char *message,char *phonenumber);
 
/* USER CODE END Prototypes */
 
#ifdef __cplusplus
}
#endif
#endif /*__ SIM900A_H__ */
 