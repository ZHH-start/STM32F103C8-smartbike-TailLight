#include "stm32f10x_conf.h"

char print;
u8 Mode_state = 0;

int main(void)
{
    LED_Init();
    OLED_Init();
    USART1_Init(115200);
    USART2_Init(9600);
    MPU6050_Init();
    Key_init();
    OLED_ShowString(1, 1, "hello!");
    USART2_SendString("hello!");
    while (1) {
        // OLED_Clear();
        // OLED_ShowNum(10, 1, Mode_state, 1);
        Key_Scan();
        LED_close();
        Delay_ms(50);
    }
}
