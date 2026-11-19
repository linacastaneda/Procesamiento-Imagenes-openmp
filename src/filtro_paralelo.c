#include "filtros.h"

#include <stdint.h>
#include <string.h>

void suavizar_paralelo(const Imagen *entrada, Imagen *salida,
                       int tamano_filtro, int num_hilos) {
    if (entrada == NULL || salida == NULL || entrada->pixeles == NULL ||
        salida->pixeles == NULL || num_hilos < 1 ||
        (tamano_filtro != 3 && tamano_filtro != 7 && tamano_filtro != 15))
        return;

    const int ancho = entrada->ancho;
    const int alto = entrada->alto;
    const int canales = entrada->canales;

    if (ancho <= 0 || alto <= 0 || canales != 3 ||
        ancho != salida->ancho || alto != salida->alto ||
        canales != salida->canales || entrada->pixeles == salida->pixeles)
        return;

    if ((size_t)ancho > SIZE_MAX / (size_t)alto ||
        (size_t)ancho * (size_t)alto > SIZE_MAX / (size_t)canales)
        return;

    const int radio = tamano_filtro / 2;
    const int cantidad = tamano_filtro * tamano_filtro;
    const unsigned char *origen = entrada->pixeles;
    unsigned char *destino = salida->pixeles;

    #pragma omp parallel for default(none) schedule(static) num_threads(num_hilos) \
        shared(origen, destino, ancho, alto, canales, radio, cantidad)
    for (int fila = 0; fila < alto; ++fila) {
        for (int col = 0; col < ancho; ++col) {
            size_t centro = ((size_t)fila * ancho + col) * canales;
            if (fila < radio || fila >= alto - radio ||
                col < radio || col >= ancho - radio) {
                memcpy(destino + centro, origen + centro, (size_t)canales);
                continue;
            }

            for (int canal = 0; canal < canales; ++canal) {
                int suma = 0;
                for (int dy = -radio; dy <= radio; ++dy) {
                    for (int dx = -radio; dx <= radio; ++dx) {
                        size_t vecino = ((size_t)(fila + dy) * ancho +
                                         (size_t)(col + dx)) * canales + canal;
                        suma += origen[vecino];
                    }
                }
                destino[centro + canal] = (unsigned char)(suma / cantidad);
            }
        }
    }
}
