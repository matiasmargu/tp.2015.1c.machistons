################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Marta/src/Marta.c 

OBJS += \
./Marta/src/Marta.o 

C_DEPS += \
./Marta/src/Marta.d 


# Each subdirectory must supply rules for building sources it contributes
Marta/src/%.o: ../Marta/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


