//
// Created by PKJ on 2024/6/25.
//

#ifndef BEEPER_H
#define BEEPER_H
#include "stm32f1xx_hal.h"

typedef struct {
    uint16_t declipse; //表示这段时间的长短
    uint8_t status; //表示这段时间是开还是关
} beep_timestamp_t;

typedef struct {
    beep_timestamp_t timestamp_list[10];
    uint8_t timestamp_list_len;
} beeper_dev_t;


typedef enum {
    SHORT,
    LONG,
    SHORT_SHORT,
    SHORT_LONG,
    LONG_SHORT,
    LONG_LONG
} beep_type_e;

void beeper_beep(beep_type_e type);

void _beeper_off();

void _beeper_on();

#endif //BEEPER_H
