//
// Created by PKJ on 2024/6/23.
//

#ifndef PATROL_LINE_H
#define PATROL_LINE_
#include "stm32f1xx_hal.h"

/**
 *  获取巡线模块的数据
 * @return 返回四路选线模块的数据，高电平表示识别到黑线，[3:0]分别对应左外侧，左内侧，右内侧，右外侧
 */
uint8_t get_patrol_data();

#endif //PATROL_LINE_H
