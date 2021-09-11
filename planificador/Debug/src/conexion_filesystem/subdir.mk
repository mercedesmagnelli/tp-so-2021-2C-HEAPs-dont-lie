################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/conexion_filesystem/conexion_filesystem.c \
../src/conexion_filesystem/manejar_mensajes_filesystem.c 

O_SRCS += \
../src/conexion_filesystem/conexion_filesystem.o \
../src/conexion_filesystem/manejar_mensajes_filesystem.o 

OBJS += \
./src/conexion_filesystem/conexion_filesystem.o \
./src/conexion_filesystem/manejar_mensajes_filesystem.o 

C_DEPS += \
./src/conexion_filesystem/conexion_filesystem.d \
./src/conexion_filesystem/manejar_mensajes_filesystem.d 


# Each subdirectory must supply rules for building sources it contributes
src/conexion_filesystem/%.o: ../src/conexion_filesystem/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


