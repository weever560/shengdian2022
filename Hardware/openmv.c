#include "openmv.h"
#include "stdio.h"
#include "usart.h"
#include "main.h"
#include "stdio.h"
#include "stdint.h"
/*四个变量用于存放目标物体的中心坐标以及宽度，高度*/
static uint8_t  Cx=0,Cy=0,Cw=0,Ch=0;
uint8_t RoG=0;	//RoG=1为识别到红灯
uint8_t RoomNum, TargetNum;
uint8_t sendBuf[4];
uint8_t TASK=1;
/*数据接收函数*/
void SendDataToOpenmv(void)
{
	uint8_t i;
	//加上发送给openmv 的数据的代码 (帧头， 模板匹配模式选择标志位，模式2所需要匹配的数字，帧尾)   //不需要很高的发送频率
		
		
		for(i = 0; i <= 4; i++)   //将TASK和TargetNum打包一次性发送给openmv
		{
			sprintf((char *)sendBuf, "*%d%d&", TASK, TargetNum);    
				HAL_UART_Transmit(&huart4, sendBuf, sizeof(sendBuf), 1000);
			 // greenLED_on;
		}
		//greenLED_off;
}

void Openmv_Receive_Data(int16_t Com_Data)
{
  /*循环体变量*/
  uint8_t i;
	/*计数变量*/
	static uint8_t RxCounter1=0;//计数
	/*数据接收数组*/
	static uint16_t RxBuffer1[10]={0};
	/*数据传输状态位*/
	static uint8_t RxState = 0;	
	/*对数据进行校准，判断是否为有效数据*/
  if(RxState==0&&Com_Data==0x2C)  //0x2c帧头
		{
          
			RxState=1;
			RxBuffer1[RxCounter1++]=Com_Data;
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
		}

	else if(RxState==1&&Com_Data==0x12)  //0x12帧头
		{
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			RxState=2;
			RxBuffer1[RxCounter1++]=Com_Data;
		}
	else if(RxState==2)
		{
           
			RxBuffer1[RxCounter1++]=Com_Data;
			if(RxCounter1>=11||Com_Data == 0x5B)       //RxBuffer1接受满了,接收数据结束
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
		
				else if(RxState==3)//检测是否接受到结束标志
				{
						if(RxBuffer1[RxCounter1-1] == 0x5B)
						{
									//RxFlag1 = 0;
									RxCounter1 = 0;
									RxState = 0;
						}
						else   //接收错误
						{
									RxState = 0;
									RxCounter1=0;
									for(i=0;i<10;i++)
									{
											RxBuffer1[i]=0x00;      //将存放数据数组清零
									}
						}
				} 
	
				else   //接收异常
				{
						RxState = 0;
						RxCounter1=0;
						for(i=0;i<10;i++)
						{
								RxBuffer1[i]=0x00;      //将存放数据数组清零
						}
				}
      }
