################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../funciones/commons/bitarray.c \
../funciones/commons/config.c \
../funciones/commons/error.c \
../funciones/commons/log.c \
../funciones/commons/process.c \
../funciones/commons/string.c \
../funciones/commons/temporal.c \
../funciones/commons/txt.c 

OBJS += \
./funciones/commons/bitarray.o \
./funciones/commons/config.o \
./funciones/commons/error.o \
./funciones/commons/log.o \
./funciones/commons/process.o \
./funciones/commons/string.o \
./funciones/commons/temporal.o \
./funciones/commons/txt.o 

C_DEPS += \
./funciones/commons/bitarray.d \
./funciones/commons/config.d \
./funciones/commons/error.d \
./funciones/commons/log.d \
./funciones/commons/process.d \
./funciones/commons/string.d \
./funciones/commons/temporal.d \
./funciones/commons/txt.d 


# Each subdirectory must supply rules for building sources it contributes
funciones/commons/%.o: ../funciones/commons/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


