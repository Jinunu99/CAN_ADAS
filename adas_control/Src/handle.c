#include "handle.h"

void handle_INIT()
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

// val - 500(핸들 오른쪽), 1500(핸들 중간), 2500(핸들 왼쪽)
void handle_CONTROL(uint16_t val)
{
	TIM1->CCR1 = val;
}
