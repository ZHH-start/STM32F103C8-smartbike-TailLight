#include "ADC.h"
#include "Delay.h"
#include "USART.h"

uint16_t AD_Value;
float distemp;

void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
    ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
    ADC_InitStructure.ADC_NbrOfChannel       = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET)
        ;
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET)
        ;
}

uint16_t AD_GetValue(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
        ;
    AD_Value = ADC_GetConversionValue(ADC1);
    return AD_Value;
}

// ������⴫�е��ľ���
float Distance_1(void)
{
    uint32_t temp_val = 0;
    uint8_t t;
    for (t = 0; t < 10; t++) {

        //		temp_val+=ADC_GetConversionValue(ADC1);	//��ȡת�������ͨ��11
        temp_val += AD_GetValue(); // ��ȡת�������ͨ��11

        // ��ʱ5����
        Delay_ms(5);
    }
    temp_val /= 10; // ��ƽ��ֵ���õ����յ�ADCֵ
    //	printf("average_val=%d\r\n",temp_val);
    // USART1_Printf("%d      %d\n",distemp,temp_val);
    distemp = temp_val * 3.3 / 4095;
    // USART1_Printf("%d     %f\n",temp_val,distemp);
    //	printf("voltage= %f\r\n",distemp);//��ӡ��ѹֵ
    // ���ݵ�ѹֵ�������
    distemp = (-13.2 * distemp * distemp * distemp) + 72.84 * distemp * distemp - 140 * distemp + 107.12;
    // USART1_Printf("%f\n",distemp);
    return distemp;
}
