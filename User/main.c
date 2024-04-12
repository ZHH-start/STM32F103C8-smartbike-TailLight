#include "stm32f10x_conf.h"

char print;
u8 Mode_state = 0;//ģʽ�л���0�ǹرգ�1�ǳ��ƣ�2�Ƿ���ģʽ

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
        ParseGps();//��������
        printGpsBuffer();//�������
    }
}
