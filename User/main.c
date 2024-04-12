#include "stm32f10x_conf.h"

char print;

int main(void)
{
    // LED_Init();
    OLED_Init();
    ATGM_StructInit();
    USART1_Init(115200);
    USART2_Init(9600);
    // MPU6050_Init();

    USART1_Printf("Init done.\n");
    USART2_Printf("USART2 Init done.\n");
    while (1) {
        ParseGps();//解析接收
        printGpsBuffer();//处理接收

        // LED_open();
        // Delay_ms(500);
        // LED_close();
        // Delay_ms(500);
    }
}
