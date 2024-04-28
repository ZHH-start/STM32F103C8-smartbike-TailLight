#include "IIC.h"
#include "USART.h"
#include "MPU6050.h"
#include "6050_use.h"
#include "Delay.h"
#include "math.h"

int16_t AX, AY, AZ;                   // 参考加速度
int16_t AX_later, AY_later, AZ_later; // 当前加速度
float pitch, roll, yaw;               // 欧拉角

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
    while (mpu_dmp_init()) { printf("6050mpu orror"); }
}

void MPU6050_detect_move()
{
    MPU_Get_Accelerometer(&AX_later, &AY_later, &AZ_later);
    // USART2_Printf("AX:");USART2_SendNumber(GX,6);USART2_Printf("\n");
    // USART2_Printf("AY:");USART2_SendNumber(GY,6);USART2_Printf("\n");
    // USART2_Printf("AZ:");USART2_SendNumber(GZ,6);USART2_Printf("\n");
    // USART2_Printf("AX:");USART2_SendNumber(AX,6);USART2_Printf("\n");
    // USART2_Printf("AY:");USART2_SendNumber(AY,6);USART2_Printf("\n");
    // USART2_Printf("AZ:");USART2_SendNumber(AZ,6);USART2_Printf("\n");

    // OLED_ShowNum(3, 1, AX, 6);
    // OLED_ShowNum(4, 1, AY, 6);
    // OLED_ShowNum(5, 1, AZ, 6);

    if (abs(AX - AX_later) >= 1000) {
        // USART2_Printf("warning!");
        Alarm_open = 1;
    }
    if (abs(AY - AY_later) >= 1000) {
        // USART2_Printf("warning!");
        Alarm_open = 1;
    }
    if (abs(AZ - AZ_later) >= 1000) {
        // USART2_Printf("warning!");
        Alarm_open = 1;
    }
    AX_later = AX;
    AY_later = AY;
    AZ_later = AZ;

    // OLED_ShowString(10,1,"AX:");OLED_ShowNum(10,1,abs(AX - AX_later),6);
    // OLED_ShowString(15,1,"AY:");OLED_ShowNum(15,1,abs(AY - AY_later),6);
    // OLED_ShowString(20,1,"AZ:");OLED_ShowNum(20,1,abs(AZ - AZ_later),6);

    // OLED_ShowString(10,1,"AX:");OLED_ShowNum(10,1,GX,6);
    // OLED_ShowString(15,1,"AY:");OLED_ShowNum(15,1,GY,6);
    // OLED_ShowString(20,1,"AZ:");OLED_ShowNum(20,1,GZ,6);
}

void MPU6050_detect_drop()
{
    static int pitch_last, roll_last, yaw_last; // 欧拉角
    if (mpu_dmp_get_data(&pitch, &roll, &yaw) == 0) {
        OLED_ShowString(3, 1, "pitch:");
        OLED_ShowNum(3, 7, (int)(pitch), 3);
        OLED_ShowString(4, 1, "roll:");
        OLED_ShowNum(4, 6, (int)(roll), 3);
        OLED_ShowString(5, 1, "yaw:");
        OLED_ShowNum(5, 5, (int)(yaw), 3);
    }

    if (abs(pitch_last - pitch)) {
        Drop_open = 1;
    }
    if (abs(roll_last - roll)) {
        Drop_open = 1;
    }

    if (abs(yaw_last - yaw)) {
        Drop_open = 1;
    }

    pitch_last = (int)pitch; // 更新数据
    roll_last  = (int)roll;
    yaw_last   = (int)yaw;
}