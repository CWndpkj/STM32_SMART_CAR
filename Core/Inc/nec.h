//
// Created by PKJ on 2024/6/26.
//

#ifndef NEC_H
#define NEC_H
#include "stm32f1xx_hal.h"

extern uint8_t nec_state_machine;
extern uint32_t nec_timestamp;
extern uint32_t nec_data;
extern uint8_t nec_data_ready;

#define NEC_POWER 0x00
#define NEC_PLUS 0x30
#define NEC_MINUS 0x70
#define NEC_LIGHT 0x40
#define NEC_FORWARD 0x80
#define NEC_LEFT 0x20
#define NEC_RIGHT 0x60
#define NEC_BACKWARD 0x90
#define NEC_STOP 0xA0
#define NEC_NUMBER3 0x48
void nec_init();

uint8_t nec_is_data_ready();

/**
 * @note 必须在调用data_ready返回true后才返回有效数据，否则返回0
 * @return
 */
uint8_t nec_get_code();

#endif //NEC_H
