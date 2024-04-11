#ifndef _ATGM336H_H
#define _ATGM336H_H
#include "stm32f10x.h"

#define GPS_Buffer_Length 80 // 结构体buffer大小
#define UTCTime_Length    11
#define latitude_Length   11
#define longitude_Length  12
#define N_S_Length        2
#define E_W_Length        2

typedef struct {
    char isGetData;   // gps数据接收成功标志位，1成功0失败
    char isParseData; // 解析完成标志位，1结束0正在进行

    char GPS_Buffer[GPS_Buffer_Length]; // gps接收帧信息数组

    char UTCTime[UTCTime_Length];     // 第一个数据：UTC时间
    char isUsefull;                   // 第二个数据：信息有效标志位。1有效0无效
    char latitude[latitude_Length];   // 第三个数据：纬度
    char N_S[N_S_Length];             // 第四个数据：纬度方向：N北S南
    char longitude[longitude_Length]; // 第五个数据：经度
    char E_W[E_W_Length];             // 第六个数据：经度方向：E东W西

} Receive_GPS_data;

extern Receive_GPS_data GNRMC_Info;

void ATGM_StructInit();
void ParseGps();
void printGpsBuffer();

#endif
