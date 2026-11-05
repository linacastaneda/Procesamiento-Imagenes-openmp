
.RECIPEPREFIX := >

CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11 -Iinclude

FUENTES = src/main.c src/imagen.c src/filtro_secuencial.c
PRUEBA = tests/test_rgb.c src/filtro_secuencial.c

.PHONY: all test run clean

all: programa

programa: $(FUENTES) include/imagen.h include/filtros.h
>$(CC) $(CFLAGS) $(FUENTES) -o programa -lm

test_rgb: $(PRUEBA) include/imagen.h include/filtros.h
>$(CC) $(CFLAGS) $(PRUEBA) -o test_rgb

test: test_rgb
>./test_rgb

run: programa
>mkdir -p resultados
>./programa

clean:
>rm -f programa test_rgb *.o
