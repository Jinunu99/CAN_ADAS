#include "handle.h"

void handle_INIT()
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

// val - 500(0도), 1500(90도), 2500(180도)
void handle_CONTROL(uint16_t val)
{
	TIM1->CCR1 = val;
}
