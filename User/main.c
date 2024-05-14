#include "stm32f10x_conf.h"

int Mode_state = -1; // ģʽ�л���0�ǹرգ�1�Ƿ���ģʽ��2�ǳ���

u8 Alarm_init_switch = 0; // ��������ʱ�ĳ�ʼ����0δ��ɣ�1���
u8 Alarm_open        = 0; // �Ƿ񴥷�����������1δ����0

u8 LIGHT_init_switch = 0; // ��������ģʽ����ʼ����0δ��ɣ�1���
u8 Drop_open         = 0; // �Ƿ񴥷�ˤ������������1δ����0

u8 main_delay_key_use = 0; // ����keyɨ���������mainѭ��ִ���ٶ�

int main(void)
{
    int i;
    // long int j;
    OLED_Init();
    OLED_ShowString(1, 1, "Init...");
    LED_Init();
    ATGM_StructInit();
    USART1_Init(115200);
    USART2_Init(115200);
    MPU_Init();
    OLED_Clear();
    OLED_ShowString(1, 1, "Init done.");
    Key_init();

    while (1) {
        if (main_delay_key_use == 0) {
            // USART2_SendString("running\r\n");
            main_delay_key_use++;
            switch (Mode_state) {
                case 1: // ����ģʽ
                    if (Alarm_init_switch == 0) {
                        // MPU6050_Alarm_init();
                        OLED_ShowString(2, 1, "Alarm...");
                        Alarm_init_switch = 1; // ��ʼ���Ѿ����
                    }
                    // Delay_ms(80);
                    // MPU6050_move_compare();

                    if (Alarm_open) {
                        OLED_ShowString(3, 1, "Alarm open!");
                        USART1_SendString("GPS/alarm:Alarm open!");
                        // USART2_Printf("alarm open");
                        ParseGps();       // ��������
                        printGpsBuffer(); // �������
                        // Delay_ms(50);
                        if (GNRMC_Info.isCompute == 1) {
                            GNRMC_Info.isCompute = 0;
                            USART1_SendString("GPS/gps_long:");
                            USART1_SendString(GNRMC_Info.longitude); // ����
                            i = 22540;
                            while (i > 0) {
                                i--;
                            }

                            USART1_SendString("GPS/gps_lati:");
                            USART1_SendString(GNRMC_Info.latitude); // ����
                        }
                        //  else
                        //     USART1_Printf("GPS/alarm:No warning");
                    }
                    break;
                case 2: // ����ģʽ
                    if (LIGHT_init_switch == 0) {
                        // MPU6050_Drop_init();
                        LED_open();
                        OLED_ShowString(2, 1, "Pose solve...");
                        LIGHT_init_switch = 1; // ��ʼ���Ѿ����
                    }

                    // MPU6050_detect_drop();//�����̬���Ƚ�
                    // OLED_ShowSignedNum(7, 1, ++i, 2);
                    // MPU6050_drop_compare();
                    // if (i >= 100) {
                    //     i = 0;
                    // }

                    // Delay_ms(5);

                    if (Drop_open == 1) {
                        // TIM_Cmd(TIM3, DISABLE);
                        OLED_ShowString(3, 1, "Drop  open!");
                        USART1_SendString("GPS/alarm:Drop  open!");
                        ParseGps();       // ��������
                        printGpsBuffer(); // �������
                        if (GNRMC_Info.isCompute == 1) {
                            GNRMC_Info.isCompute = 0;
                            USART1_SendString("GPS/gps_long:");
                            USART1_SendString(GNRMC_Info.longitude); // ����
                            i = 22540;
                            while (i > 0) {
                                i--;
                            }
                            // Delay_ms(50);
                            USART1_SendString("GPS/gps_lati:");
                            USART1_SendString(GNRMC_Info.latitude); // ����
                        }
                        //  else
                        //     USART1_Printf("GPS/alarm:No warning");
                    }

                    break;
                default:
                    LED_close();
                    // USART1_Printf("GPS/alarm:No warning");
                    Alarm_init_switch = 0; // ���������ʼ����־λ
                    Alarm_open        = 0; // ���������־λ

                    LIGHT_init_switch = 0; // ������г�ʼ����־λ
                    Drop_open         = 0; // �Ƿ񴥷�ˤ������������1δ����0
                    // TIM_Cmd(TIM3, DISABLE);
                    break;
            }
        }
    }
}
