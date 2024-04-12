#include "KEY.h"
#include "Delay.h"
#include "LED.h"
#include "USART.h"

void Key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void Key_Scan(void)
{

    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 1) {
        USART2_SendString("in");
        Delay_ms(10);
        USART1_SendString("in111");
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 1) {
            if (Mode_state < 2) {
                Mode_state++;
            } else
                Mode_state = 0;
            LED_open();
        }
    }
}