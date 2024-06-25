//
// Created by PKJ on 2024/6/25.
//
#include <stdio.h>

#include "stm32f1xx_hal.h"
#include "hc_sr04.h"
extern hc_sr04_dev_t hc_sr04_dev;
extern TIM_HandleTypeDef htim2;
//定时器2中断，可用于按键扫描，固定间隔时间执行某个任务等
//note:可能包含执行时间较长的函数，不可用于对时间精度要求较高的函数
void schedule_start() {
    HAL_TIM_Base_Start_IT(&htim2);
}

void schedule_stop() {
    HAL_TIM_Base_Stop_IT(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    //1ms进一次中断
    //超声波传感器定时执行任务
    if (hc_sr04_dev.mesuring == 1) {
        hc_sr04_dev.next_mesure--;
        if (hc_sr04_dev.next_mesure == 0) {
            hc_sr04_dev.next_mesure = hc_sr04_dev.declipse;
            hc_sr04_trig();
        }
    }
}
