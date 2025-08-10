#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "tim.h"
#include "main.h"

void motor_INIT();
void motor_front(uint16_t speed);
void motor_left(uint16_t speed);
void motor_right(uint16_t speed);
void motor_rear(uint16_t speed);
//void motor_speed(uint16_t speed);

#endif /* INC_MOTOR_H_ */
