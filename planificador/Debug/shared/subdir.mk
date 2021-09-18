################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/conexion.c \
/home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/estructura_compartida.c \
/home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/logger.c \
/home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/manejo_hilos.c \
/home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/protocolo.c \
/home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/shared-test.c \
/home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/thread_safe_value.c 

OBJS += \
./shared/conexion.o \
./shared/estructura_compartida.o \
./shared/logger.o \
./shared/manejo_hilos.o \
./shared/protocolo.o \
./shared/shared-test.o \
./shared/thread_safe_value.o 

C_DEPS += \
./shared/conexion.d \
./shared/estructura_compartida.d \
./shared/logger.d \
./shared/manejo_hilos.d \
./shared/protocolo.d \
./shared/shared-test.d \
./shared/thread_safe_value.d 


# Each subdirectory must supply rules for building sources it contributes
shared/conexion.o: /home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/conexion.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

shared/estructura_compartida.o: /home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/estructura_compartida.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

shared/logger.o: /home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/logger.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

shared/manejo_hilos.o: /home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/manejo_hilos.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

shared/protocolo.o: /home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/protocolo.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

shared/shared-test.o: /home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/shared-test.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

shared/thread_safe_value.o: /home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared/thread_safe_value.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


