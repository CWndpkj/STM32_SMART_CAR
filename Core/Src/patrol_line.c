//
// Created by PKJ on 2024/6/23.
//

#include "patrol_line.h"

#include "main.h"


uint8_t get_patrol_data() {
    uint8_t patrol_mod_data = 0;
    if (HAL_GPIO_ReadPin(PATROL_LEFT_2_GPIO_Port,PATROL_LEFT_2_Pin)) {
        patrol_mod_data |= 0x1 << 3;
    }
    if (HAL_GPIO_ReadPin(PATROL_LEFT_1_GPIO_Port,PATROL_LEFT_1_Pin)) {
        patrol_mod_data |= 0x1 << 2;
    }
    if (HAL_GPIO_ReadPin(PATROL_RIGHT_1_GPIO_Port,PATROL_RIGHT_1_Pin)) {
        patrol_mod_data |= 0x1 << 1;
    }
    if (HAL_GPIO_ReadPin(PATROL_RIGHT_2_GPIO_Port,PATROL_RIGHT_2_Pin)) {
        patrol_mod_data |= 0x1;
    }
    return patrol_mod_data;
}
