################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/configuracion/matelib_configuracion.c \
../src/configuracion/matelib_configuracion_guardada.c 

OBJS += \
./src/configuracion/matelib_configuracion.o \
./src/configuracion/matelib_configuracion_guardada.o 

C_DEPS += \
./src/configuracion/matelib_configuracion.d \
./src/configuracion/matelib_configuracion_guardada.d 


# Each subdirectory must supply rules for building sources it contributes
src/configuracion/%.o: ../src/configuracion/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


