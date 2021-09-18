################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include tests/subdir.mk
-include src/consola/subdir.mk
-include src/configuracion/subdir.mk
-include src/conexion_ram/subdir.mk
-include src/conexion_filesystem/subdir.mk
-include src/subdir.mk
-include shared/tests/subdir.mk
-include shared/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: planificador

# Tool invocations
planificador: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/workspace/tp-2021-2c-HEAPs-dont-lie/shared" -o "planificador" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) planificador
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
