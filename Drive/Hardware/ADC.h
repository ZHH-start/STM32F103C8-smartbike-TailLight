#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

void AD_Init(void); // A6A7
uint16_t AD_GetValue();
float Distance_1();

extern uint16_t AD_Value;
extern float distemp;

#endif
