#include "motor.h"

#include <stdio.h>

#include "main.h"

extern TIM_HandleTypeDef htim4;
motor_dev_t motor_dev;
//TODO:根据实际情况调整合适的比例系数
#define RATIO 5

/**
 *
 * @param speed 运动的方向及速度,符号为正位前进，为负为后退
 * @param declipse 持续运动的时间
 */
void motor_move(int speed, uint8_t declipse) {
    motor_timestamp_t timestamp = {
        .left_speed = speed,
        .right_speed = speed,
        .declipse = declipse
    };
    motor_dev.timestamp_list_len++;
    motor_dev.timestamp_list[motor_dev.timestamp_list_len - 1] = timestamp;
}


/**
 * 
 * @param angle 转动的角度，值为负为逆时针转动，值为正为顺时针转动
 */
void motor_turn_angle(int angle) {
    int16_t left_speed, right_speed;
    if (angle > 0) {
        left_speed = 500;
        right_speed = 0;
    } else {
        left_speed = 0;
        right_speed = 0;
        angle = -angle;
    }
    motor_timestamp_t timestamp = {
        .declipse = angle * RATIO,
        .left_speed = left_speed,
        .right_speed = right_speed
    };
    motor_dev.timestamp_list_len++;
    motor_dev.timestamp_list[motor_dev.timestamp_list_len - 1] = timestamp;
}


/**
 *
 * @param speed 转动速度，数值为正时为前进，数值为负时为后退
 */
void motor_ctl_left(int speed) {
    if (speed >= 0) {
        HAL_GPIO_WritePin(MOTOR_LEFT_CTL2_GPIO_Port, MOTOR_LEFT_CTL2_Pin, GPIO_PIN_SET); // AIN2
        HAL_GPIO_WritePin(MOTOR_LEFT_CTL1_GPIO_Port, MOTOR_LEFT_CTL1_Pin, GPIO_PIN_RESET); // AIN1
    } else {
        HAL_GPIO_WritePin(MOTOR_LEFT_CTL2_GPIO_Port, MOTOR_LEFT_CTL2_Pin, GPIO_PIN_RESET); // AIN2
        HAL_GPIO_WritePin(MOTOR_LEFT_CTL1_GPIO_Port, MOTOR_LEFT_CTL1_Pin, GPIO_PIN_SET); // AIN1
        speed = -speed;
    }
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = speed;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2);
    //开启PWM输出
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
}

void motor_ctl_right(int speed) {
    if (speed >= 0) {
        HAL_GPIO_WritePin(MOTOR_RIGHT_CTL2_GPIO_Port, MOTOR_RIGHT_CTL2_Pin, GPIO_PIN_SET); // BIN2
        HAL_GPIO_WritePin(MOTOR_RIGHT_CTL1_GPIO_Port, MOTOR_RIGHT_CTL1_Pin, GPIO_PIN_RESET); // BIN1
    } else {
        HAL_GPIO_WritePin(MOTOR_RIGHT_CTL2_GPIO_Port, MOTOR_RIGHT_CTL2_Pin, GPIO_PIN_RESET); // BIN2
        HAL_GPIO_WritePin(MOTOR_RIGHT_CTL1_GPIO_Port, MOTOR_RIGHT_CTL1_Pin, GPIO_PIN_SET); // BIN1
        speed = -speed;
    }
    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = speed;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1);
    //开启PWM输出
    HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
}


void motor_break(void) {
    motor_ctl_left(0);
    motor_ctl_right(0);
}
