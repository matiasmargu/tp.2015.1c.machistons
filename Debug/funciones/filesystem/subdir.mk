################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../funciones/filesystem/filesystem.c 

OBJS += \
./funciones/filesystem/filesystem.o 

C_DEPS += \
./funciones/filesystem/filesystem.d 


# Each subdirectory must supply rules for building sources it contributes
funciones/filesystem/%.o: ../funciones/filesystem/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


