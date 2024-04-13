#include "ATGM336H.h"
#include "USART.h"

Receive_GPS_data GNRMC_Info; // USART.c文件中使用的GPS数据结构体

float Lat; // 纬度的实际值
float Lon; // 经度的实际值
char dest[23];

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
 *函数功能：解析串口接收的数据，分别将数据保存到结构体GNRMC_Info内各项
 *输入参数：无
 *返回值：无
 *备  注：无
 *==============================================================================
 */
void ParseGps()
{
    char *subString;     // 字符串逗号指针
    char *subStringNext; // 字符串下一个逗号指针
    char i = 0;
    if (GNRMC_Info.isGetData) {
        GNRMC_Info.isGetData = 0;
        //		printf("\r\n");
        //		printf(GNRMC_Info.GPS_Buffer);

        // 截取数据帧前六部分    							 |对地航速 对地航向  日期
        //$GNRMC,112536.000,A,2322.75023,N,11326.28605,E,|  0.00,   0.00,  100722,,,A*78
        for (i = 0; i <= 6; i++) {
            if (i == 0) {
                if ((subString = strstr(GNRMC_Info.GPS_Buffer, ",")) == NULL) // 扫一遍字符串并拿到逗号指针
                {
                    return; // 如果没有找到逗号则直接返回
                }
            } else {
                subString++;                                            // 扫GPS_Buffer下一个数据
                if ((subStringNext = strstr(subString, ",")) != NULL) { // 扫一次，拿到下一个逗号指针
                    char usefulBuffer[2];
                    switch (i) {
                        case 1:
                            memcpy(GNRMC_Info.UTCTime, subString, subStringNext - subString); // 第一个数据
                            break;
                        case 2: {
                            memcpy(usefulBuffer, subString, subStringNext - subString); // 第二个数据
                            if (usefulBuffer[0] == 'A')
                                GNRMC_Info.isUsefull = 1;
                            else if (usefulBuffer[0] == 'V')
                                GNRMC_Info.isUsefull = 0;
                            break;
                        }
                        case 3:
                            memcpy(GNRMC_Info.latitude, subString, subStringNext - subString); // 第三个数据
                            break;
                        case 4:
                            memcpy(GNRMC_Info.N_S, subString, subStringNext - subString); // 第四个数据
                            break;
                        case 5:
                            memcpy(GNRMC_Info.longitude, subString, subStringNext - subString); // 第五个
                            break;
                        case 6:
                            memcpy(GNRMC_Info.E_W, subString, subStringNext - subString); // 第六个
                            break;
                        default:
                            break;
                    }
                    subString = subStringNext;
                }
            }
        }
        GNRMC_Info.isParseData = 1;
        // USART1_Printf("receive messages");
    }
}

// 转换GPS数据到标准WGS-84坐标系（dd.dddd格式）可在https://old.openluat.com/GPS-Offset.html内手动填入解析
void printGpsBuffer()
{
    //$GNRMC,123211.000,A,2322.74250,N,11326.27041,E,3.21,217.19,100722,,,A*7A
    if (GNRMC_Info.isParseData) { // 如果解析完成

        int i                  = 0;
        GNRMC_Info.isParseData = 0; // 清空解析完成标志位
        if (GNRMC_Info.isUsefull) { // 如果信息有效
            float tmp            = 0;
            int j                = 0;
            GNRMC_Info.isUsefull = 0;                          // 清空有效情况
            for (i = 0; GNRMC_Info.latitude[i] != '\0'; i++) { // 对于接收的纬度信息
                if (GNRMC_Info.latitude[i] == '.') {           // 跳过.符号
                    continue;
                }
                if (i <= 1) {
                    Lat = (GNRMC_Info.latitude[0] - 48) * 10 + (GNRMC_Info.latitude[1] - 48); // 取出个位和十位拼起来
                } else {
                    tmp += (GNRMC_Info.latitude[i] - 48); // 取出小数点，一位一位拼起来
                    tmp *= 10;                            // 每拿到一位就整体*10，左移一位
                }
            }
            for (j = 0; j <= 5; j++) {
                tmp /= 10; // 把小数点后面的数字拿到后用浮点数存起来
            }
            Lat += tmp / 60; // 数据转换：浮点数要除以60再合并
            // 23 22.74250
            // 23.xxxxx
            int iLat               = 0;
            iLat                   = (int)Lat;
            GNRMC_Info.latitude[0] = iLat / 10 + '0';
            GNRMC_Info.latitude[1] = iLat % 10 + '0';
            GNRMC_Info.latitude[2] = '.';
            Lat -= iLat; // Lat整数位再次转换成字符
            for (j = 3; j < 10; j++) {
                Lat *= 10;                           // 左移数据（小数放到整数位置）
                iLat                   = (int)Lat;   // 提出整数位
                GNRMC_Info.latitude[j] = iLat + '0'; // 转成字符型放回
                Lat -= iLat;                         // 去掉已经转化的整数
            }
            tmp = 0;
            // 113.27041-示例数据

            // 处理纬度完成

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
            } // 处理完成经度

            dest[8] = dest[10] = dest[20] = ',';
            dest[9]                       = 'N';
            dest[21]                      = 'E';
            dest[22]                      = '\0';
            for (i = 0; i < 22; i++) {
                if (i <= 7)
                    dest[i] = GNRMC_Info.latitude[i];
                if (i >= 11 && i <= 19)
                    dest[i] = GNRMC_Info.longitude[i - 11];
            } // 将几个关键数据合到一起通过串口发送

            // printf("\r\ndest = ");
            // printf(dest);
            USART1_SendString(dest);
            OLED_ShowString(20, 1, "Send GPS done!");
            // printf("\r\n");
        } else {
            OLED_ShowString(20, 1, "GPS not found..."); // 返回信息无效
        }
    }
}
