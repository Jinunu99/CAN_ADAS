#include "led.h"

//void led_INIT()
//{
//	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
//	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
//	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
//}


//void led_SET(uint8_t r, uint8_t g, uint8_t b)
//{
//	// 8bit(0~255) → PWM 범위(예: 0~999)로 변환
//	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (r * 999) / 255); // Red
//	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, (g * 999) / 255); // Green
//	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, (b * 999) / 255); // Blue
//}
