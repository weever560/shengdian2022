#include "mq135.h"
#include "stm32f1xx_hal.h"
extern ADC_HandleTypeDef hadc2;
extern float ppm;
void MQ135_Task(void)
{
		//空气质量	
		HAL_ADC_Start(&hadc2);
		float temp;
		int Val;
		HAL_ADC_PollForConversion(&hadc2, 50);
        
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc2),HAL_ADC_STATE_REG_EOC))
    {
        Val = HAL_ADC_GetValue(&hadc2);
    }
    temp = (float)Val*3.3 / 4096;
  	ppm = pow((3.4880*10*temp)/(5-temp),(1.0/0.3203));

		
		//***********测试用
//		uint8_t temp1[10];
//		sprintf(temp1,"%f\r\n",ppm);
//    HAL_UART_Transmit(&huart1,temp1,sizeof(temp1),100);		
//        OLED_ShowString(0,2,&Font_8x16,"ADC:%02d",Val);
//        OLED_ShowString(0,4,&Font_8x16,"VOL:%02f",temp);
//        
    HAL_Delay(500);
}



