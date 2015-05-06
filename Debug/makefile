################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include funciones/nodo/subdir.mk
-include funciones/marta/subdir.mk
-include funciones/job/subdir.mk
-include funciones/filesystem/subdir.mk
-include funciones/commons/collections/subdir.mk
-include funciones/commons/subdir.mk
-include funciones/Debug/nodo/subdir.mk
-include funciones/Debug/marta/subdir.mk
-include funciones/Debug/job/subdir.mk
-include Nodo/src/subdir.mk
-include Marta/src/subdir.mk
-include Job/src/subdir.mk
-include FileSystem/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libFunciones\ de\ ArchConfig.so

# Tool invocations
libFunciones\ de\ ArchConfig.so: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -shared -o "libFunciones de ArchConfig.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(LIBRARIES) "libFunciones de ArchConfig.so"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
