# TESTS MATELIB

# COLORES PARA LA CONSOLA
RED :=\033[0;31m
GREEN :=\033[92m
YELLOW :=\033[0;33m
NOCOLOR :=\033[0m

RM=rm -rf
CC=gcc

C_SPEC=../../../cspec
C_SPEC_BIN=$(C_SPEC)/release

PROJECT_ROUTE := ..
PROJECT_NAME := matelib-test

LOG_FILE_VALGRIND := valgrind.log

# Devuelve todos los archivos con la extensión ".c"
C_SRCS=$(shell find ../ -type f \( -iname "*.c" ! -iname "main.c" \) | tr '\n' ' ') $(C_SHARED_SRCS)
# Obtenemos todos los archivos con extension ".c" de la carpeta shared
C_SHARED_SRCS = $(shell find ../../shared -type f \( -iname "*.c" ! -iname "main.c" \) | tr '\n' ' ')
# Al armnar todos los archivos hay un problema y toma el main.c hay que asegurarse que estén todos en una ruta específica para buildear
OBJS=$(C_SRCS:./%.c=build/%.o)

all: build/$(PROJECT_NAME)

# Crea la carpeta "build" el "-p" es para que si ya existia no de error
create-dirs:
	@mkdir -p build

# Antes llamaba a compilar la cspecs pero se va a asumir instalada para evitar errores
build/$(PROJECT_NAME): dependents create-dirs $(OBJS)
	$(CC) -L"$(PROJECT_ROUTE)" -L"$(C_SPEC_BIN)" -o "build/$(PROJECT_NAME)" $(OBJS) -lcommons -lcspecs -lpthread -lreadline -g -Wall
#gcc -L"../../../src" -L"../../../../../cspec/release" -o "build/boludeces-test" build/test_saraza.o -lcommons -lcspecs

build/%.o: ./%.c
	$(CC) -I"$(PROJECT_ROUTE)" -I"$(C_SPEC)" -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"

debug: CC += -DDEBUG -g
debug: all

clean:
	$(RM) build

clear: clean

# Solo queremos correr los tests
test: all
	LD_LIBRARY_PATH="$(PROJECT_ROUTE)/src/build/:$(C_SPEC_BIN)" build/$(PROJECT_NAME)

# Hace build del proyecto principal
dependents:
	-@cd $(PROJECT_ROUTE)/src && $(MAKE) all

valgrind: all
	-LD_LIBRARY_PATH="$(PROJECT_ROUTE)/src/build/:$(C_SPEC_BIN)" 
	-@valgrind --error-exitcode=42 --leak-check=full -v --log-file="$(LOG_FILE_VALGRIND)" ./build/$(PROJECT_NAME)
	-@echo "$(GREEN) 🎮 🎮 🎮 Resultados Valgrind $(PROJECT_NAME) 🎮 🎮 🎮 $(NOCOLOR)"
	-@tail $(LOG_FILE_VALGRIND) -n8
	-@rm $(LOG_FILE_VALGRIND)

.PHONY: all create-dirs clean install-cspec valgrind

