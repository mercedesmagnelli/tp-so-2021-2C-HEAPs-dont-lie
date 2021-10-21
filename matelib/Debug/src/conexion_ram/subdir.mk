################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/conexion_ram/conexion_ram.c \
../src/conexion_ram/manejar_mensajes.c 

O_SRCS += \
../src/conexion_ram/conexion_ram.o \
../src/conexion_ram/manejar_mensajes.o 

OBJS += \
./src/conexion_ram/conexion_ram.o \
./src/conexion_ram/manejar_mensajes.o 

C_DEPS += \
./src/conexion_ram/conexion_ram.d \
./src/conexion_ram/manejar_mensajes.d 


# Each subdirectory must supply rules for building sources it contributes
src/conexion_ram/%.o: ../src/conexion_ram/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


