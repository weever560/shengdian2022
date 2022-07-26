#ifndef __MQ135_H
#define __MQ135_H

#include "stdint.h"
#include "math.h"
#include "usart.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>

extern uint16_t ADCConvertedValue[10];

extern void MQ135_Task(void);
extern float GetMQ135Out(void);


#endif
