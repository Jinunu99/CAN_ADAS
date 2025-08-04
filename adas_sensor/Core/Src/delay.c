#include "delay.h"

// delay_us를 만드는 함수
void delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while((__HAL_TIM_GET_COUNTER(&htim2)) < us);
}
