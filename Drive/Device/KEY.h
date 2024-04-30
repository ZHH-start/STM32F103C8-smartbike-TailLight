#ifndef _KEY_H
#define _KEY_H
#include "stm32f10x.h"

extern int Mode_state;
void Key_init(void);
void Key_Scan(void);
void Key_update_show(void);

extern u8 Alarm_init_switch; // 启动防盗时的初始化。0未完成，1完成
extern u8 Alarm_open;        // 是否触发防盗，触发1未触发0

extern u8 LIGHT_init_switch; // 启动骑行模式检测初始化。0未完成，1完成
extern u8 Drop_open;         // 是否触发摔倒保护，触发1未触发0

extern u8 main_delay_key_use; // 利用key扫描减缓整个main循环执行速度

#endif
