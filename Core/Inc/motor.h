#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f1xx_hal.h"

typedef struct {
    uint16_t declipse; //表示这段时间的长短
    int16_t left_speed; //表示这段时间的的左电机速度
    int16_t right_speed; //表示这段时间的的右电机速度
} motor_timestamp_t;

typedef struct {
    motor_timestamp_t timestamp_list[10];
    uint8_t timestamp_list_len;
} motor_dev_t;

extern motor_dev_t motor_dev;

void motor_ctl_left(int speed);

void motor_ctl_right(int speed);

void motor_break(void);

void motor_turn_angle(int angle);

#endif // __MOTOR_H
