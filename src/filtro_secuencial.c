
#include "filtros.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

int filtro_valido(int tamano) {
    return tamano == 3 || tamano == 7 || tamano == 15;
}

void suavizar_secuencial(
    const Imagen *entrada,
    Imagen *salida,
    int tamano_filtro
) {
    if (entrada == NULL || salida == NULL ||
        entrada->pixeles == NULL ||
        salida->pixeles == NULL ||
        !filtro_valido(tamano_filtro))
        return;

    int ancho = entrada->ancho;
    int alto = entrada->alto;
    int canales = entrada->canales;

    if (ancho <= 0 || alto <= 0 ||
        canales != 3 ||
        salida->ancho != ancho ||
        salida->alto != alto ||
        salida->canales != canales ||
        entrada->pixeles == salida->pixeles)
        return;

    if ((size_t)ancho > SIZE_MAX / (size_t)alto ||
        (size_t)ancho * (size_t)alto >
            SIZE_MAX / (size_t)canales)
        return;

    size_t total =
        (size_t)ancho * (size_t)alto * (size_t)canales;

    /* Copiar imagen original: conserva los bordes. */
    memcpy(salida->pixeles, entrada->pixeles, total);

    int radio = tamano_filtro / 2;
    int cantidad = tamano_filtro * tamano_filtro;

    /* Procesamiento secuencial de cada píxel. */
    for (int fila = radio; fila < alto - radio; fila++) {
        for (int columna = radio;
             columna < ancho - radio;
             columna++) {

            for (int canal = 0; canal < canales; canal++) {
                int suma = 0;

                for (int dy = -radio; dy <= radio; dy++) {
                    for (int dx = -radio; dx <= radio; dx++) {
                        size_t indice =
                            ((size_t)(fila + dy) *
                             (size_t)ancho +
                             (size_t)(columna + dx)) *
                             (size_t)canales +
                             (size_t)canal;

                        suma += entrada->pixeles[indice];
                    }
                }

                size_t centro =
                    ((size_t)fila * (size_t)ancho +
                     (size_t)columna) *
                     (size_t)canales +
                     (size_t)canal;

                salida->pixeles[centro] =
                    (unsigned char)(suma / cantidad);
            }
        }
    }
}
