################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ClingHal/batt_hal.c \
../ClingHal/ble_hal.c \
../ClingHal/flash_hal.c \
../ClingHal/periph_hal.c \
../ClingHal/rtc_hal.c \
../ClingHal/spi_hal.c 

OBJS += \
./ClingHal/batt_hal.o \
./ClingHal/ble_hal.o \
./ClingHal/flash_hal.o \
./ClingHal/periph_hal.o \
./ClingHal/rtc_hal.o \
./ClingHal/spi_hal.o 

C_DEPS += \
./ClingHal/batt_hal.d \
./ClingHal/ble_hal.d \
./ClingHal/flash_hal.d \
./ClingHal/periph_hal.d \
./ClingHal/rtc_hal.d \
./ClingHal/spi_hal.d 


# Each subdirectory must supply rules for building sources it contributes
ClingHal/%.o: ../ClingHal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_D -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\projects\dk_apps\features\rain_flower\config" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\adapters\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\config" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\include\adapter" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\include\manager" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\config" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\attc" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\attm" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\atts" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap\gapc" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap\gapm" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt\gattc" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt\gattm" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\l2c\l2cc" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\l2c\l2cm" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp\smpc" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp\smpm" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\rwble_hl" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\em" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\llc" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\lld" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\llm" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\rwble" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\profiles" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ea\api" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\em\api" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\hci\api" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\hci\src" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\common\api" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\dbg\api" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\gtl\api" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\gtl\src" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\h4tl\api" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\ke\api" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\ke\src" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\nvds\api" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\rwip\api" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\ll\armgcc_4_8" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\boot\armgcc_4_8" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\compiler\armgcc_4_8" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\build\ble-full\reg\fw" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\flash" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\reg" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\rf" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\rf\api" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble_services\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\config" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\system\sys_man\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\free_rtos\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\osal" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\peripherals\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\memory\include" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\projects\dk_apps\features\rain_flower" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\projects\dk_apps\features\rain_flower\ClingHal" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\projects\dk_apps\features\rain_flower\ClingSDK" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\projects\dk_apps\features\rain_flower\ClingSDK\bsp" -I"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\projects\dk_apps\features\rain_flower\ClingSDK\task" -include"E:\DA1468x_SDK_BTLE\DA1468x_SDK_BTLE_v_1.0.5.885\projects\dk_apps\features\rain_flower\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


