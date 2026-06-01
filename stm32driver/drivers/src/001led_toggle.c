/*
 * 001led_toggle.c
 *
 *  Created on: Apr 16, 2026
 *      Author: 91762
 */
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include <stdint.h>
void delay(void)
{
	for(uint32_t i=0; i<500000;i++);
}
int main(void)
{
	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	//Open drain no led blink
//	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
//	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	//Blink with less intensity
//	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
//	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	//Open drain led blink with normal intensity.
		GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

//	uint32_t *pRCC = (uint32_t*)0x40023830U;
//
//	*pRCC |= (0x1 << 3);
//
//	GPIOD->MODER |= (1 << 24);

	GPIO_PeriClockControl(GPIOD,ENABLE);
	GPIO_Init(&GpioLed);
	while(1)
	{
		GPIO_ToggleOutputPin(GPIOD,GPIO_PIN_NO_13);
		delay();
	}
	return 0;
}


