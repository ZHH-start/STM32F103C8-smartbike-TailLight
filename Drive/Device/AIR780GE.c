#include "AIR780GE.h"
#include "USART.h"

void AIR780GE_Init(){
    USART2_Init(115200);
    USART2_Printf("config,get,firmwarever\r\n"); // ²éÑ¯¹Ì¼þ
    

}