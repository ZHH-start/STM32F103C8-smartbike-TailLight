#include "KEY.h"
#include "Delay.h"
#include "LED.h"
#include "USART.h"

void Key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure1;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure1.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure1.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure1);
}

void Key_Scan(void)
{
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 1) {
        Delay_ms(20);
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 1)
            USART2_SendString("in");
            if (Mode_state<2)
            {
                Mode_state++;
            }
            else
                Mode_state = 0;

            LED_open();
    }
}