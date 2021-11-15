# ORQUESTADOR DE MAKES

# COLORES PARA LA CONSOLA
RED :=\033[0;31m
GREEN :=\033[92m
YELLOW :=\033[0;33m
NOCOLOR :=\033[0m

# Me guardo esto por si a futuro nos sirve de algo como barra de carga
# ▓▓▓░░░░░░░░░░░░░░░░░

SHARED_NAME := SHARED
SHARED_ROUTE := shared/tests

# PLANIFICADOR INFO
PLANIFICADOR_NAME := PLANIFICADOR
PLANIFICADOR_ROUTE := planificador/src
PLANIFICADOR_TEST_ROUTE := planificador/tests
PLANIFICADOR_COMPILADO := planificador.out
PLANIFICADOR_ARCHIVO_CONFIG := planificador.config

# RAM INFO
RAM_NAME := RAM
RAM_ROUTE := ram/src
RAM_TEST_ROUTE := ram/tests
RAM_COMPILADO := ram.out
RAM_ARCHIVO_CONFIG := ram.config

# FILESYSTEM INFO
FILESYSTEM_NAME := FILESYSTEM
FILESYSTEM_ROUTE := filesystem/src
FILESYSTEM_TEST_ROUTE := filesystem/tests
FILESYSTEM_COMPILADO := filesystem.out
FILESYSTEM_ARCHIVO_CONFIG := filesystem.config

# MATELIB
MATELIB_NAME := MATELIB
MATELIB_ROUTE := matelib/src
MATELIB_TEST_ROUTE := matelib/tests
MATELIB_COMPILADO := matelib.out
MATELIB_ARCHIVO_CONFIG := matelib.config

all: p r f m
	
# Trabajos de compilacion proyectos y test "make d" "make r" "make f"
p:
	-@echo "$(YELLOW) COMPILANDO '$(PLANIFICADOR_NAME)' 🎮 🎮 🎮 $(NOCOLOR)"
	-@cd $(PLANIFICADOR_ROUTE) && $(MAKE) all
	-@echo "$(YELLOW) COMPILANDO TESTS '$(PLANIFICADOR_NAME)' 🎮 🎮 🎮 $(NOCOLOR)"
	-@cd $(PLANIFICADOR_TEST_ROUTE) && $(MAKE) all
	-@echo "$(GREEN) COMPILADO Tests '$(PLANIFICADOR_NAME)' ✔️✔️✔️✔️✔️✔️✔️✔️✔️✔️ $(NOCOLOR)"

r:
	-@echo "$(YELLOW) COMPILANDO '$(RAM_NAME)' 📝 📝 📝$(NOCOLOR)"
	-@cd $(RAM_ROUTE) && $(MAKE) all
	-@echo "$(YELLOW) Building Tests '$(RAM_NAME)' 📝 📝 📝 $(NOCOLOR)"
	-@cd $(RAM_TEST_ROUTE) && $(MAKE) all
	-@echo "$(GREEN) COMPILADO Tests '$(RAM_NAME)' ✔️✔️✔️✔️✔️✔️✔️✔️✔️✔️ $(NOCOLOR)"

f:
	-@echo "$(YELLOW) COMPILANDO '$(FILESYSTEM_NAME)' 📝 📝 📝$(NOCOLOR)"
	-@cd $(FILESYSTEM_ROUTE) && $(MAKE) all
	-@echo "$(YELLOW) COMPILANDO TESTS '$(FILESYSTEM_NAME)' 📝 📝 📝 $(NOCOLOR)"
	-@cd $(FILESYSTEM_TEST_ROUTE) && $(MAKE) all
	-@echo "$(GREEN) COMPILADO Tests '$(FILESYSTEM_NAME)' ✔️✔️✔️✔️✔️✔️✔️✔️✔️✔️ $(NOCOLOR)"
	
m:
	-@echo "$(YELLOW) COMPILANDO '$(MATELIB_NAME)' 📝 📝 📝$(NOCOLOR)"
	-@cd $(MATELIB_ROUTE) && $(MAKE) all
	-@echo "$(YELLOW) COMPILANDO TESTS '$(MATELIB_NAME)' 📝 📝 📝 $(NOCOLOR)"
	-@cd $(MATELIB_TEST_ROUTE) && $(MAKE) all
	-@echo "$(GREEN) COMPILADO Tests '$(MATELIB_NAME)' ✔️✔️✔️✔️✔️✔️✔️✔️✔️✔️ $(NOCOLOR)"
	
# Trabajos de clean "make clean-d" "make clean-r" "make clean-m" "make clean-s" "make clean"
clean-p:
	-@cd $(PLANIFICADOR_ROUTE) && $(MAKE) clean
	-@cd $(PLANIFICADOR_TEST_ROUTE) && $(MAKE) clean
	
clean-r:
	-@cd $(RAM_ROUTE) && $(MAKE) clean
	-@cd $(RAM_TEST_ROUTE) && $(MAKE) clean
	
clean-f:
	-@cd $(FILESYSTEM_ROUTE) && $(MAKE) clean
	-@cd $(FILESYSTEM_TEST_ROUTE) && $(MAKE) clean

clean-s:
	-@cd $(SHARED_ROUTE) && $(MAKE) clean
	
clean-m:
	-@cd $(MATELIB_ROUTE) && $(MAKE) clean
	-@cd $(MATELIB_TEST_ROUTE) && $(MAKE) clean

clean: clean-p clean-r clean-f clean-s clean-m
	-@find . -name "*.o" -type f -delete
	-@clear
	-@echo "$(GREEN) 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️   Limpiado archvos 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️ 🗑️  $(NOCOLOR)"

clear: clean
	-@echo "$(YELLOW) Sabía que no te acordarías que el comando es 'clean' no 'clear' así que lo codee igualmente $(NOCOLOR)"

# Trabajos para correr los tests "make test-d" "make test-r" "make test-m" "make test-s" "make test"
test-p:
	-@cd $(PLANIFICADOR_TEST_ROUTE) && $(MAKE) test

test-r:
	-@cd $(RAM_TEST_ROUTE) && $(MAKE) test
	
test-f:
	-@cd $(FILESYSTEM_TEST_ROUTE) && $(MAKE) test
	
test-m:
	-@cd $(MATELIB_TEST_ROUTE) && $(MAKE) test

test-s:
	-@cd $(SHARED_ROUTE) && $(MAKE) test
	
test: test-p test-r test-f test-s
	-@echo "$(YELLOW) Ejecutados todos los tests $(NOCOLOR)"

check: test
	
# Trabajos para correr los procesos con valgrind "make valgrind-s" "make valgrind-d" "make valgrind-r" "make valgrind-m"
valgrind-s:
	-@cd $(SHARED_ROUTE) && $(MAKE) valgrind

valgrind-p:
	-@cd $(PLANIFICADOR_ROUTE) && $(MAKE) valgrind
		
valgrind-r:
	-@cd $(RAM_ROUTE) && $(MAKE) valgrind

valgrind-f:
	-@cd $(FILESYSTEM_ROUTE) && $(MAKE) valgrind
	
valgrind-m:
	-@cd $(MATELIB_ROUTE) && $(MAKE) valgrind

# Trabajos para ejecutar los archivos compilados "make run-d" "make run-r" "make run-m"
run-p:
	-@./$(PLANIFICADOR_COMPILADO) $(PLANIFICADOR_ARCHIVO_CONFIG)
	
run-r:
	-@./$(RAM_COMPILADO) $(RAM_ARCHIVO_CONFIG)

run-f:
	-@./$(FILESYSTEM_COMPILADO) $(FILESYSTEM_ARCHIVO_CONFIG)

run-m:
	-@./$(MATELIB_COMPILADO) $(MATELIB_ARCHIVO_CONFIG)

.PHONY: all test clean planificador filesystem ram matelib shared
