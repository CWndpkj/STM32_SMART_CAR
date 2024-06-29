/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PATROL_LEFT_2_Pin GPIO_PIN_13
#define PATROL_LEFT_2_GPIO_Port GPIOC
#define PATROL_LEFT_1_Pin GPIO_PIN_14
#define PATROL_LEFT_1_GPIO_Port GPIOC
#define PATROL_RIGHT_1_Pin GPIO_PIN_15
#define PATROL_RIGHT_1_GPIO_Port GPIOC
#define BEEPER_Pin GPIO_PIN_0
#define BEEPER_GPIO_Port GPIOA
#define FAN_Pin GPIO_PIN_1
#define FAN_GPIO_Port GPIOA
#define OBSTACLE_RIGHT_Pin GPIO_PIN_4
#define OBSTACLE_RIGHT_GPIO_Port GPIOA
#define OBSTACLE_RIGHT_EXTI_IRQn EXTI4_IRQn
#define LIGHT_FIND_RIGHT_Pin GPIO_PIN_5
#define LIGHT_FIND_RIGHT_GPIO_Port GPIOA
#define LIGHT_FIND_RIGHT_EXTI_IRQn EXTI9_5_IRQn
#define OBSTACLE_LEFT_Pin GPIO_PIN_6
#define OBSTACLE_LEFT_GPIO_Port GPIOA
#define OBSTACLE_LEFT_EXTI_IRQn EXTI9_5_IRQn
#define LED_B_Pin GPIO_PIN_7
#define LED_B_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_0
#define LED_G_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_1
#define LED_R_GPIO_Port GPIOB
#define PATROL_RIGHT_2_Pin GPIO_PIN_12
#define PATROL_RIGHT_2_GPIO_Port GPIOB
#define NEC_Pin GPIO_PIN_13
#define NEC_GPIO_Port GPIOB
#define NEC_EXTI_IRQn EXTI15_10_IRQn
#define SERVOR_Pin GPIO_PIN_11
#define SERVOR_GPIO_Port GPIOA
#define ECHO_Pin GPIO_PIN_12
#define ECHO_GPIO_Port GPIOA
#define ECHO_EXTI_IRQn EXTI15_10_IRQn
#define TRIGGER_Pin GPIO_PIN_15
#define TRIGGER_GPIO_Port GPIOA
#define LIGHT_FIND_LEFT_Pin GPIO_PIN_3
#define LIGHT_FIND_LEFT_GPIO_Port GPIOB
#define LIGHT_FIND_LEFT_EXTI_IRQn EXTI3_IRQn
#define MOTOR_RIGHT_CTL2_Pin GPIO_PIN_4
#define MOTOR_RIGHT_CTL2_GPIO_Port GPIOB
#define MOTOR_RIGHT_CTL1_Pin GPIO_PIN_5
#define MOTOR_RIGHT_CTL1_GPIO_Port GPIOB
#define RIGHT_SPEED_Pin GPIO_PIN_6
#define RIGHT_SPEED_GPIO_Port GPIOB
#define LEFT_SPEED_Pin GPIO_PIN_7
#define LEFT_SPEED_GPIO_Port GPIOB
#define MOTOR_LEFT_CTL1_Pin GPIO_PIN_8
#define MOTOR_LEFT_CTL1_GPIO_Port GPIOB
#define MOTOR_LEFT_CTL2_Pin GPIO_PIN_9
#define MOTOR_LEFT_CTL2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
