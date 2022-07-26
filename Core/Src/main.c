/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "syn6288.h"
#include "openmv.h"
#include "mq135.h"
#include "sim900.h"
#include "hdmi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//短信发送的消息内容,危险驾驶行为
char msg[]="30104E0953EA5C0F732A30114EB27231768475286237FF0C68C06D4B523060A876849A7E9A76884C4E3A5B5857285B89516898CE9669FF0C4E3A4E8660A8548C4ED64EBA76845E78798FFF0C8BF76CE8610F4EA4901A5B8951683002795D60A84E008DEF5E735B89FF01\r\n";

float temperate=0;
float ppm;
extern  uint8_t RoG;
uint16_t time=0;
uint8_t USART1_RXbuff[10];
uint8_t USART2_RXbuff[5];
uint8_t USART3_RXbuff[10];
uint8_t warn[10];
uint8_t warn_flag=0;
uint8_t USART4_RXbuff;  // opennmv接收缓冲区
uint8_t if_alcohol;     //是否酒驾标志位
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC2_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
//	HAL_UART_Receive_IT(&huart1,USART1_RXbuff,2);
	HAL_UART_Receive_IT(&huart2,USART2_RXbuff,1);
	HAL_UART_Receive_IT(&huart3,USART3_RXbuff,4);
	HAL_UART_Receive_IT(&huart4,(void *)&USART4_RXbuff,1);	
	
	//************串口测试
	char hdmi_n0msg[20];
	int ppm1;
	int time_flag=0;
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,0);//开灯
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//检测空气质量（卢）
		MQ135_Task();	
		
		//更新屏幕数据，n0——ppm
		ppm1=ppm*10;
		sprintf(hdmi_n0msg,"x0.val=%d",ppm1);
		Hdmi_SendAtCmd(hdmi_n0msg);
		SendEnd();
		memset(hdmi_n0msg,'\0',sizeof(hdmi_n0msg));
		
		//**************酒精检测
		//检测酒精，如果酒精超标就报警＋语音（卢东）
		if(USART2_RXbuff[0]=='1')
		{
			if(time_flag==0)
			{			
				SYN_FrameInfo("[v6][t5]请在五秒内进行酒精检测"); 
				HAL_TIM_Base_Start_IT(&htim2);
				time_flag=1;
			}
			
			else if(time_flag==1 && time>=10)	//5s过去
			{
				HAL_TIM_Base_Stop_IT(&htim2);
				time_flag=0;
				memset(USART2_RXbuff,'\0',sizeof(USART2_RXbuff));
				if(if_alcohol==1)SYN_FrameInfo("[v6][t5]您已喝酒，请勿酒后开车");
				else(SYN_FrameInfo("[v6][t5]检测完毕，祝您一路顺风"));
			}
		}
		
		
		//除红灯外，检测危险行为，有则语音播报加短信提醒（卢康）
//		if(RoG==1)
//		{
//			//HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
//			RoG=0;
//			HAL_Delay(1000);
//		}
		
		if(warn[0]=='w'&&warn[1]=='a'&&warn[2]=='r'&&warn[3]=='n')
		{
			HAL_UART_Transmit(&huart4, (uint8_t *)"1",1,0xFFFF);	//开启震动
			warn_flag=1;
			memset(warn,'\0',sizeof(warn));
			HAL_UART_Transmit(&huart4, (uint8_t *)"1",1,0xFFFF);
			SYN_FrameInfo("[v6][t5]检测到危险驾驶行为，请安全驾驶");
			Sim900A_SendMsg2(msg,"13022045427");
			HAL_UART_Transmit(&huart4, (uint8_t *)"0",1,0xFFFF);	//关闭震动
			warn_flag=0;
		}
		//检测心率（东）
		
		
		
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//		HAL_ADC_Start_DMA(&hadc1,&adc_value,1);
//		//数值处理		
//		HAL_Delay(1000);
//		adc_value=adc_value>>16;
//		temperate=((float)(adc_value*3.3/4096)-0.4)*40;//得到电压值
//		printf("temperate=%.2f\r\n",temperate);		
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin==Alcohol_DO_Pin){
	 HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,0);
		if_alcohol=1;
	}
	if(GPIO_Pin==KEY_Pin){
//	 HAL_GPIO(LED_GPIO_Port,LED_Pin,0);
		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
			if_alcohol=0;
	}						//酒精传感器引脚中断
	
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  uint16_t tempt  /*定义临时变量存放接受的数据*/;
  if(huart->Instance==UART4)	//串口4是openmv
  {
    tempt=USART4_RXbuff;
    Openmv_Receive_Data(tempt);
		/*调运数据接收处理函数，每次进入中断都对数据进行理处
		，由于需要接收器个数据，因此要进入七次断理*/
		
		HAL_UART_Receive_IT(&huart4,(void *)&USART4_RXbuff,1);/*再次开启接收中断*/
  }	

	//***************卢
	
//	if(huart->Instance==USART1)	//串口2是短信
//	{
////		HAL_UART_Transmit(&huart1, (uint8_t *)USART1_RXbuff, 2,0xFFFF);
//		memset(USART1_RXbuff,'\0',sizeof(USART1_RXbuff));
//		HAL_UART_Receive_IT(&huart1,(void *)USART1_RXbuff,2);/*再次开启接收中断*/
//	}
//	
	if(huart->Instance==USART2)	//串口2是hdmi
	{
//		memset(USART2_RXbuff,'\0',sizeof(USART2_RXbuff));
		HAL_UART_Receive_IT(&huart2,USART2_RXbuff,1);/*再次开启接收中断*/
	}
	
		if(huart->Instance==USART3)	//串口3是树莓派
	{
		if(warn_flag==0)	//让警告执行完
		{
		memcpy(warn,USART3_RXbuff,sizeof(USART3_RXbuff));
		memset(USART3_RXbuff,'\0',sizeof(USART3_RXbuff));
		
		}
		HAL_UART_Receive_IT(&huart3,USART3_RXbuff,4);/*再次开启接收中断*/
	}
}																												 				

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
		if(htim->Instance == TIM2)
		{
			time++;
			if(time>10000)
				time=0;	//防止越界
		}
}
	
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

