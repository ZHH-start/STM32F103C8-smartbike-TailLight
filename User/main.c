#include "stm32f10x_conf.h"

char print;
u8 Mode_state = 0;

int main(void)
{
    LED_Init();
    OLED_Init();
    ATGM_StructInit();
    USART1_Init(115200);
    USART2_Init(9600);
    MPU6050_Init();
    Key_init();
    OLED_ShowString(1, 1, "hello!");
    USART2_SendString("hello!");
    while (1) {
        Key_Scan();
        LED_close();
        Delay_ms(50);
        ParseGps();//解析接收
        printGpsBuffer();//处理接收
    }
}
