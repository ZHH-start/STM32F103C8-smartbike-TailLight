#include "USART.h"
#include "ATGM336H.h"
#include <stdio.h>
#include <stdarg.h>
#include "OLED.h"
#include "LED.h"

// �������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 0
#pragma import(__use_no_semihosting)
// ��׼����Ҫ��֧�ֺ���
struct __FILE {
    int handle;
};

FILE __stdout;
// ����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}
// �ض���fputc����
// int fputc(int ch, FILE *f)
// {
// 	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
//     USART1->DR = (u8) ch;
// 	return ch;
// }

// int fputc(int ch, FILE *f)
// {
//     USART1_SendByte(ch);
//     return ch;
// }

#endif

// #if EN_USART1_RX // ���ʹ���˽���

u8 USART1_RX_BUF[USART1_REC_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�.
u16 USART1_RX_STA = 0;            // ����״̬���

u8 USART2_RX_BUF[USART2_REC_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�.
u16 USART2_RX_STA    = 0;         // ����״̬���
u8 GPS_receive_count = 0;         // ���ڽ������ݼ���


u8 USART3_RX_BUF[USART3_REC_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�.
u16 USART3_RX_STA = 0;            // ����״̬���

// USART3_RX_STA����״̬��־λ��
// bit15��	������ɱ�־
// bit14��	���յ�0x0d
// bit13~0��	���յ�����Ч�ֽ���Ŀ

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

    USART_Cmd(USART1, ENABLE); // ʹ�ܴ���
}

void USART2_Init(u32 baud)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //|RCC_APB2Periph_AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // ʹ��GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // ʹ��USART2ʱ��

    // PA2  TX
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; // ��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA3  RX
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // ��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);  // ��λ����2
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE); // ֹͣ��λ

    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                     // ����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�   0-3;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn; // ʹ�ܴ���2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;           // ��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;           // �����ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;      // ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);                                     // ����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    USART_InitStructure.USART_BaudRate            = baud;                           // ����������
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            // 8λ���ݳ���
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               // һ��ֹͣλ
    USART_InitStructure.USART_Parity              = USART_Parity_No;                /// ��żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������������
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  // �շ�ģʽ

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // �����жϽ���
    USART_Cmd(USART2, ENABLE);                     // ʹ�ܴ���
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

    USART_Init(USART3, &USART_InitStructure); // ��ʼ������1
    USART_Cmd(USART3, ENABLE);
    //	USART_ClearFlag(USART3,USART_FLAG_TC);//������ձ�־
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // �ж����͡��������ж�
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

void USART1_IRQHandler(void) // ����1�жϷ������
{
    uint8_t Res;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // �����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res = USART_ReceiveData(USART1);   // ��ȡ���յ�������
        if ((USART1_RX_STA & 0x8000) == 0) // ����δ���(15λ����1)
        {
            if (USART1_RX_STA & 0x4000) // �����汻��ֵ����ʾ���յ���0x0d��ͨ���ж�14λ1����0
            {
                if (Res != 0x0a) {
                    USART1_RX_STA = 0; // 0d����û����0a�����մ���,���¿�ʼ
                } else {
                    USART1_RX_STA |= 0x8000; // ��������ˣ�15λ��1 |=����λ��ֵ
                }
            } else // ��û�յ�0X0D
            {
                if (Res == 0x0d) {
                    USART1_RX_STA |= 0x4000; // �������������ӣ�14λ��1�����ٽ��գ��ȴ�0x0a����һ��ѭ������������յ�0d��ѭ��
                } else {
                    USART1_RX_BUF[USART1_RX_STA & 0X3FFF] = Res; // 14 15λ���㣬�������ݱ�����USART1_RX_BUF��
                    USART1_RX_STA++;
                    if (USART1_RX_STA > (USART1_REC_LEN - 1)) USART1_RX_STA = 0; // �������ݹ���,���¿�ʼ����
                }
            }
        }
    }
}

// void USART2_IRQHandler(void) // ����2�жϷ������
// {
//     u8 Res;
//     if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // �����ж�(���յ������ݱ�����0x0d 0x0a��β)
//     {
//         Res = USART_ReceiveData(USART2); // ��ȡ���յ�������

//         if ((USART2_RX_STA & 0x8000) == 0) // ����δ��ɣ���15λ����1��
//         {
//             if (USART2_RX_STA & 0x4000) // ���յ���0x0d
//             {
//                 if (Res != 0x0a)
//                     USART2_RX_STA = 0; // ���մ���,���¿�ʼ
//                 else
//                     USART2_RX_STA |= 0x8000; // ���������
//             } else                           // ��û�յ�0X0D
//             {
//                 if (Res == 0x0d)
//                     USART2_RX_STA |= 0x4000;
//                 else {
//                     USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res;
//                     USART2_RX_STA++;
//                     if (USART2_RX_STA > (USART2_REC_LEN - 1)) USART2_RX_STA = 0; // �������ݴ���,���¿�ʼ����
//                 }
//             }
//         }
//     }
// }

/*
 *==============================================================================
 *�������ƣ�USART1_IRQHandler
 *�������ܣ�����1�жϷ�����
 *�����������
 *����ֵ����
 *��  ע����
 *==============================================================================
 */
void USART2_IRQHandler(void)
{
    u8 recContent; // �洢��������

    // ������ڽ��յ�����
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        recContent = USART_ReceiveData(USART2); // �洢��������

        // ������յ�����$��$��һ֡��Ϣ�Ŀ�ʼ��
        // ��֤ÿ���յ��µ�һ֡��Ϣ�ʹӻ�������ʼλ�ÿ�ʼ�洢
        if (recContent == '$') {
            GPS_receive_count = 0; // ����֡��Ϣ��������
        }

        // �洢���յ���֡��Ϣ
        USART2_RX_BUF[GPS_receive_count++] = recContent;

        // ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
        if (USART2_RX_BUF[0] == '$' && USART2_RX_BUF[4] == 'M' && USART2_RX_BUF[5] == 'C') {
            // ���յ����У���������һ֡��Ϣ��
            if (recContent == '\n') {
                memset(GNRMC_Info.GPS_Buffer, 0, GPS_Buffer_Length);           // ��ʼ������֡��Ϣ����
                memcpy(GNRMC_Info.GPS_Buffer, USART2_RX_BUF, GPS_receive_count); // ����GPRMC/GNRMC��֡������
                GNRMC_Info.isGetData = 1;                                          // ���ճɹ�
                GPS_receive_count       = 0;                                          // �������֡��Ϣ���ռ�������
                memset(USART2_RX_BUF, 0, USART2_REC_LEN);                             // ��մ���1����Buf
            }
        }

        // ����������ݳ�����󳤶ȣ����ټ�������
        if (GPS_receive_count >= USART2_REC_LEN) {
            GPS_receive_count = USART2_REC_LEN;
        }
    }
}

// void USART3_IRQHandler(void) // ����3�жϷ������
// {
//     u8 Res;
//     if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) // �����ж�(���յ������ݱ�����0x0d 0x0a��β)
//     {
//         Res = USART_ReceiveData(USART3); // ��ȡ���յ�������

//         if ((USART3_RX_STA & 0x8000) == 0) // ����δ���
//         {
//             if (USART3_RX_STA & 0x4000) // ���յ���0x0d
//             {
//                 if (Res != 0x0a)
//                     USART3_RX_STA = 0; // ���մ���,���¿�ʼ
//                 else
//                     USART3_RX_STA |= 0x8000; // ���������
//             } else                           // ��û�յ�0X0D
//             {
//                 if (Res == 0x0d)
//                     USART3_RX_STA |= 0x4000;
//                 else {
//                     USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = Res;
//                     USART3_RX_STA++;
//                     if (USART3_RX_STA > (USART3_REC_LEN - 1)) USART3_RX_STA = 0; // �������ݴ���,���¿�ʼ����
//                 }
//             }
//         }
//     }
// }
