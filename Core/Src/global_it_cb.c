//
// Created by PKJ on 2024/6/25.
//

#include "global_it_cb.h"

#include <stdio.h>

#include "main.h"
#include "stm32f1xx_hal.h"
#include "hc_sr04.h"
#include "nec.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    switch (GPIO_Pin) {
        case OBSTACLE_LEFT_Pin: {
            //左红外避障中断
            // printf("left obstacle triggered\r\n");
            break;
        }
        case OBSTACLE_RIGHT_Pin: {
            //右红外避障中断
            // printf("right obstacle triggered\r\n");
            break;
        }
        case LIGHT_FIND_LEFT_Pin: {
            //左寻光中断
            // printf("left light find triggered\r\n");
            break;
        }
        case LIGHT_FIND_RIGHT_Pin: {
            //右寻光中断
            // printf("right light find triggered\r\n");
            break;
        }
        case ECHO_Pin: {
            if (hc_sr04_dev.level_high == 0) {
                //从空闲部分转到捕获模式
                hc_sr04_dev.start = SysTick->VAL;
                hc_sr04_dev.level_high = 1;
            } else if (hc_sr04_dev.level_high == 1) {
                hc_sr04_dev.level_high = 0;
                //捕获到一个完整的高电平
                hc_sr04_dev.end = SysTick->VAL;
                if (hc_sr04_dev.repeat == 0) {
                    hc_sr04_dev.distance = 0.0002361 * (hc_sr04_dev.start - hc_sr04_dev.end);
                } else if (hc_sr04_dev.repeat == 1) {
                    hc_sr04_dev.distance = 0.0002361 * (
                                               hc_sr04_dev.start + TIMER_COUNTER_MAX -
                                               hc_sr04_dev.end);
                } else {
                    hc_sr04_dev.distance = (0.0002361 * (hc_sr04_dev.start + TIMER_COUNTER_MAX - hc_sr04_dev.end)
                                            + 17 * (hc_sr04_dev.repeat - 1));
                }
                // if (hc_sr04_info.start > hc_sr04_info.end) {
                //     hc_sr04_dev.distance += 34000 * (double) (
                //                 hc_sr04_info.start - hc_sr04_info.end)
                //             / 72000000;
                // } else {
                //     hc_sr04_dev.distance += 34000 * (double) (16777216 - hc_sr04_info.end + hc_sr04_info.start);
                // }
                // hc_sr04_dev.distance += 34000 * ((double) 16777216 / 72000000 * hc_sr04_info.repeat);
                // if (complete_cb != NULL) {
                //     complete_cb(hc_sr04_dev.distance);
                // }
                // printf("start=%ld,end=%ld,repeat=%ld\r\n", hc_sr04_dev.start, hc_sr04_dev.end, hc_sr04_dev.repeat);
                // printf("start-end:%ld\r\n,hc_sr04_dev.distance:%lf\r\n", hc_sr04_dev.start - hc_sr04_dev.end,
                // 0.123);


                //TODO:修复直接以浮点数的形式打印距离时，打印出的数据错误的问题
                // printf("dis:%d\r\n",hc_sr04_dev.distance);
                //repeat采用自增计数方式，需要在下一次使用前清零
                hc_sr04_dev.repeat = 0;
            }
            break;
        }
        //nec接收状态机
        /**
         * 0x0 空闲状态,0x1开始接收引导吗9ms高电平,0x2开始接收引导码4.5m低电平
         * 0x3~0x4接收用户码第一字节,0x4~0x5接收用户码第二字节
         * 0x5~0x6接收用户码第三字节,0x6~0x7接收用户码第四字节
         *
         */
        case NEC_Pin: {
            //nec 红外解码
            switch (nec_state_machine) {
                case 0x0: {
                    //之前为空闲状态
                    nec_state_machine = 0x1;
                    //9ms跳过
                    break;
                }
                case 0x1: {
                    //之前为引导码9ms高电平
                    nec_state_machine = 0x2;
                    //4.5m跳过
                    break;
                }
                case 0x2: {
                    //之前为引导码4.5ms低电平
                    nec_state_machine = 0x3;
                    //记录该时刻的时间戳
                    nec_timestamp = TIM2->CNT;
                    break;
                }
                case 0x3:{

                }

            }
        }
        default:
            break;
    }
}
