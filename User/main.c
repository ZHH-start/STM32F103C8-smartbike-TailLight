#include "stm32f10x_conf.h"

u8 Mode_state        = 0; // ģʽ�л���0�ǹرգ�1�Ƿ���ģʽ��2�ǳ���
u8 Alarm_init_switch = 0; // ��������ʱ�ĳ�ʼ����0δ��ɣ�1���
u8 Alarm_open        = 0; // �Ƿ񴥷�����������1δ����0

int main(void)
{
    LED_Init();
    OLED_Init();
    ATGM_StructInit();
    USART1_Init(115200);
    USART2_Init(9600);
    MPU6050_Init();
    Key_init();
    OLED_ShowString(1, 1, "MODE:OFF");

    while (1) {
        switch (Mode_state) {
            case 1:
                if (Alarm_init_switch == 0) {
                    Delay_s(1);
                    MPU6050_Alarm_init();
                    OLED_ShowString(10, 1, "Alarm...");
                    Alarm_init_switch = 1; // ��ʼ���Ѿ����
                }
                Delay_ms(80);

                MPU6050_detect();
                if (Alarm_open) {
                    OLED_ShowString(10, 1, "Alarm open!");
                    // USART2_Printf("alarm open");
                    ParseGps();//��������
                    printGpsBuffer(); // �������
                }
                break;
            case 2:
                LED_open();
                break;
            default:
                LED_close();
                break;
        }
    }
}
