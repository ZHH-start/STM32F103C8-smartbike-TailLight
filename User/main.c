#include "stm32f10x_conf.h"

char print;
u8 Mode_state = 0;//模式切换，0是关闭，1是车灯，2是防盗模式

int main(void)
{
    LED_Init();
    OLED_Init();
    ATGM_StructInit();
    USART1_Init(115200);
    USART2_Init(9600);
    MPU6050_Init();
    Key_init();
    OLED_ShowString(1, 1, "STATE:OFF");
    USART2_SendString("hello!");
    while (1) {
        Key_Scan();
        Delay_ms(50);
        ParseGps();//解析接收
        printGpsBuffer();//处理接收
    }
}
