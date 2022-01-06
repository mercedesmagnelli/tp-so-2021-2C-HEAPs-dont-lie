# PLANIFICADOR
# MAKEFILE PRINCIPAL

# COLORES PARA LA CONSOLA
RED :=\033[0;31m
GREEN :=\033[92m
YELLOW :=\033[0;33m
NOCOLOR :=\033[0m

PROJECT_NAME := PLANIFICADOR
TARGET := ../../planificador.out # Nombre del archivo al compilar 
LIBS := -lreadline -lcommons -lcspecs -lpthread # Dependencias necesarias
CC := gcc # Compilador a utilizar
CFLAGS := -g -Wall # Flags en la comilación (g: Debug) (Wall: Muestra todos los warnings)

CONFIG_ROUTE := ../../planificador.config

LOG_FILE_VALGRIND := valgrind.log

# Si hay archivos con nombres similares a alguna regla, que no asuma el argumento es el archivo.
.PHONY: default all clean

default: $(TARGET) # Regla por defecto
all: default # Si ejecutan all va a default

OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c)) # Remplaza en el 3er parametro lo que está en el 1ro con el 2do.
OBJECTS_ALL := $(patsubst %.c, %.o, $(wildcard **/*.c))
OBJECTS_SHARED := $(patsubst %.c, %.o, $(wildcard ../../shared/*.c))

HEADERS := $(wildcard *.h) # Retorna todos los archivos .h
HEADERS_ALL := $(wildcard **/*.h)
HEADERS_SHARED := $(wildcard ../../shared/*.h)

# Recibe los archivos, por ejemplo main.o y llama a "main.c main.h"
%.o: %.c $(HEADERS) $(HEADERS_ALL) $(HEADERS_SHARED)
	$(CC) $(CFLAGS) -c $< -o $@

# Si matan al make con ctrl-c nos permite mantener los archivos temporales terminados
.PRECIOUS: $(TARGET) $(OBJECTS)

# Le pasamos a la tarea los objects ya compilados
	# Usamos el compilador para linkear los objects a las bibliotecas y tener nuestro binario
$(TARGET): $(OBJECTS) $(OBJECTS_ALL) $(OBJECTS_SHARED) # Llama a compilar por cada OBJECTS
	$(CC) $(OBJECTS) $(OBJECTS_ALL) $(OBJECTS_SHARED) $(CFLAGS) $(LIBS) -o $@

valgrind: all
	-@valgrind --show-reachable=no --log-file="valgrind_d.log" --error-exitcode=42 --leak-check=full --show-leak-kinds=all -v $(TARGET) $(CONFIG_ROUTE)
	-@echo "$(GREEN) 🎮 🎮 🎮 Resultados Valgrind $(PROJECT_NAME) 🎮 🎮 🎮 $(NOCOLOR)"
	
clean: # Tarea para eliminar todos los .o y el binario
	-find . -name "*.o" -type f -delete
	-rm -f $(TARGET)
	