#include "filtros.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void comprobar(int ancho, int alto) {
    size_t bytes = (size_t)ancho * alto * 3;
    unsigned char *original = malloc(bytes);
    Imagen entrada = {ancho, alto, malloc(bytes), 3};
    Imagen sec = {ancho, alto, malloc(bytes), 3};
    Imagen par = {ancho, alto, malloc(bytes), 3};
    if (!original || !entrada.pixeles || !sec.pixeles || !par.pixeles) {
        fprintf(stderr, "Memoria insuficiente para %dx%d\n", ancho, alto);
        exit(EXIT_FAILURE);
    }

    uint32_t estado = 42;
    for (size_t i = 0; i < bytes; ++i) {
        estado = estado * 1664525u + 1013904223u;
        original[i] = (unsigned char)(estado >> 24);
    }
    memcpy(entrada.pixeles, original, bytes);

    const int filtros[] = {3, 7, 15};
    for (size_t k = 0; k < sizeof(filtros) / sizeof(filtros[0]); ++k) {
        int lado = filtros[k];
        int radio = lado / 2;
        suavizar_secuencial(&entrada, &sec, lado);
        for (int p = 1; p <= 4; p *= 2) {
            /* Un byte sin escribir debe ser distinto del esperado. */
            for (size_t i = 0; i < bytes; ++i)
                par.pixeles[i] = (unsigned char)~sec.pixeles[i];
            suavizar_paralelo(&entrada, &par, lado, p);

            if (memcmp(original, entrada.pixeles, bytes) != 0) {
                fprintf(stderr, "Entrada modificada: %dx%d k=%d p=%d\n",
                        ancho, alto, lado, p);
                exit(EXIT_FAILURE);
            }
            for (int y = 0; y < alto; ++y) {
                for (int x = 0; x < ancho; ++x) {
                    size_t i = ((size_t)y * ancho + x) * 3;
                    int borde = y < radio || y >= alto - radio ||
                                x < radio || x >= ancho - radio;
                    for (int c = 0; c < 3; ++c) {
                        unsigned char valor = par.pixeles[i+c];
                        if (valor != sec.pixeles[i+c] ||
                            (borde && valor != original[i+c])) {
                            fprintf(stderr, "%dx%d k=%d p=%d (x=%d,y=%d,c=%d)"
                                    " sec=%u par=%u origen=%u\n", ancho, alto,
                                    lado, p, x, y, c, (unsigned)sec.pixeles[i+c],
                                    (unsigned)valor, (unsigned)original[i+c]);
                            exit(EXIT_FAILURE);
                        }
                    }
                }
            }
            printf("OK %dx%d k=%d p=%d\n", ancho, alto, lado, p);
            fflush(stdout);
        }
    }
    free(par.pixeles);
    free(sec.pixeles);
    free(entrada.pixeles);
    free(original);
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--grandes") == 0) {
        comprobar(1024, 1024);
        comprobar(2048, 2048);
        comprobar(4096, 4096);
    } else if (argc == 1) {
        comprobar(1, 1);
        comprobar(2, 19);
        comprobar(19, 2);
        comprobar(3, 3);
        comprobar(7, 7);
        comprobar(15, 15);
        comprobar(23, 17);
    } else {
        fprintf(stderr, "Uso: %s [--grandes]\n", argv[0]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
