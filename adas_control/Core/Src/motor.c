#include "motor.h"


void motor_INIT()
{
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // 왼쪽
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); // 오른쪽
}

void motor_front(uint16_t speed)
{
	// 왼쪽 : 정방향, 오른쪽 : 정방향
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0); // IN1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1); // IN2
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0); // IN3
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1); // IN4

	TIM3->CCR1 = speed;
	TIM3->CCR2 = speed;
}

void motor_left(uint16_t speed)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0); // IN1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1); // IN2
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0); // IN3
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1); // IN4

	TIM3->CCR1 = 0;
	TIM3->CCR2 = speed;
}

void motor_right(uint16_t speed)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0); // IN1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 1); // IN2
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0); // IN3
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1); // IN4

	TIM3->CCR1 = speed;
	TIM3->CCR2 = 0;
}

void motor_rear(uint16_t speed)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1); // IN1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, 0); // IN2
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1); // IN3
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0); // IN4

	TIM3->CCR1 = speed;
	TIM3->CCR2 = speed;
}


// speed : 300 ~ 999
//void motor_speed(uint16_t speed)
//{
//	TIM3->CCR1 = speed;
//	TIM3->CCR2 = speed;
//}
