################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/src/stm32f407xx_RCC_driver.c \
../driver/src/stm32f407xx_adc_driver.c \
../driver/src/stm32f407xx_gpio_driver.c 

OBJS += \
./driver/src/stm32f407xx_RCC_driver.o \
./driver/src/stm32f407xx_adc_driver.o \
./driver/src/stm32f407xx_gpio_driver.o 

C_DEPS += \
./driver/src/stm32f407xx_RCC_driver.d \
./driver/src/stm32f407xx_adc_driver.d \
./driver/src/stm32f407xx_gpio_driver.d 


# Each subdirectory must supply rules for building sources it contributes
driver/src/%.o driver/src/%.su driver/src/%.cyclo: ../driver/src/%.c driver/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -I"D:/005_STM32/36_adc_baremetal/driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-driver-2f-src

clean-driver-2f-src:
	-$(RM) ./driver/src/stm32f407xx_RCC_driver.cyclo ./driver/src/stm32f407xx_RCC_driver.d ./driver/src/stm32f407xx_RCC_driver.o ./driver/src/stm32f407xx_RCC_driver.su ./driver/src/stm32f407xx_adc_driver.cyclo ./driver/src/stm32f407xx_adc_driver.d ./driver/src/stm32f407xx_adc_driver.o ./driver/src/stm32f407xx_adc_driver.su ./driver/src/stm32f407xx_gpio_driver.cyclo ./driver/src/stm32f407xx_gpio_driver.d ./driver/src/stm32f407xx_gpio_driver.o ./driver/src/stm32f407xx_gpio_driver.su

.PHONY: clean-driver-2f-src

