#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData();
void MPU6050_detect();
void MPU6050_Alarm_init(void);

extern u8 Alarm_open;

#define MPU6050_SMPLRT_DIV   0x19 // �����ǲ����ʣ�����ֵ��0x07(125Hz)
#define MPU6050_CONFIG       0x1A // ��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define MPU6050_GYRO_CONFIG  0x1B // �������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define MPU6050_ACCEL_CONFIG 0x1C // ���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x00(���Լ죬2G��5Hz)

#define MPU6050_ACCEL_XOUT_H 0x3B // X����ٶȸ�λ�Ĵ���
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D // y����ٶȸ�λ�Ĵ���
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F // z����ٶȸ�λ�Ĵ���
#define MPU6050_ACCEL_ZOUT_L 0x40
#define MPU6050_TEMP_OUT_H   0x41
#define MPU6050_TEMP_OUT_L   0x42
#define MPU6050_GYRO_XOUT_H  0x43 // x����ٶȸ�λ�Ĵ���
#define MPU6050_GYRO_XOUT_L  0x44
#define MPU6050_GYRO_YOUT_H  0x45 // y����ٶȸ�λ�Ĵ���
#define MPU6050_GYRO_YOUT_L  0x46
#define MPU6050_GYRO_ZOUT_H  0x47 // z����ٶȸ�λ�Ĵ���
#define MPU6050_GYRO_ZOUT_L  0x48

#define MPU6050_PWR_MGMT_1   0x6B // ��Դ��������ֵ��0x00(��������)
#define MPU6050_PWR_MGMT_2   0x6C
#define MPU6050_ADDRESS      0xD0 // IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ
#define MPU6050_WHO_AM_I     0x75 // IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)

#endif
