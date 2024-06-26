#include "stm32f10x_conf.h"

int Mode_state = -1; // 模式切换，0是关闭，1是防盗模式，2是车灯

u8 Alarm_init_switch = 0; // 启动防盗时的初始化。0未完成，1完成
u8 Alarm_open        = 0; // 是否触发防盗，触发1未触发0

u8 LIGHT_init_switch = 0; // 启动骑行模式检测初始化。0未完成，1完成
u8 Drop_open         = 0; // 是否触发摔倒保护，触发1未触发0

u8 main_delay_key_use = 0; // 利用key扫描减缓整个main循环执行速度

int main(void)
{
    int i;
    // long int j;
    OLED_Init(); // SCL-PB8 SDA-PB9
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
                case 1: // 防盗模式
                    if (Alarm_init_switch == 0) {
                        // MPU6050_Alarm_init();
                        OLED_ShowString(2, 1, "Alarm...");
                        Alarm_init_switch = 1; // 初始化已经完成
                    }
                    // Delay_ms(80);
                    // MPU6050_move_compare();

                    if (Alarm_open) {
                        OLED_ShowString(3, 1, "Alarm open!");
                        USART1_SendString("GPS/alarm:Alarm open!");
                        // USART2_Printf("alarm open");
                        ParseGps();       // 解析接收
                        printGpsBuffer(); // 处理接收
                        // Delay_ms(50);
                        if (GNRMC_Info.isCompute == 1) {
                            GNRMC_Info.isCompute = 0;
                            USART1_SendString("GPS/gps_long:");
                            USART1_SendString(GNRMC_Info.longitude); // 经度
                            i = 22540;
                            while (i > 0) {
                                i--;
                            }

                            USART1_SendString("GPS/gps_lati:");
                            USART1_SendString(GNRMC_Info.latitude); // 经度
                        }
                        //  else
                        //     USART1_Printf("GPS/alarm:No warning");
                    }
                    break;
                case 2: // 骑行模式
                    if (LIGHT_init_switch == 0) {
                        // MPU6050_Drop_init();
                        LED_open();
                        OLED_ShowString(2, 1, "Pose solve...");
                        LIGHT_init_switch = 1; // 初始化已经完成
                    }

                    // MPU6050_detect_drop();//检测姿态并比较
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
                        ParseGps();       // 解析接收
                        printGpsBuffer(); // 处理接收
                        if (GNRMC_Info.isCompute == 1) {
                            GNRMC_Info.isCompute = 0;
                            USART1_SendString("GPS/gps_long:");
                            USART1_SendString(GNRMC_Info.longitude); // 经度
                            i = 22540;
                            while (i > 0) {
                                i--;
                            }
                            // Delay_ms(50);
                            USART1_SendString("GPS/gps_lati:");
                            USART1_SendString(GNRMC_Info.latitude); // 经度
                        }
                        //  else
                        //     USART1_Printf("GPS/alarm:No warning");
                    }

                    break;
                default:
                    LED_close();
                    // USART1_Printf("GPS/alarm:No warning");
                    Alarm_init_switch = 0; // 清除防盗初始化标志位
                    Alarm_open        = 0; // 清除防盗标志位

                    LIGHT_init_switch = 0; // 清除骑行初始化标志位
                    Drop_open         = 0; // 是否触发摔倒保护，触发1未触发0
                    // TIM_Cmd(TIM3, DISABLE);
                    break;
            }
        }
    }
}
