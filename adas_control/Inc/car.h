#ifndef INC_CAR_H_
#define INC_CAR_H_

#include "main.h"

/*
car)
	ultra, ir, motor speed, direction, buzzer(flag)
*/
typedef enum
{
	LEFT,
	RIGHT,
	STRAIGHT,
	BACK
} DIRECTION;

typedef enum
{
	ON,
	OFF
}BUZZER;

typedef struct
{
	uint8_t ultra[4];
	uint8_t ir[2];
	int motor_speed;
	DIRECTION direction;
	BUZZER aeb_buzzer;
	BUZZER ldws_buzzer;
	BUZZER bcw_buzzer;
} CAR;


void car_INIT();
void speed_SET();
void direction_SET();

#endif /* INC_CAR_H_ */
