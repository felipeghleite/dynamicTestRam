################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include drivers/wdog/subdir.mk
-include drivers/uart/subdir.mk
-include drivers/spi/subdir.mk
-include drivers/sim/subdir.mk
-include drivers/rtc/subdir.mk
-include drivers/pit/subdir.mk
-include drivers/nvm/subdir.mk
-include drivers/kbi/subdir.mk
-include drivers/iic/subdir.mk
-include drivers/ics/subdir.mk
-include drivers/gpio/subdir.mk
-include drivers/ftm/subdir.mk
-include drivers/crc/subdir.mk
-include drivers/adc/subdir.mk
-include drivers/acmp/subdir.mk
-include drivers/PMC/subdir.mk
-include cpu/subdir.mk
-include common/subdir.mk
-include Sources/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
dynamicRamTest.siz \


# All Target
all: dynamicRamTest.elf secondary-outputs

# Tool invocations
dynamicRamTest.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g3 -T "D:\Data\Workspace Kinetis IDE\dynamicRamTest\Project_Settings\Linker_Files\MKE02Z64xxx2_flash.ld" -Xlinker --gc-sections -Wl,-Map,"dynamicRamTest.map" -specs=nano.specs -specs=nosys.specs -o "dynamicRamTest.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

dynamicRamTest.siz: dynamicRamTest.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=sysv -x "dynamicRamTest.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(C++_DEPS)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS)$(S_UPPER_DEPS) dynamicRamTest.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
