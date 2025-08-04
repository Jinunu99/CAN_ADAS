#include "car.h"

CAR car;

void car_INIT()
{
	memset(car.ultra, 0, sizeof(car.ultra));
	memset(car.ir, 0, sizeof(car.ir));
	car.motor_speed = 0;
	car.direction = STRAIGHT;
}

// 모터 속도 설정
void speed_SET()
{
	uint8_t ULTRA_front = car.ultra[0];

	if(ULTRA_front >= 12)
	{
		car.motor_speed = 550;
	}
	else if(ULTRA_front >= 7 && ULTRA_front < 12)
	{
		car.motor_speed = (ULTRA_front - 7) * 50 + 300;
	}
	else
		car.motor_speed = 0;
}

// 차 방향 설정
void direction_SET()
{
	// IR 값이 1이면 검은색 라인, 0이면 흰색
	uint8_t IR_l = car.ir[0];
	uint8_t IR_r = car.ir[1];

	if(IR_l == 1 && IR_r == 1)      // 직진
		car.direction = STRAIGHT;
	else if(IR_l == 1 && IR_r == 0) // 왼쪽으로 회전
		car.direction = LEFT;
	else if(IR_l == 0 && IR_r == 1) // 오른쪽으로 회전
		car.direction = RIGHT;
	else							// 후진
		car.direction = BACK;
}

