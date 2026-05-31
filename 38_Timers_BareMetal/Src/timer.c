#include "stm32f407xx.h"

void LED_Init(void)
{
    GPIO_Handle_t led;

    GPIO_PeriClockControl(GPIOD, ENABLE);

    led.pGPIOx = GPIOD;
    led.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    led.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    led.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    led.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    led.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    GPIO_Init(&led);
}

void TIM4_Init(void)
{
    // Enable timer clock
    TIM4_PCLK_EN();

    // 16 MHz / 16000 = 1 kHz
    TIM4->PSC = 16000 - 1;

    // 1 kHz / 1000 = 1 second
    TIM4->ARR = 1000 - 1;

    // Clear counter
    TIM4->CNT = 0;

    // Start timer
    TIM4->CR1 |= (1 << 0);
}

void TIM4_Delay(void)
{
    // Wait until update event
    while(!(TIM4->SR & (1 << 0)));

    // Clear update flag
    TIM4->SR &= ~(1 << 0);
}

int main(void)
{
    LED_Init();

    TIM4_Init();

    while(1)
    {
        GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_NO_12);

        TIM4_Delay();
    }
}
