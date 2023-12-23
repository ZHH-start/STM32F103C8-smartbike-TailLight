#include "headfile.h"                  // Device header

char print;

int main(void)
{
	LED_Init();
    OLED_Init();
    USART1_Init(115200);
    MPU6050_Init();

    USART1_Printf("Init done.");
	while (1)
	{
		LED_open();
        Delay_ms(80);
        LED_close();
        Delay_ms(80);
        MPU6050_GetData();
        OLED_ShowString(1,1,"GX:");
        OLED_ShowSignedNum(1,4,GX,6);

        OLED_ShowString(2,1,"GY:");
        OLED_ShowSignedNum(2,4,GY,6);
        
	}
}
