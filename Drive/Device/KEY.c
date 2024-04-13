#include "KEY.h"
#include "Delay.h"
#include "LED.h"
#include "USART.h"

void Key_TIM1_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // 使能定时器1的时钟

    // 配置定时器1为向上计数模式，定时器时钟为72MHz
    TIM1->CR1 = 0;    // 禁用定时器
    TIM1->PSC = 7199; // 设置预分频值，得到10kHz的计数频率
    TIM1->ARR = 999;  // 设置自动重载值，得到10ms的定时周期

    TIM1->DIER |= TIM_DIER_UIE;        // 使能更新中断
    NVIC_SetPriority(TIM1_UP_IRQn, 0); // 设置中断优先级为最高
    TIM1->SR = 0;                      // 清除中断标志位
    TIM1->CR1 |= TIM_CR1_CEN;          // 使能定时器1
    NVIC_EnableIRQ(TIM1_UP_IRQn);      // 使能定时器1中断
}

void Key_GPIO_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Key_init(void)
{
    Key_GPIO_init();
    Key_TIM1_init();
}

// 更新显示状态
void Key_update_show(void)
{
    if (Mode_state == 0) {
        OLED_Clear();
        OLED_ShowString(1, 1, "MODE:");
        OLED_ShowString(1, 7, "OFF");
    }
    if (Mode_state == 1) {
        OLED_Clear();
        OLED_ShowString(1, 1, "MODE:");
        OLED_ShowString(1, 7, "ALARM");
    }
    if (Mode_state == 2) {
        OLED_Clear();
        OLED_ShowString(1, 1, "MODE:");
        OLED_ShowString(1, 7, "LIGHT");
    }
}

void Key_Scan(void)
{
    static char Key_read_last  = 0;
    static char Key_read_state = 0;

    Key_read_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) - Key_read_last;
    Key_read_last  = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
    if (Key_read_state == 1) { // 如果按键按下

        Alarm_init_switch = 0; // 清除防盗标志位
        Alarm_open        = 0; // 清除防盗标志位

        if (Mode_state < 2) {
            Mode_state++; // 模式切换
        } else
            Mode_state = 0; // 模式清零
        Key_update_show();
    }
}

void TIM1_UP_IRQHandler()
{
    // 处理定时器1的中断事件
    Key_Scan();
    // USART2_Printf("TIM1 done");
    // 清除中断标志位
    TIM1->SR &= ~TIM_SR_UIF;
}
