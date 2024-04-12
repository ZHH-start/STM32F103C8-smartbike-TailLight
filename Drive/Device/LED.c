#include "LED.h"

void LED_open()
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    // GPIO_SetBits(GPIOA, GPIO_Pin_12);
}

void LED_close()
{
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
    // GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}

void LED_Init()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // GPIO_InitTypeDef GPIO_InitStructure;
    // GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    // GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    // GPIO_Init(GPIOA, &GPIO_InitStructure);

    LED_close();
}