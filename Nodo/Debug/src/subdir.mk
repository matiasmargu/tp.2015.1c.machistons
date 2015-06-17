################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Nodo.c \
../src/atenderJob.c \
../src/atenderNodoYFS.c \
../src/funcionesParaEnviar.c \
../src/manejoDeMemoria.c \
../src/rutinasMapperYReducer.c \
../src/variablesGlobales.c 

OBJS += \
./src/Nodo.o \
./src/atenderJob.o \
./src/atenderNodoYFS.o \
./src/funcionesParaEnviar.o \
./src/manejoDeMemoria.o \
./src/rutinasMapperYReducer.o \
./src/variablesGlobales.o 

C_DEPS += \
./src/Nodo.d \
./src/atenderJob.d \
./src/atenderNodoYFS.d \
./src/funcionesParaEnviar.d \
./src/manejoDeMemoria.d \
./src/rutinasMapperYReducer.d \
./src/variablesGlobales.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/git/tp-2015-1c-machistons/funciones" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


