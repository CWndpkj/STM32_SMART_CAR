//
// Created by PKJ on 2024/6/25.
//

#include "../Inc/control.h"
#include "motor.h"
#include <stdio.h>

//左转状态
//bit 0:左转任务是否存在
//bit [2:1]:左转任务的完成状态,00代表未开始执行,01代表正在左转,10代表已完成左转
uint8_t turn_left_flag;
//右转状态
//bit位状态同左转状态
uint8_t turn_right_flag;

void control_tick() {
    uint8_t patrol_sensor_data = get_patrol_data();
    printf("patrol_data:%d\r\n", patrol_sensor_data);
    if (patrol_sensor_data == 5) {
        //小车正对线，无需调整
        turn_left_flag = 0;
        turn_right_flag = 0;
    } else if (patrol_sensor_data == 7) {
        //向左小幅调节
        printf("turn left little\r\n");
    } else if (patrol_sensor_data == 13) {
        //向右小幅调节
        printf("turn right little\r\n");
    } else if (patrol_sensor_data == 11 || patrol_sensor_data == 3) {
        //向左大幅调节
        printf("turn left a lot\r\n");
    } else if (patrol_sensor_data == 14 || patrol_sensor_data == 12) {
        //向右大幅调节
        printf("turn right a lot\r\n");
    } else if (patrol_sensor_data == 15) {
        //未识别到线，原地旋转
        printf("turn around\r\n");
    } else if (patrol_sensor_data == 1) {
        //识别到左直角
        printf("turn left 90 degree\r\n");
        turn_left_flag |= 0x1;
    } else if (patrol_sensor_data == 4) {
        //识别到右直角
        printf("turn right 90 degree\r\n");
        turn_right_flag |= 0x1;
    }
}
