//
// Created by PKJ on 2024/6/23.
//

#include "rgb_led.h"

#include "main.h"
extern TIM_HandleTypeDef htim3;

// void led_config(LED_Config_t config) {
//     TIM_OC_InitTypeDef sConfigOC = {0};
//     sConfigOC.OCMode = TIM_OCMODE_PWM1;
//     sConfigOC.Pulse = 1000 * config.R_value / 0xFF;
//     sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//     sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//     HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4);
//     sConfigOC.Pulse = 1000 * config.G_value / 0xFF;
//     HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);
//     sConfigOC.Pulse = 1000 * config.B_value / 0xFF;
//     HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2);
//     //开启PWM输出
//     HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
//     HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
//     HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
// }


void led_config(LED_Config_t config) {
    if (config.R_value) {
        HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(LED_R_GPIO_Port,LED_R_Pin, GPIO_PIN_RESET);
    }
    if (config.G_value) {
        HAL_GPIO_WritePin(LED_G_GPIO_Port,LED_G_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(LED_G_GPIO_Port,LED_G_Pin, GPIO_PIN_RESET);
    }
    if (config.B_value) {
        HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(LED_B_GPIO_Port,LED_B_Pin, GPIO_PIN_RESET);
    }
}
