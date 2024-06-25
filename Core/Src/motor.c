#include "motor.h"

#include "main.h"

extern TIM_HandleTypeDef htim4;

/**
 *
 * @param speed 转动速度，数值为正时为前进，数值为负时为后退
 */
void LeftMotor_Control(int speed) {
    if (speed > 0) {
        HAL_GPIO_WritePin(MOTOR_LEFT_CTL2_GPIO_Port, MOTOR_LEFT_CTL2_Pin, GPIO_PIN_RESET); // AIN2
        HAL_GPIO_WritePin(MOTOR_LEFT_CTL1_GPIO_Port, MOTOR_LEFT_CTL1_Pin, GPIO_PIN_SET); // AIN1
    } else {
        HAL_GPIO_WritePin(MOTOR_LEFT_CTL2_GPIO_Port, MOTOR_LEFT_CTL2_Pin, GPIO_PIN_SET); // AIN2
        HAL_GPIO_WritePin(MOTOR_LEFT_CTL1_GPIO_Port, MOTOR_LEFT_CTL1_Pin, GPIO_PIN_RESET); // AIN1
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

void RightMotor_Control(int speed) {
    if (speed > 0) {
        HAL_GPIO_WritePin(MOTOR_RIGHT_CTL2_GPIO_Port, MOTOR_RIGHT_CTL2_Pin, GPIO_PIN_RESET); // BIN2
        HAL_GPIO_WritePin(MOTOR_RIGHT_CTL1_GPIO_Port, MOTOR_RIGHT_CTL1_Pin, GPIO_PIN_SET); // BIN1
    } else {
        HAL_GPIO_WritePin(MOTOR_RIGHT_CTL2_GPIO_Port, MOTOR_RIGHT_CTL2_Pin, GPIO_PIN_SET); // BIN2
        HAL_GPIO_WritePin(MOTOR_RIGHT_CTL1_GPIO_Port, MOTOR_RIGHT_CTL1_Pin, GPIO_PIN_RESET); // BIN1
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


void Break(void) {
    LeftMotor_Control(0);
    RightMotor_Control(0);
}
