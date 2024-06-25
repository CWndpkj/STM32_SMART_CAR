#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f1xx_hal.h"

#define FORWARD 1
#define BACKWARD 0

void LeftMotor_Control(int speed);

void RightMotor_Control(int speed);

void Break(void);

#endif // __MOTOR_H
