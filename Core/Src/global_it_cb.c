//
// Created by PKJ on 2024/6/25.
//

#include "global_it_cb.h"

#include <stdio.h>
#include <sys/time.h>

#include "delay.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "hc_sr04.h"
#include "motor.h"
#include "nec.h"
extern TIM_HandleTypeDef htim3;
uint8_t count = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    switch (GPIO_Pin) {
        case OBSTACLE_LEFT_Pin: {
            //左红外避障中断
            // printf("left obstacle triggered\r\n");
                break;
                motor_ctl_left(100);
                motor_ctl_left(200);
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
         * ...
         */
        case NEC_Pin: {
            // nec 红外解码
            if (nec_state_machine == 0x0) {
                //之前为空闲状态
                nec_state_machine++;
                TIM3->CNT = 0;
            } else if (nec_state_machine == 0x1) {
                uint32_t counter = __HAL_TIM_GetCounter(&htim3);
                if (counter > 8000 && counter < 15000) {
                    //之前为引导码9ms低电平
                    nec_state_machine++;
                    TIM3->CNT = 0;
                } else {
                    //接收错误，重新接收
                    printf("%ld\r\n", counter);
                    nec_state_machine = 0;
                }
            } else if (nec_state_machine == 0x2) {
                uint32_t counter = __HAL_TIM_GetCounter(&htim3);
                printf("counter:%d\r\n", counter);
                if (counter > 3000) {
                    //引导码4.5ms高电平
                    nec_state_machine++;
                    //清空nec_data,开始新的数据接收
                    nec_data = 0;
                } else if (counter < 3000 && counter > 1500) {
                    //重复码2.25ms
                    nec_state_machine = 66;
                    printf("rcved repeat\r\n");
                } else {
                    //接收错误
                    printf("wrong rcv\r\n");
                    nec_state_machine = 0;
                }
            } else if (nec_state_machine <= 65) {
                nec_state_machine++;
                if (nec_state_machine % 2 == 1) {
                    //接收的是一字节的低电平部分
                    //定时器准备
                    __HAL_TIM_SetCounter(&htim3, 0);
                } else {
                    //接收的是一字节的高电平部分
                    //读取定时器的值
                    uint32_t counter = __HAL_TIM_GetCounter(&htim3);
                    printf("%d\r\n", counter);
                    if (counter > 1000) {
                        //该bit的值为1
                        nec_data |= (0x80000000) >> ((nec_state_machine + 3) / 2 - 3);
                    }
                }
            } else if (nec_state_machine == 66) {
                printf("rcv complete:%x\r\n", nec_data);
                //一次完整的传输过程，所有值复位
                nec_data_ready = 1;
                nec_state_machine = 0;
            } else {
                printf("rcv_error:%d\r\n", nec_state_machine);
                //接收错误
                nec_state_machine = 0;
            }
            break;
        }
        default:
            break;
    }
}
