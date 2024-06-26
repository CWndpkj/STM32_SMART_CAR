//
// Created by PKJ on 2024/6/25.
//

#ifndef PID_H
#define PID_H
#include "stm32f1xx_hal.h"

void PID_Init(float p, float i, float d, float maxI, float maxOut);

/**
 * @brief 巡线前进：需要两个独立的PID计算：自旋，左右平移
 *
 * @param dealta_d 线距离中心点的水平距离
 * @param output 直接输出SBUS控制数据:yaw(自旋)和lateral(左右平移)
 */
void PID_Patrol_line_calc(float dealta_d, uint16_t output[2]);

#endif //PID_H
