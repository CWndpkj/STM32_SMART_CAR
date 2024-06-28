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
uint8_t nec_state_machine;
uint32_t nec_timestamp;

uint16_t nec_get_code() {
}
