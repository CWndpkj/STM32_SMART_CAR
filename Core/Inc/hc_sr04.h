#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "stm32f1xx_hal.h"
#define TIMER_COUNTER_MAX 71999.0
#define TIMER_FRQ 72000000.0
#define TIMER_OVERFLOW 0.001

typedef struct {
    uint32_t start;
    uint32_t end;
    uint16_t repeat;
    uint8_t level_high; //表示echo接收高电平
    uint16_t declipse; //测量间隔时间(ms)
    uint16_t next_mesure; //for 测量频率
    uint8_t mesuring; //测量状态，调用 start()函数后置位
    uint8_t distance; //测量结果
} hc_sr04_dev_t;

void hc_sr04_set_cb(void (*cb)(uint32_t dis));

void hc_sr04_trig();

void hc_sr04_start(uint16_t declipse_ms);

void hc_sr04_stop();

uint8_t hc_sr04_get_dis();

extern hc_sr04_dev_t hc_sr04_dev;

#endif  // !__HC_SR04_H
