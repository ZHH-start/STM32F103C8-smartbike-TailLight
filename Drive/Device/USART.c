#include "USART.h"
#include "ATGM336H.h"
#include <stdio.h>
#include <stdarg.h>
#include "OLED.h"
#include "LED.h"

// 加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 0
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE {
    int handle;
};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
// 重定义fputc函数
// int fputc(int ch, FILE *f)
// {
// 	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
//     USART1->DR = (u8) ch;
// 	return ch;
// }

// int fputc(int ch, FILE *f)
// {
//     USART1_SendByte(ch);
//     return ch;
// }

#endif

// #if EN_USART1_RX // 如果使能了接收

u8 USART1_RX_BUF[USART1_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.
u16 USART1_RX_STA = 0;            // 接收状态标记

u8 USART2_RX_BUF[USART2_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.
u16 USART2_RX_STA    = 0;         // 接收状态标记
u8 GPS_receive_count = 0;         // 串口接收数据计数


u8 USART3_RX_BUF[USART3_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.
u16 USART3_RX_STA = 0;            // 接收状态标记

// USART3_RX_STA接收状态标志位：
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目

// #endif

uint8_t USART1_RxData;
uint8_t USART1_RxFlag;

uint8_t USART2_RxData;
uint8_t USART2_RxFlag;

uint8_t USART3_RxData;
uint8_t USART3_RxFlag;

void USART1_Init(u32 baud)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // PA9 TX
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA10 RX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate            = baud;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);

    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // NVIC_InitTypeDef NVIC_InitStructure;
    // NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    // NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE); // 使能串口
}

void USART2_Init(u32 baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //|RCC_APB2Periph_AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // 使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // 使能USART2时钟

    // PA2  TX
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; // 复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA3  RX
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);  // 复位串口2
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE); // 停止复位

    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                     // 设置NVIC中断分组2:2位抢占优先级，2位响应优先级   0-3;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn; // 使能串口2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;           // 先占优先级2级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;           // 从优先级2级
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;      // 使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);                                     // 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    USART_InitStructure.USART_BaudRate            = baud;                           // 波特率设置
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            // 8位数据长度
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               // 一个停止位
    USART_InitStructure.USART_Parity              = USART_Parity_No;                /// 奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  // 收发模式

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 开启中断接收
    USART_Cmd(USART2, ENABLE);                     // 使能串口
}

void USART3_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10; // TX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11; // RX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate            = bound;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Init(USART3, &USART_InitStructure); // 初始化串口1
    USART_Cmd(USART3, ENABLE);
    //	USART_ClearFlag(USART3,USART_FLAG_TC);//清除接收标志
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 中断类型——接收中断
}

void USART1_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
}

void USART2_SendByte(uint8_t Byte)
{
    USART_SendData(USART2, Byte);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
        ;
}
 
void USART3_SendByte(uint8_t Byte)
{
    USART_SendData(USART3, Byte);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
        ;
}

void USART1_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++) {
        USART1_SendByte(Array[i]);
    }
}

void USART2_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++) {
        USART2_SendByte(Array[i]);
    }
}

void USART3_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++) {
        USART3_SendByte(Array[i]);
    }
}

void USART1_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) {
        USART1_SendByte(String[i]);
    }
}

void USART2_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) {
        USART2_SendByte(String[i]);
    }
}

void USART3_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) {
        USART3_SendByte(String[i]);
    }
}

uint32_t USART1_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--) {
        Result *= X;
    }
    return Result;
}

void USART1_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++) {
        USART1_SendByte(Number / USART1_Pow(10, Length - i - 1) % 10 + '0');
    }
}

void USART2_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++) {
        USART2_SendByte(Number / USART1_Pow(10, Length - i - 1) % 10 + '0');
    }
}

void USART3_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++) {
        USART3_SendByte(Number / USART1_Pow(10, Length - i - 1) % 10 + '0');
    }
}

void USART1_RX_BUF_clean(void)
{

    for (int i = 0; i <= 180; i++) {
        USART1_RX_BUF[i] = '\0';
    }
}

void USART1_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    USART1_SendString(String);
}

void USART2_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    USART2_SendString(String);
}

void USART3_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    USART3_SendString(String);
}

uint8_t USART1_GetRxFlag(void)
{
    if (USART1_RxFlag == 1) {
        USART1_RxFlag = 0;
        return 1;
    }
    return 0;
}

uint8_t USART2_GetRxFlag(void)
{
    if (USART2_RxFlag == 1) {
        USART2_RxFlag = 0;
        return 1;
    }
    return 0;
}

uint8_t USART3_GetRxFlag(void)
{
    if (USART3_RxFlag == 1) {
        USART3_RxFlag = 0;
        return 1;
    }
    return 0;
}

uint8_t USART1_GetRxData(void)
{
    return USART1_RxData;
}

uint8_t USART2_GetRxData(void)
{
    return USART2_RxData;
}

uint8_t USART3_GetRxData(void)
{
    return USART3_RxData;
}

void USART1_IRQHandler(void) // 串口1中断服务程序
{
    uint8_t Res;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res = USART_ReceiveData(USART1);   // 读取接收到的数据
        if ((USART1_RX_STA & 0x8000) == 0) // 接收未完成(15位不是1)
        {
            if (USART1_RX_STA & 0x4000) // 在下面被赋值，表示接收到了0x0d（通过判断14位1或者0
            {
                if (Res != 0x0a) {
                    USART1_RX_STA = 0; // 0d后面没跟着0a，接收错误,重新开始
                } else {
                    USART1_RX_STA |= 0x8000; // 接收完成了，15位赋1 |=：按位或赋值
                }
            } else // 还没收到0X0D
            {
                if (Res == 0x0d) {
                    USART1_RX_STA |= 0x4000; // 计数器不再增加，14位置1，不再接收，等待0x0a，下一次循环进入上面接收到0d的循环
                } else {
                    USART1_RX_BUF[USART1_RX_STA & 0X3FFF] = Res; // 14 15位清零，并将数据保存在USART1_RX_BUF内
                    USART1_RX_STA++;
                    if (USART1_RX_STA > (USART1_REC_LEN - 1)) USART1_RX_STA = 0; // 接收数据过大,重新开始接收
                }
            }
        }
    }
}

// void USART2_IRQHandler(void) // 串口2中断服务程序
// {
//     u8 Res;
//     if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
//     {
//         Res = USART_ReceiveData(USART2); // 读取接收到的数据

//         if ((USART2_RX_STA & 0x8000) == 0) // 接收未完成（第15位不是1）
//         {
//             if (USART2_RX_STA & 0x4000) // 接收到了0x0d
//             {
//                 if (Res != 0x0a)
//                     USART2_RX_STA = 0; // 接收错误,重新开始
//                 else
//                     USART2_RX_STA |= 0x8000; // 接收完成了
//             } else                           // 还没收到0X0D
//             {
//                 if (Res == 0x0d)
//                     USART2_RX_STA |= 0x4000;
//                 else {
//                     USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res;
//                     USART2_RX_STA++;
//                     if (USART2_RX_STA > (USART2_REC_LEN - 1)) USART2_RX_STA = 0; // 接收数据错误,重新开始接收
//                 }
//             }
//         }
//     }
// }

/*
 *==============================================================================
 *函数名称：USART1_IRQHandler
 *函数功能：串口1中断服务函数
 *输入参数：无
 *返回值：无
 *备  注：无
 *==============================================================================
 */
void USART2_IRQHandler(void)
{
    u8 recContent; // 存储接收内容

    // 如果串口接收到内容
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        recContent = USART_ReceiveData(USART2); // 存储接收内容

        // 如果接收到的是$（$是一帧信息的开始）
        // 保证每接收到新的一帧信息就从缓冲区起始位置开始存储
        if (recContent == '$') {
            GPS_receive_count = 0; // 清零帧信息计数变量
        }

        // 存储接收到的帧信息
        USART2_RX_BUF[GPS_receive_count++] = recContent;

        // 确定是否收到"GPRMC/GNRMC"这一帧数据
        if (USART2_RX_BUF[0] == '$' && USART2_RX_BUF[4] == 'M' && USART2_RX_BUF[5] == 'C') {
            // 接收到换行（接收完了一帧信息）
            if (recContent == '\n') {
                memset(GNRMC_Info.GPS_Buffer, 0, GPS_Buffer_Length);           // 初始化接收帧信息数组
                memcpy(GNRMC_Info.GPS_Buffer, USART2_RX_BUF, GPS_receive_count); // 保存GPRMC/GNRMC这帧的数据
                GNRMC_Info.isGetData = 1;                                          // 接收成功
                GPS_receive_count       = 0;                                          // 清零接收帧信息接收计数变量
                memset(USART2_RX_BUF, 0, USART2_REC_LEN);                             // 清空串口1接收Buf
            }
        }

        // 如果接收内容超出最大长度，不再继续接收
        if (GPS_receive_count >= USART2_REC_LEN) {
            GPS_receive_count = USART2_REC_LEN;
        }
    }
}

// void USART3_IRQHandler(void) // 串口3中断服务程序
// {
//     u8 Res;
//     if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
//     {
//         Res = USART_ReceiveData(USART3); // 读取接收到的数据

//         if ((USART3_RX_STA & 0x8000) == 0) // 接收未完成
//         {
//             if (USART3_RX_STA & 0x4000) // 接收到了0x0d
//             {
//                 if (Res != 0x0a)
//                     USART3_RX_STA = 0; // 接收错误,重新开始
//                 else
//                     USART3_RX_STA |= 0x8000; // 接收完成了
//             } else                           // 还没收到0X0D
//             {
//                 if (Res == 0x0d)
//                     USART3_RX_STA |= 0x4000;
//                 else {
//                     USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = Res;
//                     USART3_RX_STA++;
//                     if (USART3_RX_STA > (USART3_REC_LEN - 1)) USART3_RX_STA = 0; // 接收数据错误,重新开始接收
//                 }
//             }
//         }
//     }
// }
