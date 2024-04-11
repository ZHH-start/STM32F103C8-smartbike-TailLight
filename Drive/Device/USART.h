#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"

#define USART1_REC_LEN 200 // �����������ֽ��� 200
#define USART2_REC_LEN 70 // �����������ֽ��� 200
#define USART3_REC_LEN 200 // �����������ֽ��� 200

// #define EN_USART1_RX   1 // ʹ�ܣ�1��/��ֹ��0������1����

extern u8 USART1_RX_BUF[USART1_REC_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART1_RX_STA;                // ����״̬���

extern u8 USART2_RX_BUF[USART2_REC_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART2_RX_STA;                // ����״̬���

extern u8 USART3_RX_BUF[USART3_REC_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART3_RX_STA;                // ����״̬���



void USART1_Init(u32 baud); // A9--TX,A10--RX
void USART2_Init(u32 baud); // A2--TX,A3--RX
void USART3_Init(u32 baud); // B10--TX,B11--RX

void USART1_SendByte(uint8_t Byte); // �����ַ�
void USART2_SendByte(uint8_t Byte);
void USART3_SendByte(uint8_t Byte);

void USART1_SendArray(uint8_t *Array, uint16_t Length); //��������
void USART2_SendArray(uint8_t *Array, uint16_t Length);
void USART3_SendArray(uint8_t *Array, uint16_t Length);

void USART1_SendString(char *String); // �����ַ���
void USART2_SendString(char *String);
void USART3_SendString(char *String);

void USART1_SendNumber(uint32_t Number, uint8_t Length); //��������
void USART2_SendNumber(uint32_t Number, uint8_t Length);
void USART3_SendNumber(uint32_t Number, uint8_t Length);

void USART1_RX_BUF_clean(void);

void USART1_Printf(char *format, ...); //print
void USART2_Printf(char *format, ...);
void USART3_Printf(char *format, ...);

uint8_t USART1_GetRxFlag(void);
uint8_t USART1_GetRxData(void);

uint8_t USART2_GetRxFlag(void);
uint8_t USART2_GetRxData(void);

uint8_t USART3_GetRxFlag(void);
uint8_t USART3_GetRxData(void);

#endif
