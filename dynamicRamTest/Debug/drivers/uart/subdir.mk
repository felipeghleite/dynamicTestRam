################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Data/Workspace\ Kinetis\ IDE/kexx_drv_lib/src/drivers/uart/uart.c 

OBJS += \
./drivers/uart/uart.o 

C_DEPS += \
./drivers/uart/uart.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/uart/uart.o: D:/Data/Workspace\ Kinetis\ IDE/kexx_drv_lib/src/drivers/uart/uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g3 -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/common" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/cpu" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/platforms" -I"D:/Data/Workspace Kinetis IDE/dynamicRamTest/Sources" -I"D:/Data/Workspace Kinetis IDE/dynamicRamTest/Includes" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/acmp" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/adc" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/bitband" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/bme" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/can" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/crc" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/ftm" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/gpio" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/ics" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/iic" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/kbi" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/nvm" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/pit" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/PMC" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/pwt" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/rtc" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/sim" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/spi" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/uart" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers/wdog" -I"D:/Data/Workspace Kinetis IDE/kexx_drv_lib/src/drivers" -std=c99 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


