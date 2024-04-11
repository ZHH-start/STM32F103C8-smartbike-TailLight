#include "ATGM336H.h"

Receive_GPS_data GNRMC_Info; // USART.c文件中使用的GPS数据结构体

// 结构体初始化，清空全部数据
void ATGM_StructInit()
{
    GNRMC_Info.isGetData   = 0;
    GNRMC_Info.isParseData = 0;
    GNRMC_Info.isUsefull   = 0;

    memset(GNRMC_Info.GPS_Buffer, 0, GPS_Buffer_Length);
    memset(GNRMC_Info.UTCTime, 0, UTCTime_Length);
    memset(GNRMC_Info.latitude, 0, latitude_Length);
    memset(GNRMC_Info.N_S, 0, N_S_Length);
    memset(GNRMC_Info.longitude, 0, longitude_Length);
    memset(GNRMC_Info.E_W, 0, E_W_Length);
}

/*
 *==============================================================================
 *函数名称：Uart_Rece_Pares
 *函数功能：解析串口接收内容
 *输入参数：无
 *返回值：无
 *备  注：无
 *==============================================================================
 */
void ParseGps()
{
    char *subString;
    char *subStringNext;
    char i = 0;
    if (GNRMC_Info.isGetData) {
        GNRMC_Info.isGetData = 0;
        //		printf("\r\n");
        //		printf(GNRMC_Info.GPS_Buffer);

        // 截取数据帧前六部分    							 |对地航速 对地航向  日期
        //$GNRMC,112536.000,A,2322.75023,N,11326.28605,E,|  0.00,   0.00,  100722,,,A*78
        for (i = 0; i <= 6; i++) {
            if (i == 0) {
                if ((subString = strstr(GNRMC_Info.GPS_Buffer, ",")) == NULL) // 如果没有找到逗号
                {
                    return;
                    // ERROR
                }

            } else {
                subString++;
                if ((subStringNext = strstr(subString, ",")) != NULL) {
                    char usefulBuffer[2];
                    switch (i) {
                        case 1:
                            memcpy(GNRMC_Info.UTCTime, subString, subStringNext - subString);
                            break;
                        case 2: {
                            memcpy(usefulBuffer, subString, subStringNext - subString); // 有效标志位
                            if (usefulBuffer[0] == 'A')
                                GNRMC_Info.isUsefull = 1;
                            else if (usefulBuffer[0] == 'V')
                                GNRMC_Info.isUsefull = 0;
                            break;
                        }
                        case 3:
                            memcpy(GNRMC_Info.latitude, subString, subStringNext - subString);
                            break;
                        case 4:
                            memcpy(GNRMC_Info.N_S, subString, subStringNext - subString);
                            break;
                        case 5:
                            memcpy(GNRMC_Info.longitude, subString, subStringNext - subString);
                            break;
                        case 6:
                            memcpy(GNRMC_Info.E_W, subString, subStringNext - subString);
                            break;
                        default:
                            break;
                    }
                    subString = subStringNext;
                }
            }
        }
        GNRMC_Info.isParseData = 1;
    }
}

extern float Lat;
extern float Lon;
extern char dest[23];
void printGpsBuffer()
{
    //$GNRMC,123211.000,A,2322.74250,N,11326.27041,E,3.21,217.19,100722,,,A*7A
    if (GNRMC_Info.isParseData) {
        int i                  = 0;
        GNRMC_Info.isParseData = 0;
        if (GNRMC_Info.isUsefull) {
            float tmp            = 0;
            int j                = 0;
            GNRMC_Info.isUsefull = 0;
            for (i = 0; GNRMC_Info.latitude[i] != '\0'; i++) {

                if (GNRMC_Info.latitude[i] == '.') {
                    continue;
                }
                if (i <= 1) {
                    Lat = (GNRMC_Info.latitude[0] - 48) * 10 + (GNRMC_Info.latitude[1] - 48);
                    // 取出个位和十位
                } else {
                    tmp += (GNRMC_Info.latitude[i] - 48);
                    tmp *= 10;
                }
            }
            for (j = 0; j <= 5; j++) {
                tmp /= 10;
            }
            Lat += tmp / 60;
            // 23 22.74250
            // 23.xxxxx
            int iLat               = 0;
            iLat                   = (int)Lat;
            GNRMC_Info.latitude[0] = iLat / 10 + '0';
            GNRMC_Info.latitude[1] = iLat % 10 + '0';
            GNRMC_Info.latitude[2] = '.';
            Lat -= iLat;
            for (j = 3; j < 10; j++) {
                Lat *= 10;
                iLat                   = (int)Lat;
                GNRMC_Info.latitude[j] = iLat + '0';
                Lat -= iLat;
            }
            tmp = 0;
            // 113.27041
            for (i = 0; GNRMC_Info.longitude[i] != '\0'; i++) {

                if (GNRMC_Info.longitude[i] == '.') {
                    continue;
                }
                if (i <= 2) {
                    Lon = (((GNRMC_Info.longitude[0] - 48) * 10 + (GNRMC_Info.longitude[1] - 48)) * 10) + (GNRMC_Info.longitude[2] - 48);
                    // 取出个位和十位和百位
                } else {
                    tmp += (GNRMC_Info.longitude[i] - 48);
                    tmp *= 10;
                }
            }
            for (j = 0; j <= 5; j++) {
                tmp /= 10;
            }
            int iLon = 0;
            // 113.43784
            Lon += tmp / 60;
            iLon                    = (int)Lon;
            GNRMC_Info.longitude[0] = iLon / 100 + '0';
            GNRMC_Info.longitude[1] = (iLon % 100) / 10 + '0';
            GNRMC_Info.longitude[2] = iLon % 10 + '0';
            GNRMC_Info.longitude[3] = '.';
            Lon -= iLon;
            for (j = 4; j < 11; j++) {
                Lon *= 10;
                iLon                    = (int)Lon;
                GNRMC_Info.longitude[j] = iLon + '0';
                Lon -= iLon;
            }

            dest[8] = dest[10] = dest[20] = ',';
            dest[9]                       = 'N';
            dest[21]                      = 'E';
            dest[22]                      = '\0';
            for (i = 0; i < 22; i++) {
                if (i <= 7)
                    dest[i] = GNRMC_Info.latitude[i];
                if (i >= 11 && i <= 19)
                    dest[i] = GNRMC_Info.longitude[i - 11];
            }

            // printf("\r\ndest = ");
            printf(dest);
            // printf("\r\n");
        } else {
            printf("GPS DATA Is Not Useful!");
        }
    }
}
