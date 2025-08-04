#ifndef INC_CAR_H_
#define INC_CAR_H_

#include "main.h"

/*
car)
	ultra, ir, motor speed, direction
*/
typedef enum
{
	LEFT,
	RIGHT,
	STRAIGHT,
	BACK
} DIRECTION;

typedef struct
{
	uint8_t ultra[4];
	uint8_t ir[2];
	uint8_t motor_speed;
	DIRECTION direction;
} CAR;


void car_INIT();
void speed_SET();
void direction_SET();

#endif /* INC_CAR_H_ */
