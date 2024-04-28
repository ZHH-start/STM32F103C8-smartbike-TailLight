#ifndef __6050_USE_H
#define __6050_USE_H
#include "stm32f10x.h"

extern u8 Drop_open; // 是否触发摔倒保护，触发1未触发0

void MPU6050_Alarm_init(void);
void MPU6050_Drop_init(void);
void MPU6050_detect_move();
void MPU6050_detect_drop();

#endif