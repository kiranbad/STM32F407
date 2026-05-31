
#include "stm32f407xx.h"
uint16_t adc_value = 0;
/********* Delay *********/
void delay(void)
{
    for(uint32_t i=0;i<500000;i++);
}

/********* GPIO ANALOG INIT (PA0) *********/
void ADC_GPIO_Init(void)
{
    GPIO_Handle_t adc_pin;

    adc_pin.pGPIOx = GPIOA;
    adc_pin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    adc_pin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ANALOG;
    adc_pin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_Init(&adc_pin);
}

int main(void)
{
    ADC_Handle_t adc;

    ADC_GPIO_Init();

    adc.pADCx = ADC1;
    adc.ADC_Config.ADC_Resolution = ADC_RESOLUTION_12BIT;
    adc.ADC_Config.ADC_DataAlign = ADC_DATA_ALIGN_RIGHT;
    adc.ADC_Config.ADC_ScanConvMode = DISABLE;
    adc.ADC_Config.ADC_ContinuousConvMode = ENABLE;

    ADC_Init(&adc);

    ADC1->SQR3 = 0;
    ADC1->SMPR2 |= (7 << 0);

    ADC_PeripheralControl(ADC1, ENABLE);
    delay();

    // Start conversion ONCE before the loop because Continuous mode is ENABLED
    ADC_StartConversion(ADC1);

    while(1)
    {
        // Wait for End of Conversion (EOC flag) in SR register
        while( !(ADC1->SR & (1 << 1)) );

        // Now read the stable, converted value
        adc_value = ADC_Read(ADC1);

        delay();
    }
}
