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
MATELIB_ROUTE := matelib
MATELIB_ARCHIVO_CONFIG := matelib.config
MATELIB_PRUEBAS_RUTA := carpinchos-pruebas


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
	-@cd $(MATELIB_ROUTE) && $(MAKE) uninstall
	-@echo "$(GREEN) DESINSTALADA MATELIB '$(MATELIB_NAME)' ✔️✔️✔️✔️✔️✔️✔️✔️✔️✔️ $(NOCOLOR)"
	-@cd $(MATELIB_ROUTE) && $(MAKE) install
	-@echo "$(GREEN) INSTALADA MATELIB '$(MATELIB_NAME)' ✔️✔️✔️✔️✔️✔️✔️✔️✔️✔️ $(NOCOLOR)"
	-@cd $(MATELIB_PRUEBAS_RUTA) && $(MAKE) compile
	-@echo "$(GREEN) COMPILADOS LOS TEST CASES '$(MATELIB_NAME)' ✔️✔️✔️✔️✔️✔️✔️✔️✔️✔️ $(NOCOLOR)"
	
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
	-@cd $(MATELIB_ROUTE) && $(MAKE) clean && $(MAKE) uninstall
	-@cd $(MATELIB_PRUEBAS_RUTA) && $(MAKE) clean

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
	
# Trabajos para ejecutar los archivos compilados "make run-d" "make run-r" "make run-m"
run-p:
	-@./$(PLANIFICADOR_COMPILADO) $(PLANIFICADOR_ARCHIVO_CONFIG)
	
run-r:
	-@./$(RAM_COMPILADO) $(RAM_ARCHIVO_CONFIG)

run-f:
	-@./$(FILESYSTEM_COMPILADO) $(FILESYSTEM_ARCHIVO_CONFIG)

MATELIB_PRUEBAS_RUTA_BUILD := carpinchos-pruebas/build
CONFIGURACIONES_RUTA := configs

help:
	-@echo "$(GREEN) Pruebas Kernel $(NOCOLOR)"
	-@echo "$(YELLOW) ----- Planificacion ----- $(NOCOLOR)"
	-@echo "make run-kernel-1-sjf"
	-@echo "make run-kernel-1-hrrn"
	-@echo "$(YELLOW) ----- Deadlock ----- $(NOCOLOR)"
	-@echo "make run-kernel-2-sjf"
	-@echo "make run-kernel-2-hrrn"
	-@echo "$(YELLOW) ----- Suspensión ----- $(NOCOLOR)"
	-@echo "make run-kernel-3-sjf"
	-@echo "make run-kernel-3-hrrn"
	-@echo "$(GREEN) -------------- $(NOCOLOR)"
	-@echo "$(GREEN) Pruebas Memoria $(NOCOLOR)"
	-@echo "$(YELLOW) ----- Reemplazo MMU ----- $(NOCOLOR)"
	-@echo "make run-memoria-1-clockm"
	-@echo "make run-memoria-1-lru"
	-@echo "$(YELLOW) ----- Asignación ----- $(NOCOLOR)"
	-@echo "make run-memoria-2-fija"
	-@echo "make run-memoria-2-dinamica"
	-@echo "$(YELLOW) ----- TLB ----- $(NOCOLOR)"
	-@echo "make run-memoria-3-fifo"
	-@echo "make run-memoria-3-lru"
	-@echo "$(GREEN) -------------- $(NOCOLOR)"
	-@echo "$(GREEN) Pruebas SWAP $(NOCOLOR)"
	-@echo "$(YELLOW) ----- Default ----- $(NOCOLOR)"
	-@echo "make run-swap-1"
	-@echo "$(GREEN) -------------- $(NOCOLOR)"
	-@echo "$(GREEN) Pruebas GENERALES $(NOCOLOR)"
	-@echo "$(YELLOW) ----- Saludo al humedal ----- $(NOCOLOR)"
	-@echo "make run-general-1"
	-@echo "$(YELLOW) ----- Batalla por Nordelta ----- $(NOCOLOR)"
	-@echo "make run-general-2"
	-@echo "$(GREEN) -------------- $(NOCOLOR)"

remover-config:
	-rm -f $(PLANIFICADOR_ARCHIVO_CONFIG)
	-rm -f $(RAM_ARCHIVO_CONFIG)
	-rm -f $(FILESYSTEM_ARCHIVO_CONFIG)
	-rm -f $(MATELIB_ARCHIVO_CONFIG)

run-kernel-1-sjf:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/planificador_1_sjf/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_1_sjf/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_1_sjf/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaPlanificacion $(MATELIB_ARCHIVO_CONFIG)

run-kernel-1-hrrn:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/planificador_1_hrrn/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_1_hrrn/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_1_hrrn/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaPlanificacion $(MATELIB_ARCHIVO_CONFIG)

run-kernel-2-sjf:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/planificador_2_sjf/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_2_sjf/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_2_sjf/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaDeadlock $(MATELIB_ARCHIVO_CONFIG)

run-kernel-2-hrrn:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/planificador_2_hrrn/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_2_hrrn/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_2_hrrn/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaDeadlock $(MATELIB_ARCHIVO_CONFIG)

run-kernel-3-sjf:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/planificador_3_sjf/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_3_sjf/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_3_sjf/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaSuspension $(MATELIB_ARCHIVO_CONFIG)
	
run-kernel-3-hrrn:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/planificador_3_hrrn/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_3_hrrn/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/planificador_3_hrrn/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaSuspension $(MATELIB_ARCHIVO_CONFIG)	
	
run-memoria-1-clockm:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/memoria_1_clock_m/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_1_clock_m/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_1_clock_m/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaMMU $(MATELIB_ARCHIVO_CONFIG)

run-memoria-1-lru:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/memoria_1_lru/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_1_lru/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_1_lru/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaMMU $(MATELIB_ARCHIVO_CONFIG)

run-memoria-2-fija:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/memoria_2_fija/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_2_fija/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_2_fija/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaAsignacion $(MATELIB_ARCHIVO_CONFIG)

run-memoria-2-dinamica:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/memoria_2_dinamica/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_2_dinamica/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_2_dinamica/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaAsignacion $(MATELIB_ARCHIVO_CONFIG)

run-memoria-3-fifo:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/memoria_3_fifo/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_3_fifo/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_3_fifo/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/prueba_tlb_fifo $(MATELIB_ARCHIVO_CONFIG)
	
run-memoria-3-lru:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/memoria_3_lru/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_3_lru/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/memoria_3_lru/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/prueba_tlb_lru $(MATELIB_ARCHIVO_CONFIG)

run-swap-1:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/swap_1/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/swap_1/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/swap_1/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/swap_1/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/prueba_swamp $(MATELIB_ARCHIVO_CONFIG)
	
run-general-1:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/general_1/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/general_1/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/general_1/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@echo "$(GREEN) Se copiaron todas las configs, ahora queda ejecutar los procesos $(NOCOLOR)"
	-@echo "Ejecutar (sin make) ./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaBAse_Carpincho1 $(MATELIB_ARCHIVO_CONFIG)"
	-@echo "Ejecutar (sin make) ./$(MATELIB_PRUEBAS_RUTA_BUILD)/PruebaBAse_Carpincho2 $(MATELIB_ARCHIVO_CONFIG)"

run-general-2:
	-@$(MAKE) remover-config
	-cp $(CONFIGURACIONES_RUTA)/general_2/planificador.config $(PLANIFICADOR_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/general_2/filesystem.config $(FILESYSTEM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/general_2/ram.config $(RAM_ARCHIVO_CONFIG)
	-cp $(CONFIGURACIONES_RUTA)/matelib.config $(MATELIB_ARCHIVO_CONFIG)
	-@echo "$(YELLOW) ------- BATALLA POR NORDELTA ------- $(NOCOLOR)"
	-@./$(MATELIB_PRUEBAS_RUTA_BUILD)/BatallaPorNordelta $(MATELIB_ARCHIVO_CONFIG)

.PHONY: all test clean planificador filesystem ram matelib shared






