//
// Created by PKJ on 2024/6/26.
//

#include "nec.h"
//nec接收状态机
/**
 * 0x0 空闲状态,0x1开始接收引导吗9ms高电平,0x2开始接收引导码4.5m低电平
 * 0x3~0x4接收用户码第一字节,0x4~0x5接收用户码第二字节
 * 0x5~0x6接收用户码第三字节,0x6~0x7接收用户码第四字节
 *
 */
uint8_t nec_state_machine = 0;
uint32_t nec_timestamp;
uint8_t nec_data_ready;

uint32_t nec_data;
extern TIM_HandleTypeDef htim3;

void nec_init() {
    HAL_TIM_Base_Start(&htim3);
}

uint8_t nec_get_code() {
    return (nec_data & 0xFF00) >> 8;
}

uint8_t nec_is_data_ready() {
    if (nec_data_ready) {
        nec_data_ready = 0;
        return 1;
    }
    return 0;
}
