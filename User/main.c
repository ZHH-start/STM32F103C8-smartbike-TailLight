#include "headfile.h"                  // Device header

int main(void)
{
	LED_init();
    OLED_Init();
	while (1)
	{
		LED_open();
        Delay_ms(500);
        LED_close();
        Delay_ms(500);
        OLED_ShowString(1,1,"HaHaHaHaHa");
        
	}
}
