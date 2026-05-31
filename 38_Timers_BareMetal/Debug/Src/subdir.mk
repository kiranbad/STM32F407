################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/stm32f407xx_RCC_driver.c \
../Src/stm32f407xx_gpio_driver.c \
../Src/stm32f407xx_timer_driver.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timer.c 

OBJS += \
./Src/stm32f407xx_RCC_driver.o \
./Src/stm32f407xx_gpio_driver.o \
./Src/stm32f407xx_timer_driver.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timer.o 

C_DEPS += \
./Src/stm32f407xx_RCC_driver.d \
./Src/stm32f407xx_gpio_driver.d \
./Src/stm32f407xx_timer_driver.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F407G_DISC1 -DSTM32F4 -DSTM32F407VGTx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/stm32f407xx_RCC_driver.cyclo ./Src/stm32f407xx_RCC_driver.d ./Src/stm32f407xx_RCC_driver.o ./Src/stm32f407xx_RCC_driver.su ./Src/stm32f407xx_gpio_driver.cyclo ./Src/stm32f407xx_gpio_driver.d ./Src/stm32f407xx_gpio_driver.o ./Src/stm32f407xx_gpio_driver.su ./Src/stm32f407xx_timer_driver.cyclo ./Src/stm32f407xx_timer_driver.d ./Src/stm32f407xx_timer_driver.o ./Src/stm32f407xx_timer_driver.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timer.cyclo ./Src/timer.d ./Src/timer.o ./Src/timer.su

.PHONY: clean-Src

