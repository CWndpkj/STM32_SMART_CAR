//
// Created by PKJ on 2024/6/23.
//

#include "stm32f1xx_hal.h"
extern UART_HandleTypeDef huart1;

int _write(int fd, char *pBuffer, int size) {
    HAL_UART_Transmit(&huart1, (uint8_t *) pBuffer, size,HAL_MAX_DELAY);
    return size;
}
