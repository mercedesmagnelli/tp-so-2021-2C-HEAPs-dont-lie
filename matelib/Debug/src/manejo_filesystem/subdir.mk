################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/manejo_matelib/manejo_archivos.c 

O_SRCS += \
../src/manejo_matelib/manejo_archivos.o 

OBJS += \
./src/manejo_matelib/manejo_archivos.o 

C_DEPS += \
./src/manejo_matelib/manejo_archivos.d 


# Each subdirectory must supply rules for building sources it contributes
src/manejo_matelib/%.o: ../src/manejo_matelib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


