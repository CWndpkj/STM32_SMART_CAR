#include "hc_sr04.h"
#include <stdio.h>

#include "delay.h"
#include "main.h"
#define TIMER_COUNTER_MAX 71999
#define TIMER_FRQ 72000000
#define TIMER_T 0.001

static void (*complete_cb)(uint32_t dis);

static float distance;
hc_sr04_dev_t hc_sr04_dev;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == ECHO_Pin) {
        if (hc_sr04_dev.level_high == 0) {
            //从空闲部分转到捕获模式
            hc_sr04_dev.start = SysTick->VAL;
            hc_sr04_dev.level_high = 1;
        } else if (hc_sr04_dev.level_high == 1) {
            hc_sr04_dev.level_high = 0;
            //捕获到一个完整的高电平
            hc_sr04_dev.end = SysTick->VAL;
            if (hc_sr04_dev.repeat == 0) {
                distance = 34000 * (double) (hc_sr04_dev.start - hc_sr04_dev.end) / TIMER_FRQ;
            } else if (hc_sr04_dev.repeat == 1) {
                distance = 34000 * (double) (
                               hc_sr04_dev.start + TIMER_COUNTER_MAX -
                               hc_sr04_dev.end) / TIMER_FRQ;
            } else {
                distance = 34000 * ((double) (hc_sr04_dev.start + TIMER_COUNTER_MAX - hc_sr04_dev.end) / TIMER_FRQ
                                    + 0.001 *
                                    (hc_sr04_dev.repeat - 1));
            }
            // if (hc_sr04_info.start > hc_sr04_info.end) {
            //     distance += 34000 * (double) (
            //                 hc_sr04_info.start - hc_sr04_info.end)
            //             / 72000000;
            // } else {
            //     distance += 34000 * (double) (16777216 - hc_sr04_info.end + hc_sr04_info.start);
            // }
            // distance += 34000 * ((double) 16777216 / 72000000 * hc_sr04_info.repeat);
            // if (complete_cb != NULL) {
            //     complete_cb(distance);
            // }
            printf("start=%ld,end=%ld,repeat=%ld\r\n", hc_sr04_dev.start, hc_sr04_dev.end, hc_sr04_dev.repeat);
            printf("start-end:%ld\r\n,distance:%lf\r\n", hc_sr04_dev.start - hc_sr04_dev.end, distance);
            hc_sr04_dev.start = 0;
            hc_sr04_dev.end = 0;
            hc_sr04_dev.repeat = 0;
            distance = 0;
        }
    }
}

/**
 * @brief       定时器更新中断回调函数,计数溢出时调用
 * @param        htim:定时器句柄指针
 * @note        此函数会被定时器中断函数共同调用的
 * @retval      无
 */
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//     if (htim->Instance == TIM_) {
//         if (ECHO_sta & 0x80) {
//             // 表明定时器捕获到一个上升沿,在溢出时仍未捕获到下降沿
//             ECHO_sta++; // 溢出计数自增
//         }
//     }
// }

void hc_sr04_set_cb(void (*cb)(uint32_t dis)) {
    complete_cb = cb;
}

void hc_sr04_trig() {
    HAL_GPIO_WritePin(TRIGGER_GPIO_Port, TRIGGER_Pin, GPIO_PIN_SET);
    delay_us(20);
    HAL_GPIO_WritePin(TRIGGER_GPIO_Port, TRIGGER_Pin, GPIO_PIN_RESET);
}

void hc_sr04_start(uint16_t declipse_ms) {
    hc_sr04_dev.declipse = declipse_ms;
    hc_sr04_dev.next_mesure = declipse_ms;
    hc_sr04_dev.mesuring = 1;
}

void hc_sr04_stop() {
    hc_sr04_dev.mesuring = 0;
}

float hc_sr04_get_dis() {
    return distance;
}
