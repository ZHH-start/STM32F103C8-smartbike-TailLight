#include "stm32f10x_conf.h"

char print;

int main(void)
{
    // LED_Init();
    OLED_Init();
    USART1_Init(115200);
    USART2_Init(9600);
    // MPU6050_Init();
    // OLED_ShowString(1, 1, "receive:");
    // AD_Init();
    // USART2_Printf("test");
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    USART1_Printf("Init done.\n");
    USART2_Printf("USART2 Init done.\n");
    while (1) {

        if (GNRMC_Info.isGetData==1)
        {
            USART2_SendString(GNRMC_Info.GPS_Buffer);
            GNRMC_Info.isGetData = 0;
        }
        

        // LED_open();
        // Delay_ms(500);
        // LED_close();
        // Delay_ms(500);
    }
}
