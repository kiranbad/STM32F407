#include "stm32f407xx.h"
#include "stm32f407xx_spi_driver.h"
#include <string.h>

/*
* AF5 -- SPI2
* PB14 -- MISO
* PB15 -- MOSI
* PB13 -- SCLK
* PB12 --- NSS (manual GPIO)
*/

void delay(void)
{
	for(uint32_t i = 0; i < 500000; i++);
}

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins, NSSpin;

	// SPI Pins
	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCLK -> PB13
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	// MOSI -> PB15
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	// MISO -> PB14
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	// NSS (manual control) -> PB12
	NSSpin.pGPIOx = GPIOB;
	NSSpin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	NSSpin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	NSSpin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	NSSpin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	NSSpin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&NSSpin);

	// CS HIGH initially
	GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_SET);
}

void SPI2_Init(void)
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI2handle);
}

char user_data[] = "Hello world";
char rx_buffer[32];

int main(void)
{
	SPI2_GPIOInits();
	SPI2_Init();

	// Enable SPI peripheral
	SPI_PeripheralControl(SPI2, ENABLE);

	delay();

	while(1)
	{
		// CS LOW (select slave / start communication)
		GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_RESET);

		// Send data
		SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

		// Receive data (loopback)
		SPI_ReceiveData(SPI2, (uint8_t*)rx_buffer, strlen(user_data));

		// Wait until SPI not busy
		while(SPI2->SR & SPI_BUSY_FLAG);

		// CS HIGH (end communication)
		GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, GPIO_PIN_SET);

		// Null terminate received string
		rx_buffer[strlen(user_data)] = '\0';

		delay();
	}
}
