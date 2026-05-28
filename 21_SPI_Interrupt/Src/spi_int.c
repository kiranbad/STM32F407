/*
 * PB13->SCLK
 * PB15->MOSI
 * PB14->MISO
 * PB12->NSS
 */
#include "stm32f407xx.h"
#include <string.h>

SPI_Handle_t SPI2handle;

char user_data[] = "H";
char read_buffer[2];

volatile uint8_t rx_complete_flag = 0;

void delay(void)
{
    for(uint32_t i = 0; i < 500000; i++);
}

void SPI2_GPIOInits(void)
{
    GPIO_Handle_t SPIPins, NSSpin;

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
    SPIPins.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // SCLK
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    // MOSI
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&SPIPins);

    // MISO
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    GPIO_Init(&SPIPins);

    // NSS manual
    NSSpin.pGPIOx = GPIOB;
    NSSpin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    NSSpin.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_PP;
    NSSpin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    NSSpin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    NSSpin.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;

    GPIO_Init(&NSSpin);
    GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, SET);
}

void SPI2_Init(void)
{
    SPI2handle.pSPIx = SPI2;
    SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV64;
    SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

    SPI_Init(&SPI2handle);
}

int main(void)
{
    SPI2_GPIOInits();
    SPI2_Init();

    SPI_IRQInterruptConfig(IRQ_NO_SPI2, ENABLE);
    SPI_IRQPriorityConfig(IRQ_NO_SPI2, 1);

    SPI_SSIConfig(SPI2, ENABLE);
    SPI_PeripheralControl(SPI2, ENABLE);

    while(1)
    {
    	//This tells slave start the communication
        GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, RESET); // NSS LOW

        rx_complete_flag = 0;

        SPI_SendDataReceiveIT(&SPI2handle,
                              (uint8_t*)user_data,
                              (uint8_t*)read_buffer,
                              1);

        // wait for interrupt
        while(rx_complete_flag == 0);

        GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_NO_12, SET); // NSS HIGH

        delay();
    }
}

/* ISR */
void SPI2_IRQHandler(void)
{
    SPI_IRQHandling(&SPI2handle);
}

/* CALLBACK */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
{
    if(AppEv == SPI_EVENT_RX_CMPLT)
    {
        rx_complete_flag = 1;
        // MODIFY received data (proof interrupt executed)
        read_buffer[0] = read_buffer[0] + 1;
    }
}
