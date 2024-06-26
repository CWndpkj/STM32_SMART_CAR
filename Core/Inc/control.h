//
// Created by PKJ on 2024/6/25.
//

#ifndef CONTROL_H
#define CONTROL_H
#include "patrol_line.h"

extern uint8_t turn_left_flag;
extern uint8_t turn_right_flag;

void control_tick();

void control_turn_left_90();

void control_turn_right_90();

//TODO:小车避障及巡线功能的实现

#endif //CONTROL_H
