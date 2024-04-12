#include "KEY.h"
#include "Delay.h"
#include "LED.h"
#include "USART.h"

void Key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//更新显示状态
void Key_update_show(void)
{
    if (Mode_state == 0) {
        OLED_Clear();
        OLED_ShowString(1, 1, "STATE:");
        OLED_ShowString(1, 7, "OFF");
    }
    if (Mode_state == 1) {
        OLED_Clear();
        OLED_ShowString(1, 1, "STATE:");
        OLED_ShowString(1, 7, "ALARM");
    }
    if (Mode_state == 2) {
        OLED_Clear();
        OLED_ShowString(1, 1, "STATE:");
        OLED_ShowString(1, 7, "LIGHT");
    }
}

void Key_Scan(void)
{
    static char Key_read_last  = 0;
    static char Key_read_state = 0;

    Key_read_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) - Key_read_last;
    Key_read_last  = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
    if (Key_read_state == 1) { // 如果按键
        if (Mode_state < 2) {
            Mode_state++; // 模式切换
        } else
            Mode_state = 0; // 模式清零
        Key_update_show();
    }
}