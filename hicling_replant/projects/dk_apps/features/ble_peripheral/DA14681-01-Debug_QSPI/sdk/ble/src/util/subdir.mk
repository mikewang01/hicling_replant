################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/vm/dialog/DA1468x_SDK_BTLE_v_1.0.5.885/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/interfaces/ble/src/util/list.c \
E:/vm/dialog/DA1468x_SDK_BTLE_v_1.0.5.885/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/interfaces/ble/src/util/queue.c 

OBJS += \
./sdk/ble/src/util/list.o \
./sdk/ble/src/util/queue.o 

C_DEPS += \
./sdk/ble/src/util/list.d \
./sdk/ble/src/util/queue.d 


# Each subdirectory must supply rules for building sources it contributes
sdk/ble/src/util/list.o: E:/vm/dialog/DA1468x_SDK_BTLE_v_1.0.5.885/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/interfaces/ble/src/util/list.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\projects\dk_apps\features\ble_peripheral\config" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\adapters\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\config" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\include\adapter" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\include\manager" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\config" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\attc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\attm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\atts" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap\gapc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap\gapm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt\gattc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt\gattm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\l2c\l2cc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\l2c\l2cm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp\smpc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp\smpm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\rwble_hl" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\em" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\llc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\lld" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\llm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\rwble" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\profiles" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ea\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\em\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\hci\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\hci\src" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\common\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\dbg\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\gtl\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\gtl\src" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\h4tl\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\ke\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\ke\src" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\nvds\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\rwip\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\ll\armgcc_4_8" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\boot\armgcc_4_8" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\compiler\armgcc_4_8" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\build\ble-full\reg\fw" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\flash" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\reg" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\rf" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\rf\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble_services\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\config" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\system\sys_man\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\free_rtos\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\osal" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\peripherals\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\memory\include" -include"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\projects\dk_apps\features\ble_peripheral\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/ble/src/util/queue.o: E:/vm/dialog/DA1468x_SDK_BTLE_v_1.0.5.885/DA1468x_SDK_BTLE_v_1.0.5.885/sdk/interfaces/ble/src/util/queue.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\projects\dk_apps\features\ble_peripheral\config" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\adapters\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\config" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\include\adapter" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\include\manager" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\config" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\attc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\attm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\att\atts" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap\gapc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gap\gapm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt\gattc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\gatt\gattm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\l2c\l2cc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\l2c\l2cm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp\smpc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\host\smp\smpm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\hl\src\rwble_hl" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\em" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\llc" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\lld" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\controller\llm" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\ll\src\rwble" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ble\profiles" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\ea\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\em\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\hci\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\ip\hci\src" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\common\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\dbg\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\gtl\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\gtl\src" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\h4tl\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\ke\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\ke\src" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\nvds\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\modules\rwip\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\ll\armgcc_4_8" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\boot\armgcc_4_8" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\arch\compiler\armgcc_4_8" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\build\ble-full\reg\fw" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\flash" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\reg" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\rf" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble\src\stack\plf\black_orca\src\driver\rf\api" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\interfaces\ble_services\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\config" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\system\sys_man\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\free_rtos\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\osal" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\peripherals\include" -I"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\sdk\bsp\memory\include" -include"E:\vm\dialog\DA1468x_SDK_BTLE_v_1.0.5.885\DA1468x_SDK_BTLE_v_1.0.5.885\projects\dk_apps\features\ble_peripheral\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


