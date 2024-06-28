//
// Created by PKJ on 2024/6/26.
//

#ifndef NEC_H
#define NEC_H
#include "stm32f1xx_hal.h"

extern uint8_t nec_state_machine;
extern uint32_t nec_timestamp;

uint16_t nec_get_code();
#endif //NEC_H
