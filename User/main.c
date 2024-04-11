#include "stm32f10x_conf.h"

char print;
extern Receive_GPS_data receDataFrame;//USART.c�ļ���ʹ�õ�GPS���ݽṹ��

int main(void)
{
    int i;
    LED_Init();
    OLED_Init();
    USART1_Init(115200);
    USART2_Init(9600);
    MPU6050_Init();
    // OLED_ShowString(1, 1, "receive:");
    AD_Init();
    // USART2_Printf("test");
    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    USART1_Printf("Init done.\n");
    USART2_Printf("USART2 Init done.\n");
    while (1) {

        if (receDataFrame.isGetData==1)
        {
            USART2_SendString(receDataFrame.Frame_Buffer);
            receDataFrame.isGetData = 0;
        }
        

        // LED_open();
        // Delay_ms(500);
        // LED_close();
        // Delay_ms(500);
    }
}
