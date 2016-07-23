################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/crc16.c \
../src/flash.c \
../src/main.c 

OBJS += \
./src/crc16.o \
./src/flash.o \
./src/main.o 

C_DEPS += \
./src/crc16.d \
./src/flash.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g3 -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\adapters\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\memory\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\config" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\peripherals\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\system\loaders\uartboot\include" -include"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


