#include "stm32f407xx.h"
#include <string.h>

#define SLAVE_ADDR  0x68

I2C_Handle_t I2C1Handle;

/* Delay */
void delay(void)
{
    for(uint32_t i = 0; i < 500000; i++);
}

/* I2C GPIO Init (PB6=SCL, PB7=SDA) */
void I2C1_GPIOInit(void)
{
    GPIO_Handle_t I2CPins;

    GPIO_PeriClockControl(GPIOB, ENABLE);

    I2CPins.pGPIOx = GPIOB;
    I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
    I2CPins.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_OD;
    I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // SCL PB6
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&I2CPins);

    // SDA PB7
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&I2CPins);
}

/* I2C Init */
void I2C1_Init(void)
{
    I2C1Handle.pI2Cx = I2C1;
    I2C1Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
    I2C1Handle.I2C_Config.I2C_DeviceAddress = 0x61;
    I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

    I2C_Init(&I2C1Handle);
}

/* I2C Event Interrupt Handler */
void I2C1_EV_IRQHandler(void)
{
    I2C_EV_IRQHandling(&I2C1Handle);
}

/* I2C Error Interrupt Handler */
void I2C1_ER_IRQHandler(void)
{
    I2C_ER_IRQHandling(&I2C1Handle);
}

/* Application Callback */
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t AppEv)
{
    if(AppEv == I2C_EV_TX_CMPLT)
    {
        // Transmission complete
        // You can toggle LED or debug here
    }
}

int main(void)
{
    char msg[] = "Hello ESP32";

    I2C1_GPIOInit();
    I2C1_Init();

    // Enable I2C peripheral
    I2C_PeripheralControl(I2C1, ENABLE);

    // 🔥 Enable NVIC interrupts
    I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
    I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

    while(1)
    {
        // Start interrupt-based transmission
        if(I2C_MasterSendDataIT(&I2C1Handle, (uint8_t*)msg, strlen(msg), SLAVE_ADDR, I2C_DISABLE_SR) == I2C_READY)
        {
            // Transmission started
        }

        delay();
    }
}
