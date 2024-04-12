#include "stm32f10x_conf.h"

u8 Mode_state        = 1; // 模式切换，0是关闭，1是车灯，2是防盗模式
u8 Alarm_init_switch = 0; // 启动防盗时的初始化。0未完成，1完成
u8 Alarm_open        = 0; // 是否触发防盗，触发1未触发0

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
                if (Alarm_init_switch == 0) {
                    MPU6050_Alarm_init();
                    Alarm_init_switch = 1; // 初始化已经完成
                }
                Delay_ms(80);

                MPU6050_detect();
                if (Alarm_open) {
                    ParseGps();       // 解析接收
                    printGpsBuffer(); // 处理接收
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
