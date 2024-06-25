//
// Created by PKJ on 2024/6/25.
//

#include "../Inc/beeper.h"

#include "main.h"
#include "delay.h"
beeper_dev_t beeper_dev;

void _beeper_load(uint16_t n_ms, uint8_t status) {
    beep_timestamp_t timestamp = {
        .declipse = n_ms,
        .status = status
    };
    beeper_dev.timestamp_list_len++;
    beeper_dev.timestamp_list[beeper_dev.timestamp_list_len - 1] = timestamp;
}

void _beeper_off() {
    HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin, GPIO_PIN_RESET);
}

void _beeper_on() {
    HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin, GPIO_PIN_SET);
}


void beeper_beep(beep_type_e type) {
    _beeper_on();
    switch (type) {
        case SHORT:
            _beeper_load(10, 1);
            break;
        case LONG:
            _beeper_load(50, 1);
            break;
        case SHORT_SHORT:
            _beeper_load(10, 1);
            _beeper_load(100, 0);
            _beeper_load(10, 1);
            break;
        case SHORT_LONG:
            _beeper_load(10, 1);
            _beeper_load(100, 0);
            _beeper_load(50, 1);
            break;
        case LONG_SHORT:
            _beeper_load(50, 1);
            _beeper_load(100, 0);
            _beeper_load(10, 1);
            break;
        case LONG_LONG:
            _beeper_load(50, 1);
            _beeper_load(100, 0);
            _beeper_load(50, 1);
            break;
    }
}
