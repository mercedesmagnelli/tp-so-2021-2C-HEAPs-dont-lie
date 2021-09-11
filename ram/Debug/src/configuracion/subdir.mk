################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/configuracion/ram_config_guardada.c \
../src/configuracion/ram_configuracion.c 

OBJS += \
./src/configuracion/ram_config_guardada.o \
./src/configuracion/ram_configuracion.o 

C_DEPS += \
./src/configuracion/ram_config_guardada.d \
./src/configuracion/ram_configuracion.d 


# Each subdirectory must supply rules for building sources it contributes
src/configuracion/%.o: ../src/configuracion/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


