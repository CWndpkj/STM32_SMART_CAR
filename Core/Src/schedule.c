//
// Created by PKJ on 2024/6/25.
//
#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "hc_sr04.h"
#include "beeper.h"
#include "control.h"
#include "motor.h"
#include "nec.h"
extern hc_sr04_dev_t hc_sr04_dev;
extern TIM_HandleTypeDef htim2;
extern uint8_t nec_ctl;
static uint16_t beeper_declipse = 2000;
extern TIM_HandleTypeDef htim3;
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
            //否则载入下一次的转速
            motor_ctl_left(motor_dev.timestamp_list[motor_dev.timestamp_list_len - 1].left_speed);
            motor_ctl_right(motor_dev.timestamp_list[motor_dev.timestamp_list_len - 1].right_speed);
            if (motor_dev.timestamp_list[motor_dev.timestamp_list_len - 1].declipse) {
                motor_dev.timestamp_list[motor_dev.timestamp_list_len - 1].declipse--;
            } else {
                motor_dev.timestamp_list_len--;
                //当没有响应需要处理时，关闭电机
                if (motor_dev.timestamp_list_len == 0) {
                    motor_break();
                }
            }
        }

        // Beeper鸣叫任务
        // beeper_declipse--;
        // if (beeper_declipse == 0) {
        //     //TODO:调节参数10为合适的数值
        //     beeper_declipse = 8 * hc_sr04_dev.distance;
        //     beeper_beep(SHORT);
        // }

        //小车运动状态切换
        if (turn_left_flag == 0x1) {
            //载入左转指令
            motor_turn_angle(-90);
            turn_left_flag |= 0x1 << 1;
        } else if (turn_right_flag == 0x1) {
            //载入右转指令
            motor_turn_angle(90);
            turn_right_flag |= 0x1 << 1;
        }

        //nec_ctl task
        //nec状态切换
        if (nec_is_data_ready()) {
            uint8_t code = nec_get_code();
            if (code == NEC_POWER) {
                if (nec_ctl) {
                    beeper_beep(LONG);
                } else {
                    beeper_beep(SHORT);
                }
                nec_ctl = !nec_ctl;
                return;
            }
            if (nec_ctl) {
                switch (code) {
                    case NEC_FORWARD: {
                        printf("nec forward\r\n");
                        motor_ctl_left(300);
                        motor_ctl_right(300);
                        break;
                    }
                    case NEC_BACKWARD: {
                        printf("nec backward\r\n");
                        motor_ctl_left(-300);
                        motor_ctl_right(-300);
                        break;
                    }
                    case NEC_LEFT: {
                        printf("nec turn left\r\n");
                        motor_ctl_left(300);
                        motor_ctl_right(-300);
                        break;
                    }
                    case NEC_RIGHT: {
                        printf("nec turn right\r\n");
                        motor_ctl_left(300);
                        motor_ctl_right(-300);
                        break;
                    }
                    case NEC_STOP: {
                        printf("nec stop\r\n");
                        motor_break();
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
}
