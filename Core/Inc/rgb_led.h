//
// Created by PKJ on 2024/6/23.
//

#ifndef RGB_LED_H
#define RGB_LED_H
#include "stm32f1xx_hal.h"


/**
 * 由于定时器资源紧张，value更改为数字量，不再支持PWM调光
 */
typedef struct {
    uint8_t R_value;
    uint8_t G_value;
    uint8_t B_value;
} LED_Config_t;

void led_config(LED_Config_t config);

#endif //RGB_LED_H
