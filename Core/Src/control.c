//
// Created by PKJ on 2024/6/25.
//

#include "../Inc/control.h"
#include "motor.h"
#include <stdio.h>
#include <sys/types.h>

#include "beeper.h"
#include "delay.h"
#include "hc_sr04.h"
#include "servor.h"


//超声波传感器检测到距离过近的最小距离
#define  MIN_DISTANCE 40

//巡线的速度
#define PATROL_FORWARD_SPEED 400
//自动模式的速度
#define AUTO_MODE_SPEED 500

//左转状态
//bit 0:左转任务是否存在
//bit [2:1]:左转任务的完成状态,00代表未开始执行,01代表正在左转,10代表已完成左转
uint8_t turn_left_flag;
//右转状态
//bit位状态同左转状态
uint8_t turn_right_flag;
uint8_t nec_ctl;

void set_nec_ctl(uint8_t flag) {
    nec_ctl = flag;
}

uint8_t control_decide_direction() {
    servor_set_angle(-90);
    delay_ms(800);
    uint16_t distance = hc_sr04_get_dis();
    printf("left distance:%d\r\n", distance);
    if (distance < MIN_DISTANCE) {
        servor_set_angle(90);
        delay_ms(800);
        distance = hc_sr04_get_dis();
        printf("right distance:%d\r\n", distance);
        if (distance < MIN_DISTANCE) {
            servor_set_angle(0);
            return BACKWARD;
        }
        servor_set_angle(0);
        return RIGHT;
    }
    servor_set_angle(0);
    return LEFT;
}

void control_auto_tick() {
    if (!nec_ctl) {
        uint16_t distance = hc_sr04_get_dis();
        printf("current distance:\r\n");
        if (distance < MIN_DISTANCE) {
            motor_break();
        flag :
            uint8_t direction = control_decide_direction();
            if (direction == LEFT) {
                printf("turn left\r\n");
                motor_ctl_left(0);
                motor_ctl_right(AUTO_MODE_SPEED);
                delay_ms(500);
            } else if (direction == RIGHT) {
                printf("turn right\r\n");
                motor_ctl_left(AUTO_MODE_SPEED);
                motor_ctl_right(0);
                delay_ms(500);
            } else if (direction == BACKWARD) {
                printf("backward\r\n");
                motor_ctl_right(-500);
                motor_ctl_left(500);
                delay_ms(800);
                goto flag;
            }
        } else {
            printf("forward\r\n");
            motor_ctl_left(300);
            motor_ctl_right(300);
        }
    }
}
uint8_t patrol_last_data;
void control_patrol_tick() {
    if (!nec_ctl) {
        uint8_t patrol_sensor_data = get_patrol_data();
        printf("patrol_data:%d\r\n", patrol_sensor_data);
        if (patrol_sensor_data == 5) {
            //小车正对线，无需调整
            turn_left_flag = 0;
            turn_right_flag = 0;
            motor_ctl_left(PATROL_FORWARD_SPEED);
            motor_ctl_right(PATROL_FORWARD_SPEED);
        } else if (patrol_sensor_data == 7) {
            //向左小幅调节
            printf("turn left little\r\n");
            motor_ctl_left(0);
            motor_ctl_right(PATROL_FORWARD_SPEED + 50);
        } else if (patrol_sensor_data == 13) {
            //向右小幅调节
            printf("turn right little\r\n");
            motor_ctl_left(PATROL_FORWARD_SPEED + 50);
            motor_ctl_right(0);
        } else if (patrol_sensor_data == 11 || patrol_sensor_data == 3) {
            //向左大幅调节
            printf("turn left a lot\r\n");
            motor_ctl_left(0);
            motor_ctl_right(PATROL_FORWARD_SPEED + 200);
        } else if (patrol_sensor_data == 14 || patrol_sensor_data == 12) {
            //向右大幅调节
            printf("turn right a lot\r\n");
            motor_ctl_left(PATROL_FORWARD_SPEED + 200);
            motor_ctl_right(0);
        } else if (patrol_sensor_data == 15) {
            //未识别到线，原地旋转
            printf("turn around\r\n");

        }else if (patrol_sensor_data == 8||patrol_sensor_data==4){
            motor_ctl_left(800);
            motor_ctl_right(-200);
            delay_ms(100);
        }
        else if(patrol_sensor_data == 6||patrol_sensor_data==1)
        {
            motor_ctl_left(-200);
            motor_ctl_right(800);
            // }else if (patrol_sensor_data == 1) {
            //     //识别到左直角
            //     printf("turn left 90 degree\r\n");
            //     turn_left_flag |= 0x1;
            // } else if (patrol_sensor_data == 4) {
            //     //识别到右直角
            //     printf("turn right 90 degree\r\n");
            //     turn_right_flag |= 0x1;
            // }
        }else if (patrol_sensor_data == 0)
        {
            printf("speed up\r\n");
            motor_ctl_left(300);
            motor_ctl_right(300);
        }else {
            //非法数据，刹车
            motor_break();
        }
         patrol_last_data=patrol_sensor_data;

    }

}
