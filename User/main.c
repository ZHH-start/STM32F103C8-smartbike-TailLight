#include "stm32f10x_conf.h"

u8 Mode_state = 0; // ģʽ�л���0�ǹرգ�1�ǳ��ƣ�2�Ƿ���ģʽ

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
        switch (Mode_state) {
            case 1:
                // Delay_s(2);
                MPU6050_detect();
                ParseGps();       // ��������
                printGpsBuffer(); // �������
                // USART2_Printf("done!");
                break;
            case 2:
                // Delay_s(1);
                // USART2_Printf("LEDOPEN");
                LED_open();
                // USART2_Printf("2222");
                break;
            default:
                LED_close();
                break;
        }
    }
}
