//
// Created by PKJ on 2024/6/25.
//

#include "../Inc/pid.h"

#define LIMIT( x, min, max ) ( x ) = ( ( ( x ) <= ( min ) ) ? ( min ) : ( ( ( x ) >= ( max ) ) ? ( max ) : ( x ) ) )

typedef struct {
    float kp, ki, kd;
    float error, lastError; // 误差、上次误差
    float integral, maxIntegral; // 积分、积分限幅
    float output, maxOutput; // 输出、输出限幅
} PID_t;

typedef struct {
    PID_t inner; // 内环
    PID_t outer; // 外环
    float output; // 串级输出，等于inner.output
} CascadePID_t;

static PID_t pid;

static void _Init(PID_t *pid, float p, float i, float d, float maxI, float maxOut) {
    pid->kp = p;
    pid->ki = i;
    pid->kd = d;
    pid->maxIntegral = maxI;
    pid->maxOutput = maxOut;
}

// 清空一个pid的历史数据
static void _Clear(PID_t *pid) {
    pid->error = 0;
    pid->lastError = 0;
    pid->integral = 0;
    pid->output = 0;
}

// 单级pid计算
static void _SingleCalc(PID_t *pid, float reference, float feedback) {
    // 更新数据
    pid->lastError = pid->error;
    pid->error = reference - feedback;
    // 计算微分
    pid->output = (pid->error - pid->lastError) * pid->kd;
    // 计算比例
    pid->output += pid->error * pid->kp;
    // 计算积分
    pid->integral += pid->error * pid->ki;
    LIMIT(pid->integral, -pid->maxIntegral, pid->maxIntegral); // 积分限幅
    pid->output += pid->integral;
    // 输出限幅
    LIMIT(pid->output, -pid->maxOutput, pid->maxOutput);
}

// 串级pid计算
static void _CascadeCalc(CascadePID_t *pid, float angleRef, float angleFdb, float speedFdb) {
    _SingleCalc(&pid->outer, angleRef, angleFdb); // 计算外环(角度环)
    _SingleCalc(&pid->inner, pid->outer.output, speedFdb); // 计算内环(速度环)
    pid->output = pid->inner.output;
}

void PID_Init(float p, float i, float d, float maxI, float maxOut) {
    for (uint8_t idx = 0; idx < 4; idx++) {
        _Init(&pid, p, i, d, maxI, maxOut);
    }
}

void PID_Patrol_line_calc(float dealta_d, uint16_t *output) {
    // 更新水平距离的数据
    _SingleCalc(&pid, 0, dealta_d);
    *output = pid.output;
}
