#include "stm32f10x_conf.h"

char print;

int main(void)
{
    int i;
    char *j;
    u8 USART2_RX_BUF_order[] = "find";
    LED_Init();
    OLED_Init();
    USART1_Init(115200);
    USART2_Init(9600);
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
            // USART1_SendString("in");
            // USART1_SendString(USART2_RX_BUF);
            j = strstr(USART2_RX_BUF, USART2_RX_BUF_order);
            USART1_SendByte(*j);
            USART1_SendByte(*(j+1));
            USART1_SendByte(*(j+2));
            // USART1_SendString("done");
            for (i = 0; i < 70; i++) {
                USART2_RX_BUF[i] = '\0';
            }
        }

        

        LED_open();
        Delay_ms(500);
        LED_close();
        Delay_ms(500);
    }
}
