//
// Created by PKJ on 2024/6/24.
//

#include "servor.h"
extern TIM_HandleTypeDef htim1;

void servor_set_angle(int8_t angle) {
    uint16_t pulse = 1500;
    if (angle < 0) {
        //0~-90  ==>1.5ms~0.5ms ==>1500/20,000(1.5ms) ,500/20,000(0.5ms)
        pulse += (float)angle / (-90) * 1000;
    } else {
        pulse -= (float)angle / 90 * 1000;
    }
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = pulse;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
}
