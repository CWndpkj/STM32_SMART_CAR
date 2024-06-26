//
// Created by PKJ on 2024/6/25.
//
#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "hc_sr04.h"
#include "beeper.h"
#include "control.h"
#include "main.h"
#include "motor.h"
extern hc_sr04_dev_t hc_sr04_dev;
extern TIM_HandleTypeDef htim2;

static uint16_t beeper_declipse = 2000;

//定时器2中断，可用于按键扫描，固定间隔时间执行某个任务等
//note:可能包含执行时间较长的函数，不可用于对时间精度要求较高的函数
void schedule_start() {
    HAL_TIM_Base_Start_IT(&htim2);
}

void schedule_stop() {
    HAL_TIM_Base_Stop_IT(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        //超声波传感器定时执行任务
        if (hc_sr04_dev.mesuring == 1) {
            hc_sr04_dev.next_mesure--;
            if (hc_sr04_dev.next_mesure == 0) {
                hc_sr04_dev.next_mesure = hc_sr04_dev.declipse;
                hc_sr04_trig();
            }
        }

        //Beeper鸣叫状态切换
        if (beeper_dev.timestamp_list_len != 0) {
            if (beeper_dev.timestamp_list[beeper_dev.timestamp_list_len - 1].declipse) {
                beeper_dev.timestamp_list[beeper_dev.timestamp_list_len - 1].declipse--;
            } else {
                beeper_dev.timestamp_list_len -= 1;
                //当没有响应需要处理时，关闭蜂鸣器
                if (beeper_dev.timestamp_list_len == 0) {
                    _beeper_off();
                } else {
                    if (beeper_dev.timestamp_list[beeper_dev.timestamp_list_len - 1].status) {
                        _beeper_on();
                    } else {
                        _beeper_off();
                    }
                }
            }
        }

        //电机旋转状态切换
        if (motor_dev.timestamp_list_len != 0) {
            if (motor_dev.timestamp_list[motor_dev.timestamp_list_len - 1].declipse) {
                motor_dev.timestamp_list[motor_dev.timestamp_list_len - 1].declipse--;
            } else {
                motor_dev.timestamp_list_len -= 1;
                //当没有响应需要处理时，关闭电机
                if (motor_dev.timestamp_list_len == 0) {
                    motor_break();
                } else {
                    //否则载入下一次的转速
                    motor_ctl_left(motor_dev.timestamp_list[motor_dev.timestamp_list_len - 1].left_speed);
                    motor_ctl_right(motor_dev.timestamp_list[motor_dev.timestamp_list_len - 1].right_speed);
                }
            }
        }

        // Beeper鸣叫任务
        beeper_declipse--;
        if (beeper_declipse == 0) {
            //TODO:调节参数10为合适的数值
            beeper_declipse = 8 * hc_sr04_dev.distance;
            beeper_beep(SHORT);
        }

        //小车运动状态切换
        if (turn_left_flag) {
            if (!(turn_left_flag & 0x6)) {
                //载入左转指令
                motor_turn_angle(-90);
            }
        } else if (turn_right_flag) {
            if (!(turn_right_flag & 0x6)) {
                //载入右转指令
                motor_turn_angle(90);
            }
        }
    }
}
