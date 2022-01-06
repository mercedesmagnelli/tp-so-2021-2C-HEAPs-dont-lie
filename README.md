# tp-2021-2c-HEAPs-dont-lie

La base del planificador, ram y filesystem son la misma. Hicimos el planificador y luego los otros dos.
Tomamos esta decisión para porque permitira un desarrollo donde la arquitectura de todos los modulos sea siempre la misma.

```
	p: Planificador
	f: Filesystem
	r: RAm
```

Distribución de carpetas:
- tp-2021
	- modulo_tp
		- src
			- main.c
			- sub-library
				- sub-library.c
				- sub-library.h
		- tests
			- test_sub-library.c
			- makefile
		- makefile
	- shared
		- tests
			- makefile
			- test_shared-helper.c
		- shared-helper.c
		- shared-helper.h
	- makefile

## Compilar Proyectos
```shell
make p
make r
make f
make all
```

## Limpiar archivos clean
```shell
make clean-p
make clean-r
make clean-f
make clean-s
make clean
```

## Ejecutar los tests
```shell
make test-p
make test-r
make test-f
make test-s
make test
```

## Ejecutar Valgrind
```shell
make valgrind-p
make valgrind-r
make valgrind-f
make valgrind-s
```

## Ejecutar proyectos previamente compilados
```shell
make run-p
make run-r
make run-f
```
