#include "stm32f10x_conf.h"

int Mode_state = -1; // 模式切换，0是关闭，1是防盗模式，2是车灯

u8 Alarm_init_switch = 0; // 启动防盗时的初始化。0未完成，1完成
u8 Alarm_open        = 0; // 是否触发防盗，触发1未触发0

u8 LIGHT_init_switch = 0; // 启动骑行模式检测初始化。0未完成，1完成
u8 Drop_open         = 0; // 是否触发摔倒保护，触发1未触发0

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
            case 1: // 防盗模式
                if (Alarm_init_switch == 0) {
                    MPU6050_Alarm_init();
                    OLED_ShowString(2, 1, "Alarm...");
                    Alarm_init_switch = 1; // 初始化已经完成
                }
                Delay_ms(80);

                MPU6050_detect_move();

                if (Alarm_open) {
                    OLED_ShowString(3, 1, "Alarm open!");
                    // USART2_Printf("alarm open");
                    ParseGps();       // 解析接收
                    printGpsBuffer(); // 处理接收
                }
                break;
            case 2: // 骑行模式
                if (LIGHT_init_switch == 0) {
                    MPU6050_Drop_init();
                    LED_open();
                    OLED_ShowString(2, 1, "Pose solve...");
                    LIGHT_init_switch = 1; // 初始化已经完成
                }

                // MPU6050_detect_drop();//检测姿态并比较
                // OLED_ShowSignedNum(7, 1, ++i, 2);

                // if (i >= 100) {
                //     i = 0;
                // }

                if (Drop_open == 1) {
                    TIM_Cmd(TIM3, DISABLE);
                    OLED_ShowString(3, 1, "Drop open!");
                    ParseGps();       // 解析接收
                    printGpsBuffer(); // 处理接收
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
