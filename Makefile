
.RECIPEPREFIX := >

CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11 -Iinclude

OPENMP = -fopenmp

FUENTES = src/main.c src/imagen.c src/filtro_secuencial.c src/filtro_paralelo.c
PRUEBA = tests/test_rgb.c src/filtro_secuencial.c

.PHONY: all test run clean experimentos

all: programa

programa: $(FUENTES) include/imagen.h include/filtros.h
>$(CC) $(CPPFLAGS) $(CFLAGS) $(OPENMP) $(FUENTES) -o programa -lm

test_rgb: $(PRUEBA) include/imagen.h include/filtros.h
>$(CC) $(CFLAGS) $(PRUEBA) -o test_rgb

test_paralelo: tests/test_paralelo.c src/filtro_paralelo.c src/filtro_secuencial.c include/filtros.h include/imagen.h
>$(CC) $(CFLAGS) $(OPENMP) $(filter %.c,$^) -o $@

test: test_rgb test_paralelo
>./test_rgb
>./test_paralelo

run: programa
>mkdir -p resultados
>./programa

clean:
>rm -f programa test_rgb test_paralelo benchmark *.o

benchmark: tests/benchmark.c src/filtro_secuencial.c src/filtro_paralelo.c include/filtros.h include/imagen.h
>$(CC) $(CFLAGS) $(OPENMP) $(filter %.c,$^) -o $@

experimentos: benchmark
>python3 scripts/experimentar.py
