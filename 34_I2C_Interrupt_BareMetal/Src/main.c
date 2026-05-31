#include "stm32f407xx.h"
#include <string.h>

#define SLAVE_ADDR  0x68

I2C_Handle_t I2C1Handle;
volatile uint8_t buttonPressed = 0;

/* Delay */
void delay(void)
{
    for(uint32_t i = 0; i < 500000; i++);
}

/* Button Interrupt Handler */
void EXTI0_IRQHandler(void)
{
    // Clear interrupt pending bit
    GPIO_IRQHandling(GPIO_PIN_NO_0);

    buttonPressed = 1;
}

/* GPIO Button Init (PA0) */
void Button_Init(void)
{
    GPIO_Handle_t GPIOBtn;

    GPIOBtn.pGPIOx = GPIOA;
    GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
    GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

    GPIO_Init(&GPIOBtn);

    // IRQ config
    GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, NVIC_IRQ_PR15);
    GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, ENABLE);
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

/* I2C GPIO Init */
void I2C1_GPIOInit(void)
{
    GPIO_Handle_t I2CPins;

    I2CPins.pGPIOx = GPIOB;
    I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
    I2CPins.GPIO_PinConfig.GPIO_PinOPtype = GPIO_OP_TYPE_OD;
    I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // SCL PB6 - 22
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&I2CPins);

    // SDA PB7
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&I2CPins);
}

int main(void)
{
    char msg[] = "Hello ESP32";

    I2C1_GPIOInit();
    I2C1_Init();
    Button_Init();

    I2C_PeripheralControl(I2C1, ENABLE);

    while(1)
    {
        if(buttonPressed)
        {
            delay(); // debounce
            I2C_MasterSendData(&I2C1Handle, (uint8_t*)msg, strlen(msg), SLAVE_ADDR, I2C_DISABLE_SR);
            buttonPressed = 0;
        }
    }
}
