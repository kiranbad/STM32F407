/*
 * 004_spi_tx_testing.c
 *
 *  Created on: Apr 29, 2026
 *      Author: 91762
 */
#include "stm32f407xx.h"
#include<string.h>
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"

/*
 * PB14--> SPI2 MISO
 * PB15--> SPI2 MOSI
 * PB13--> SPI2_SCLK
 * PB12--> SPI2_NSS
 * ALT function mode : 5
 */

/*
void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType =GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed =GPIO_SPEED_FAST;


	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
//	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
//	GPIO_Init(&SPIPins);

	//NSS
//	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
//	GPIO_Init(&SPIPins);

}

void SPI2_Inits()
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode =SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;// generates sclk of 8MHz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN; // software s
	SPI2_Inits(&SPI2handle);
}
int main()
{
	char user_data[] = "Hello world";
	//this function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	//this function is used to initialize the SPI2 peripheral parameters
	//SPI2_Inits();

	SPI_PeripheralControl(SPI2,ENABLE);

	SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));
	while(1);
	return 0;
}
*/

void SPI2_Inits()
{
	SPI_PeriClockControl(SPI2, ENABLE);
    SPI_Handle_t SPI2handle;

    SPI2handle.pSPIx = SPI2;
    SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_HD; // TX only
    SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
    SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

    SPI_Init(&SPI2handle);

    // ✅ VERY IMPORTANT
    SPI2->CR1 |= (1 << SPI_CR1_SSI);
}


void SPI2_GPIOInits(void)
{
	GPIO_PeriClockControl(GPIOB, ENABLE);
    GPIO_Handle_t SPIPins;

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // SCLK
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    // MOSI
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&SPIPins);

    // 👉 NSS as GPIO OUTPUT (manual control)
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIO_Init(&SPIPins);
}


int main(void)
{
    char user_data[] = "Hello ESP32";

    SPI2_GPIOInits();
    SPI2_Inits();

    SPI_PeripheralControl(SPI2, ENABLE);

    while(1)
    {
        // CS LOW
        GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_RESET);

        SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

        // wait until SPI not busy
        while(SPI2->SR & SPI_BUSY_FLAG);

        // CS HIGH
        GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_SET);

        for(int i = 0; i < 1000000; i++); // delay
    }
}
