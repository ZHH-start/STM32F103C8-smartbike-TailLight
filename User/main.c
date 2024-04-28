#include "stm32f10x_conf.h"

int Mode_state = -1; // ģʽ�л���0�ǹرգ�1�Ƿ���ģʽ��2�ǳ���

u8 Alarm_init_switch = 0; // ��������ʱ�ĳ�ʼ����0δ��ɣ�1���
u8 Alarm_open        = 0; // �Ƿ񴥷�����������1δ����0

u8 LIGHT_init_switch = 0; // ��������ģʽ����ʼ����0δ��ɣ�1���
u8 Drop_open         = 0; // �Ƿ񴥷�ˤ������������1δ����0

int main(void)
{
    // int i;
    LED_Init();
    OLED_Init();
    ATGM_StructInit();
    USART1_Init(115200);
    USART2_Init(9600);
    MPU_Init();
    Key_init();
    // OLED_ShowString(1, 1, "MODE:OFF");
    Delay_ms(100);

    while (1) {
        switch (Mode_state) {
            case 1: // ����ģʽ
                if (Alarm_init_switch == 0) {
                    MPU6050_Alarm_init();
                    OLED_ShowString(2, 1, "Alarm...");
                    Alarm_init_switch = 1; // ��ʼ���Ѿ����
                }
                Delay_ms(80);

                MPU6050_detect_move();

                if (Alarm_open) {
                    OLED_ShowString(3, 1, "Alarm open!");
                    // USART2_Printf("alarm open");
                    ParseGps();       // ��������
                    printGpsBuffer(); // �������
                }
                break;
            case 2: // ����ģʽ
                if (LIGHT_init_switch == 0) {
                    MPU6050_Drop_init();
                    LED_open();
                    OLED_ShowString(2, 1, "Pose solve...");
                    LIGHT_init_switch = 1; // ��ʼ���Ѿ����
                }

                // MPU6050_detect_drop();//�����̬���Ƚ�
                // OLED_ShowSignedNum(7, 1, ++i, 2);

                // if (i >= 100) {
                //     i = 0;
                // }

                if (Drop_open == 1) {
                    TIM_Cmd(TIM3, DISABLE);
                    OLED_ShowString(3, 1, "Drop open!");
                    ParseGps();       // ��������
                    printGpsBuffer(); // �������
                }

                break;
            default:
                LED_close();
                Drop_open = 0;
                TIM_Cmd(TIM3, DISABLE);
                break;
        }
    }
}
