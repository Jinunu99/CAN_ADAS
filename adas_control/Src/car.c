#include "car.h"

CAR car;

void car_INIT()
{
	memset(car.ultra, 0, sizeof(car.ultra));
	memset(car.ir, 0, sizeof(car.ir));
	car.motor_speed = 0;
	car.direction = STRAIGHT;
	car.aeb_buzzer = OFF;
	car.ldws_buzzer = OFF;
	car.bcw_buzzer = OFF;
}

// 기본 주행값 450
// 최저 pwm 240
// 모터 속도 설정
void speed_SET()
{
	uint8_t ULTRA_front = car.ultra[0];

	// ACC (Adaptive Cruise Control)
	if(ULTRA_front >= 17)
	{
		car.aeb_buzzer = OFF;
		car.motor_speed = 500;
	}
	else if(ULTRA_front >= 12 && ULTRA_front < 17)
	{
		car.aeb_buzzer = OFF;
		car.motor_speed = (ULTRA_front - 12) * 40 + 300;
	}
	// AEB (자동 긴급 제동)
	else
	{
		car.aeb_buzzer = ON;
		car.motor_speed = 0;
	}
}

// 차 방향 설정
void direction_SET()
{
	// IR 값이 1이면 검은색 라인, 0이면 흰색
	uint8_t IR_l = car.ir[0];
	uint8_t IR_r = car.ir[1];

	if(IR_l == 1 && IR_r == 1)
	{
		// 직진
		car.ldws_buzzer = OFF;
		car.direction = STRAIGHT;
	}
	else if(IR_l == 1 && IR_r == 0)
	{
		// 왼쪽으로 회전
		car.ldws_buzzer = OFF;
		car.direction = LEFT;
	}
	else if(IR_l == 0 && IR_r == 1)
	{
		// 오른쪽으로 회전
		car.ldws_buzzer = OFF;
		car.direction = RIGHT;
	}
	else
	{
		// 후진 (LDWS - 차선 이탈 경고)
		car.ldws_buzzer = ON;
		car.direction = BACK;
	}
}

// BCW (후측방 충돌 경고)
void BCW_SET()
{
	uint8_t ULTRA_left = car.ultra[1];
	uint8_t ULTRA_back = car.ultra[2];
	uint8_t ULTRA_right = car.ultra[3];

	// 일정거리 이하에 물체가 감지되면 부저
	if(ULTRA_left < 7 || ULTRA_back < 7 || ULTRA_right < 7)
		car.bcw_buzzer = ON;
	else
		car.bcw_buzzer = OFF;
}

