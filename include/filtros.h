
#ifndef FILTROS_H
#define FILTROS_H

#include "imagen.h"

/*
 * Aplica un filtro de promedio de tamaño configurable.
 *
 * Tamaños admitidos: 3, 7 y 15.
 * La entrada y la salida deben ser imágenes distintas
 * con las mismas dimensiones y número de canales.
 *
 * Se conservan los píxeles exteriores donde la
 * ventana no cabe completamente.
 */
void suavizar_secuencial(
    const Imagen *entrada,
    Imagen *salida,
    int tamano_filtro
);

/*
 * Versión paralela que implementará el integrante 2.
 * Debe producir el mismo resultado que la secuencial.
 */
void suavizar_paralelo(
    const Imagen *entrada,
    Imagen *salida,
    int tamano_filtro,
    int num_hilos
);

#endif
