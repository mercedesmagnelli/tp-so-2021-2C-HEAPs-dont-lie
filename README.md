# CarpinchOS -  tp-2021-2c-HEAPs-dont-lie

| Apellido y Nombre | Usuario de Github | Modulo a cargo | 
|-------------------|------------------------------| -----------------------------| 
| Anchart, Alan  | [@Aanchart](https://github.com/Aanchart) | SWAmP |
| Armua, Alexander  | [@AlexanderArmua](https://github.com/AlexanderArmua) | Kernel |
| Flores, Juan  | [@jfloresmaquera](https://github.com/jfloresmaquera) | Memoria |
| Magnelli, Mercedes  | [@mercedesmagnelli](https://github.com/mercedesmagnelli) | Memoria | 
| Ruiz, Roman  | [@Eruizmagliano](https://github.com/Eruizmagliano) | MateLib | 

## Acerca del trabajo

El trabajo es una abstracción de los módulos de un sistema operativo real. En esta ocasión, los algoritmos elegidos para la planificación en el Kernel fueron SJF y HRRN. La distribución de la memoria principal fue paginación (con reemplazo local o global) con memoria virtual (con asignación fija o dinámica), de esta forma conectándose con el módulo de SWAmP. La MateLib fue la interfaz utilizada para comunicar las acciones al kernel o a la memoria, según correspondiera.


# Distribución de las carpetas

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
