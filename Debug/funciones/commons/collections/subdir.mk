################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../funciones/commons/collections/dictionary.c \
../funciones/commons/collections/list.c \
../funciones/commons/collections/queue.c 

OBJS += \
./funciones/commons/collections/dictionary.o \
./funciones/commons/collections/list.o \
./funciones/commons/collections/queue.o 

C_DEPS += \
./funciones/commons/collections/dictionary.d \
./funciones/commons/collections/list.d \
./funciones/commons/collections/queue.d 


# Each subdirectory must supply rules for building sources it contributes
funciones/commons/collections/%.o: ../funciones/commons/collections/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


