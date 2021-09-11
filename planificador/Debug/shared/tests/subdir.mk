################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/utnso/workspace/tp-2021-1c-Sin-miedo-a-recursar/shared/tests/test_shared-test.c 

OBJS += \
./shared/tests/test_shared-test.o 

C_DEPS += \
./shared/tests/test_shared-test.d 


# Each subdirectory must supply rules for building sources it contributes
shared/tests/test_shared-test.o: /home/utnso/workspace/tp-2021-1c-Sin-miedo-a-recursar/shared/tests/test_shared-test.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


