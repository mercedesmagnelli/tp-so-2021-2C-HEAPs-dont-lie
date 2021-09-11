################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/conexion_planificador/conexion_planificador.c \
../src/conexion_planificador/coordinar_hilos.c \
../src/conexion_planificador/manejar_mensajes_planificador.c 

OBJS += \
./src/conexion_planificador/conexion_planificador.o \
./src/conexion_planificador/coordinar_hilos.o \
./src/conexion_planificador/manejar_mensajes_planificador.o 

C_DEPS += \
./src/conexion_planificador/conexion_planificador.d \
./src/conexion_planificador/coordinar_hilos.d \
./src/conexion_planificador/manejar_mensajes_planificador.d 


# Each subdirectory must supply rules for building sources it contributes
src/conexion_planificador/%.o: ../src/conexion_planificador/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


