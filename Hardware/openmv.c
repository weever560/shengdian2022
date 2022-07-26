#include "openmv.h"
#include "stdio.h"
#include "usart.h"
#include "main.h"
#include "stdio.h"
#include "stdint.h"
/*�ĸ��������ڴ��Ŀ����������������Լ���ȣ��߶�*/
static uint8_t  Cx=0,Cy=0,Cw=0,Ch=0;
uint8_t RoG=0;	//RoG=1Ϊʶ�𵽺��
uint8_t RoomNum, TargetNum;
uint8_t sendBuf[4];
uint8_t TASK=1;
/*���ݽ��պ���*/
void SendDataToOpenmv(void)
{
	uint8_t i;
	//���Ϸ��͸�openmv �����ݵĴ��� (֡ͷ�� ģ��ƥ��ģʽѡ���־λ��ģʽ2����Ҫƥ������֣�֡β)   //����Ҫ�ܸߵķ���Ƶ��
		
		
		for(i = 0; i <= 4; i++)   //��TASK��TargetNum���һ���Է��͸�openmv
		{
			sprintf((char *)sendBuf, "*%d%d&", TASK, TargetNum);    
				HAL_UART_Transmit(&huart4, sendBuf, sizeof(sendBuf), 1000);
			 // greenLED_on;
		}
		//greenLED_off;
}

void Openmv_Receive_Data(int16_t Com_Data)
{
  /*ѭ�������*/
  uint8_t i;
	/*��������*/
	static uint8_t RxCounter1=0;//����
	/*���ݽ�������*/
	static uint16_t RxBuffer1[10]={0};
	/*���ݴ���״̬λ*/
	static uint8_t RxState = 0;	
	/*�����ݽ���У׼���ж��Ƿ�Ϊ��Ч����*/
  if(RxState==0&&Com_Data==0x2C)  //0x2c֡ͷ
		{
          
			RxState=1;
			RxBuffer1[RxCounter1++]=Com_Data;
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
		}

	else if(RxState==1&&Com_Data==0x12)  //0x12֡ͷ
		{
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			RxState=2;
			RxBuffer1[RxCounter1++]=Com_Data;
		}
	else if(RxState==2)
		{
           
			RxBuffer1[RxCounter1++]=Com_Data;
			if(RxCounter1>=11||Com_Data == 0x5B)       //RxBuffer1��������,�������ݽ���
				{
					RxState=3;
					Cx=RxBuffer1[RxCounter1-6];
					Cy=RxBuffer1[RxCounter1-5];
					Cw=RxBuffer1[RxCounter1-4];
					Ch=RxBuffer1[RxCounter1-3];
					RoG=RxBuffer1[RxCounter1-2];
          printf("%d\r   ",Cx);
          printf("%d\r   ",Cy);
          printf("%d\r   ",Cw);
          printf("%d\r	 ",Ch); 
					printf("%d\r\n",RoG); 
					}
			}
		
				else if(RxState==3)//����Ƿ���ܵ�������־
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									//RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
						}
						else   //���մ���
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<10;i++)
									{
											RxBuffer1[i]=0x00;      //�����������������
									}
						}
				} 
	
				else   //�����쳣
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<10;i++)
						{
								RxBuffer1[i]=0x00;      //�����������������
						}
				}
      }
