# tp-2021-2c-HEAPs-dont-lie

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
make d
make r
make m
make all
```

## Limpiar archivos clean
```shell
make clean-d
make clean-r
make clean-m
make clean-s
make clean
```

## Ejecutar los tests
```shell
make test-d
make test-r
make test-m
make test-s
make test
```

## Ejecutar Valgrind
```shell
make valgrind-d
make valgrind-r
make valgrind-m
make valgrind-s
```

## Ejecutar proyectos previamente compilados
```shell
make run-d
make run-r
make run-m
```
