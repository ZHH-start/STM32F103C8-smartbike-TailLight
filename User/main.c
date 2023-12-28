#include "headfile.h" // Device header

char print;

int main(void)
{
    LED_Init();
    OLED_Init();
    USART1_Init(115200);
    MPU6050_Init();
    AD_Init();

    USART1_Printf("Init done.");
    while (1) {
        LED_open();
        
        Delay_ms(150);
        LED_close();
        // OLED_Clear();
        Delay_ms(150);
        // MPU6050_GetData();

        // OLED_ShowStr(0,0,"abcdefg",1);

        // OLED_ShowString(1, 1, "GX:");
        // OLED_ShowSignedNum(1, 4, GX, 4);

        // OLED_ShowString(2, 1, "GY:");
        // OLED_ShowSignedNum(2, 4, GY, 4);
    }
}
