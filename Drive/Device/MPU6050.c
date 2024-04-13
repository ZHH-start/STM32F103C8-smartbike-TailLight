#include "IIC.h"
#include "USART.h"
#include "MPU6050.h"
#include "Delay.h"
#include "math.h"

int16_t AX, AY, AZ, GX, GY, GZ;
int16_t AX_later, AY_later, AZ_later;

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(Data);
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data;

    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddress);
    MyI2C_ReceiveAck();

    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
    MyI2C_ReceiveAck();
    Data = MyI2C_ReceiveByte();
    MyI2C_SendAck(1);
    MyI2C_Stop();

    return Data;
}

uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

// 传感器上电初始化
void MPU6050_Init(void)
{
    MyI2C_Init();
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x00); // 电源管理，典型值：0x00(正常启用)
    // MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x07);   // 陀螺仪采样率1k
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);       // 低通滤波频率
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);  // 陀螺仪自检及测量范围
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x08); // 加速计自检、测量范围
}

// 得到传感器的数据
void MPU6050_GetData()
{
    uint8_t DataH, DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    AX    = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    AY    = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    AZ    = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    GX    = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    GY    = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    GZ    = (DataH << 8) | DataL;
}

// 启动防盗时6050保存初始数据
void MPU6050_Alarm_init(void)
{
    // Delay_ms(200);        // 先等待200ms
    MPU6050_GetData(); // 获取一次数据
    AX_later = AX;
    AY_later = AY;
    AZ_later = AZ;
}

void MPU6050_detect()
{

    MPU6050_GetData();
    // USART2_Printf("AX:");USART2_SendNumber(GX,6);USART2_Printf("\n");
    // USART2_Printf("AY:");USART2_SendNumber(GY,6);USART2_Printf("\n");
    // USART2_Printf("AZ:");USART2_SendNumber(GZ,6);USART2_Printf("\n");
    // USART2_Printf("AX:");USART2_SendNumber(AX,6);USART2_Printf("\n");
    // USART2_Printf("AY:");USART2_SendNumber(AY,6);USART2_Printf("\n");
    // USART2_Printf("AZ:");USART2_SendNumber(AZ,6);USART2_Printf("\n");

    // OLED_ShowString(10,1,"AX:");OLED_ShowNum(10,1,AX,6);
    // OLED_ShowString(15,1,"AY:");OLED_ShowNum(15,1,AY,6);
    // OLED_ShowString(20,1,"AZ:");OLED_ShowNum(20,1,AZ,6);

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