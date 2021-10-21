################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/inicializar_matelib/inicializar_matelib.c 

OBJS += \
./src/inicializar_matelib/inicializar_matelib.o 

C_DEPS += \
./src/inicializar_matelib/inicializar_matelib.d 


# Each subdirectory must supply rules for building sources it contributes
src/inicializar_matelib/%.o: ../src/inicializar_matelib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


