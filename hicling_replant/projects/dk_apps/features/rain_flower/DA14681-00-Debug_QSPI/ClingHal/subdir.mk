################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ClingHal/battmgr_hal.c \
../ClingHal/btle_api.c \
../ClingHal/flash_hal.c \
../ClingHal/i2cdev_hal.c \
../ClingHal/periph_hal.c \
../ClingHal/pinmap.c \
../ClingHal/rtc_hal.c \
../ClingHal/spidev_hal.c 

OBJS += \
./ClingHal/battmgr_hal.o \
./ClingHal/btle_api.o \
./ClingHal/flash_hal.o \
./ClingHal/i2cdev_hal.o \
./ClingHal/periph_hal.o \
./ClingHal/pinmap.o \
./ClingHal/rtc_hal.o \
./ClingHal/spidev_hal.o 

C_DEPS += \
./ClingHal/battmgr_hal.d \
./ClingHal/btle_api.d \
./ClingHal/flash_hal.d \
./ClingHal/i2cdev_hal.d \
./ClingHal/periph_hal.d \
./ClingHal/pinmap.d \
./ClingHal/rtc_hal.d \
./ClingHal/spidev_hal.d 


# Each subdirectory must supply rules for building sources it contributes
ClingHal/%.o: ../ClingHal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_D -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\projects\dk_apps\features\rain_flower\config" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\bsp\adapters\include" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\config" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\include" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\include\adapter" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\include\manager" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\config" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\attc" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\attm" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\atts" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap\gapc" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap\gapm" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt\gattc" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt\gattm" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\l2c\l2cc" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\l2c\l2cm" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp\smpc" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp\smpm" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\hl\src\rwble_hl" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\em" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\llc" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\lld" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\llm" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\ll\src\rwble" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ble\profiles" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\ea\api" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\em\api" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\hci\api" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\ip\hci\src" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\modules\common\api" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\modules\dbg\api" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\modules\gtl\api" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\modules\gtl\src" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\modules\h4tl\api" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\modules\ke\api" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\modules\ke\src" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\modules\nvds\api" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\modules\rwip\api" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\ll\armgcc_4_8" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\boot\armgcc_4_8" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\compiler\armgcc_4_8" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\plf\black_orca\src\build\ble-full\reg\fw" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\flash" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\reg" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\rf" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\rf\api" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\interfaces\ble_services\include" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\bsp\include" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\bsp\config" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\bsp\system\sys_man\include" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\bsp\free_rtos\include" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\bsp\osal" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\bsp\peripherals\include" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\sdk\bsp\memory\include" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\projects\dk_apps\features\rain_flower" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\projects\dk_apps\features\rain_flower\ClingHal" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\projects\dk_apps\features\rain_flower\ClingSDK" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\projects\dk_apps\features\rain_flower\ClingSDK\bsp" -I"E:\RAIN_FLOWER\hicling_replant\hicling_replant\projects\dk_apps\features\rain_flower\ClingSDK\task" -include"E:\RAIN_FLOWER\hicling_replant\hicling_replant\projects\dk_apps\features\rain_flower\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


