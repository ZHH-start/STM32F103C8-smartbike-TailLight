#include "IIC.h"
#include "USART.h"
#include "MPU6050.h"
#include "6050_use.h"
#include "Delay.h"
#include "math.h"

int16_t AX, AY, AZ;                   // 参考加速度
int16_t AX_later, AY_later, AZ_later; // 当前加速度

float pitch, roll, yaw;              // 欧拉角
int pitch_last, roll_last, yaw_last; // 欧拉角

// 启动防盗时6050保存初始数据
void MPU6050_Alarm_init(void)
{
    MPU_Get_Accelerometer(&AX, &AY, &AZ); // 获取一次数据
    AX_later = AX;
    AY_later = AY;
    AZ_later = AZ;
}

// 启动骑行摔倒检测时打开姿态解算
void MPU6050_Drop_init(void)
{
    if (mpu_dmp_get_data(&pitch, &roll, &yaw) == 0) {
        // OLED_ShowString(3, 1, "pitch:");
        // OLED_ShowNum(3, 7, (int)(pitch), 3);
        // OLED_ShowString(4, 1, "roll:");
        // OLED_ShowNum(4, 6, (int)(roll), 3);
        // OLED_ShowString(5, 1, "yaw:");
        // OLED_ShowNum(5, 5, (int)(yaw), 3);
    }

    pitch_last = (int)pitch; // 更新数据
    roll_last  = (int)roll;
    yaw_last   = (int)yaw;
}

void MPU6050_detect_move()
{
    if (MPU_Get_Accelerometer(&AX, &AY, &AZ))
        OLED_ShowString(7, 1, "error accelero");

    // OLED_ShowNum(3, 1, AX, 6);
    // OLED_ShowNum(4, 1, AY, 6);
    // OLED_ShowNum(5, 1, AZ, 6);

    if (abs(AX - AX_later) >= 10000) {
        // OLED_ShowString(6, 1, "ininini");
        // USART2_Printf("warning!");
        Alarm_open = 1;
    }
    if (abs(AY - AY_later) >= 10000) {
        // OLED_ShowString(6, 1, "ininini");
        // USART2_Printf("warning!");
        Alarm_open = 1;
    }
    if (abs(AZ - AZ_later) >= 10000) {
        // OLED_ShowString(6, 1, "ininini");
        // OLED_ShowNum(6, 1, abs(AZ - AZ_later), 6);
        // USART2_Printf("warning!");
        Alarm_open = 1;
    }

    AX_later = AX;
    AY_later = AY;
    AZ_later = AZ;
}

void MPU6050_detect_drop()
{

    if (mpu_dmp_get_data(&pitch, &roll, &yaw) == 0) {
        // OLED_ShowString(3, 1, "pitch:");
        // OLED_ShowNum(3, 7, (int)(pitch), 3);
        // OLED_ShowString(4, 1, "roll:");
        // OLED_ShowNum(4, 6, (int)(roll), 3);
        // OLED_ShowString(5, 1, "yaw:");
        // OLED_ShowNum(5, 5, (int)(yaw), 3);
    }
    if (LIGHT_init_switch) {  // 功能启动后
        if (Drop_open == 0) { // 未触发时运行
            if (abs(pitch_last - pitch) >= 15) {
                OLED_ShowString(6, 1, "sssss");
                Drop_open = 1;
            }
            if (abs(roll_last - roll) >= 15) {
                // OLED_ShowString(6, 1, "sssss");
                Drop_open = 1;
            }

            if (abs(yaw_last - yaw) >= 15) {
                // OLED_ShowString(6, 1, "sssss");
                Drop_open = 1;
            }
        }
    }

    pitch_last = (int)pitch; // 更新数据
    roll_last  = (int)roll;
    yaw_last   = (int)yaw;
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        // OLED_Clear();

        if (Alarm_init_switch) {
            if (Alarm_open == 0)
                MPU6050_detect_move();
        }

        // if (Drop_open == 0) {   //  只要警报没有被触发就一直运行
        MPU6050_detect_drop();
        // OLED_ShowString(5, 1, "Drop_open=0");

        // else

        // OLED_ShowString(5, 1, "Drop_open=1");
        // ;
        // }

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除中断标志位
    }
}
