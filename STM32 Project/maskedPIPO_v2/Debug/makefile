################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: maskedPIPO_v2.elf

# Tool invocations
maskedPIPO_v2.elf: $(OBJS) $(USER_OBJS) C:\Users\marku\OneDrive\IT-Sicherheit\10.\ Semester\BA\Code\SMT\ workbench\maskedPIPO_v2/LinkerScript.ld
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft -T"C:/Users/marku/OneDrive/IT-Sicherheit/10. Semester/BA/Code/SMT workbench/maskedPIPO_v2/LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -o "maskedPIPO_v2.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "maskedPIPO_v2.elf" "maskedPIPO_v2.bin"
	arm-none-eabi-size "maskedPIPO_v2.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
