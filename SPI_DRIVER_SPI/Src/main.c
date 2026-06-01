#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f40fxx_spi_driver.h"
#include <string.h>

#define SD_CS_LOW()   GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, RESET)
#define SD_CS_HIGH()  GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, SET)

SPI_Handle_t SPI2handle;

/* Delay */
void delay(void)
{
	for(uint32_t i = 0; i < 500000; i++);
}

/* SPI TxRx */
uint8_t SPI_TxRx(uint8_t data)
{
	uint8_t rx;
	SPI_TransmitReceive(SPI2, &data, &rx, 1);
	return rx;
}

/* Send dummy clocks */
void SD_DummyClocks(void)
{
	for(int i = 0; i < 20; i++) // 160 clocks
		SPI_TxRx(0xFF);
}

/* Send Command */
uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	uint8_t response;

	SPI_TxRx(0x40 | cmd);
	SPI_TxRx(arg >> 24);
	SPI_TxRx(arg >> 16);
	SPI_TxRx(arg >> 8);
	SPI_TxRx(arg);
	SPI_TxRx(crc);

	// Wait for response
	for(int i = 0; i < 100; i++)
	{
		response = SPI_TxRx(0xFF);
		if(response != 0xFF) break;
	}

	return response;
}

/* SD Initialization */
void SD_Init(void)
{
	uint8_t res;

	SD_CS_HIGH();
	SD_DummyClocks();

	// CMD0
	do {
		SD_CS_LOW();
		res = SD_SendCommand(0, 0, 0x95);
		SD_CS_HIGH();
		SPI_TxRx(0xFF);
	} while(res != 0x01);

	// CMD8
	SD_CS_LOW();
	res = SD_SendCommand(8, 0x1AA, 0x87);

	// Read R7 response (4 bytes)
	for(int i = 0; i < 4; i++)
		SPI_TxRx(0xFF);

	SD_CS_HIGH();
	SPI_TxRx(0xFF);

	// ACMD41
	do {
		SD_CS_LOW();
		SD_SendCommand(55, 0, 0xFF);
		SD_CS_HIGH();
		SPI_TxRx(0xFF);

		SD_CS_LOW();
		res = SD_SendCommand(41, 0x40000000, 0xFF);
		SD_CS_HIGH();
		SPI_TxRx(0xFF);

	} while(res != 0x00);
}

/* Write Block (512 bytes) */
uint8_t SD_WriteBlock(uint32_t blockAddr, uint8_t *data)
{
	uint8_t response;

	SD_CS_LOW();

	// CMD24
	response = SD_SendCommand(24, blockAddr, 0xFF);
	if(response != 0x00)
	{
		SD_CS_HIGH();
		return response;
	}

	SPI_TxRx(0xFE); // Start token

	for(int i = 0; i < 512; i++)
	{
		SPI_TxRx(data[i]);
	}

	// Dummy CRC
	SPI_TxRx(0xFF);
	SPI_TxRx(0xFF);

	// Data response
	response = SPI_TxRx(0xFF);

	// Wait until card not busy
	while(SPI_TxRx(0xFF) != 0xFF);

	SD_CS_HIGH();
	SPI_TxRx(0xFF);

	return response;
}

/* GPIO Init */
void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins, NSSpin;

	GPIO_PeriClockControl(GPIOB, ENABLE);

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCK PB13
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	// MOSI PB15
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	// MISO PB14
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	// CS PB12
	NSSpin.pGPIOx = GPIOB;
	NSSpin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	NSSpin.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
	NSSpin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	NSSpin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	NSSpin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Init(&NSSpin);

	SD_CS_HIGH();
}

/* SPI Init */
void SPI2_Init(void)
{
	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32; // slow init
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI2handle);

	SPI_SSIConfig(SPI2, ENABLE);
	SPI_PeripheralControl(SPI2, ENABLE);
}

/* MAIN */
int main(void)
{
	SPI2_GPIOInits();
	SPI2_Init();

	delay();

	SD_Init();

	uint8_t buffer[512] = {0};

	strcpy((char*)buffer, "Hello STM32 SD Card");

	SD_WriteBlock(0x00000000, buffer);

	while(1);
}
