#include "headfile.h" // Device header

char print;

int main(void)
{
    LED_Init();
    OLED_Init();
    USART2_Init(9600);
    USART1_Init(115200);
    MPU6050_Init();
    // OLED_ShowString(1, 1, "receive:");
    AD_Init();
    // USART1_Printf("test");
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    USART1_Printf("Init done.\n");
    // USART2_Printf("USART2 Init done.\n");
    while (1) {
            LED_open();
            Delay_ms(500);
            LED_close();
            Delay_ms(500);
    }

}
