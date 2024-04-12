#ifndef _KEY_H
#define _KEY_H
#include "stm32f10x.h"

extern u8 Mode_state;
void Key_init(void);
void Key_Scan(void);

extern u8 Alarm_init_switch; // 启动防盗时的初始化。0未完成，1完成
extern u8 Alarm_open;        // 是否触发防盗，触发1未触发0

#endif
