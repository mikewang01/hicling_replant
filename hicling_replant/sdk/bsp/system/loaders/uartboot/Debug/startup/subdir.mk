################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/DA1468x_SDK_BTLE/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/bsp/startup/config.c \
E:/DA1468x_SDK_BTLE/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/bsp/startup/system_ARMCM0.c 

S_UPPER_SRCS += \
E:/DA1468x_SDK_BTLE/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/bsp/startup/startup_ARMCM0.S \
E:/DA1468x_SDK_BTLE/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/bsp/startup/vector_table.S 

OBJS += \
./startup/config.o \
./startup/startup_ARMCM0.o \
./startup/system_ARMCM0.o \
./startup/vector_table.o 

S_UPPER_DEPS += \
./startup/startup_ARMCM0.d \
./startup/vector_table.d 

C_DEPS += \
./startup/config.d \
./startup/system_ARMCM0.d 


# Each subdirectory must supply rules for building sources it contributes
startup/config.o: E:/DA1468x_SDK_BTLE/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/bsp/startup/config.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g3 -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\adapters\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\memory\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\config" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\peripherals\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\system\loaders\uartboot\include" -include"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup/startup_ARMCM0.o: E:/DA1468x_SDK_BTLE/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/bsp/startup/startup_ARMCM0.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g3 -x assembler-with-cpp -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\config" -include"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup/system_ARMCM0.o: E:/DA1468x_SDK_BTLE/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/bsp/startup/system_ARMCM0.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g3 -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\adapters\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\memory\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\config" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\peripherals\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\system\loaders\uartboot\include" -include"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup/vector_table.o: E:/DA1468x_SDK_BTLE/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/bsp/startup/vector_table.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g3 -x assembler-with-cpp -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\config" -include"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


