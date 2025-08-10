#include "buzz.h"

void buzzer_INIT()
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

// 부저 음 세팅 (304는 음계 미로 설정하는 것)
void buzzer_set()
{
	TIM2->PSC = 304;
}

void buzzer_on()
{
	TIM2->CCR1 = 500; // buz on
}

void buzzer_off()
{
	TIM2->CCR1 = 0;   // buz off
}
