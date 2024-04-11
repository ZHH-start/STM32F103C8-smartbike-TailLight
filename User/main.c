#include "stm32f10x_conf.h"

char print;

int main(void)
{
    int i;
    LED_Init();
    OLED_Init();
    USART1_Init(115200);
    USART2_Init(115200);
    MPU6050_Init();
    // OLED_ShowString(1, 1, "receive:");
    AD_Init();
    // USART2_Printf("test");
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    USART1_Printf("Init done.\n");
    USART2_Printf("USART2 Init done.\n");
    while (1) {

        if (USART2_RX_STA & 0x8000) {
            USART2_RX_STA = USART2_RX_STA & 0x7FFF;
            USART2_SendString(USART2_RX_BUF);
            for (i = 0; i < 200; i++) {
                USART2_RX_BUF[i] = '\0';
            }
        }

        LED_open();
        Delay_ms(500);
        LED_close();
        Delay_ms(500);
    }
}
