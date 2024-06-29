#include "main.h"
#include "stm32f1xx_hal_gpio.h"
//
// Created by PKJ on 2024/6/23.
//
void fan_on() {
    HAL_GPIO_WritePin(FAN_GPIO_Port,FAN_Pin, GPIO_PIN_RESET);
}

void fan_off() {
    HAL_GPIO_WritePin(FAN_GPIO_Port,FAN_Pin, GPIO_PIN_SET);
}

#include "fan.h"
