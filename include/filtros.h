
#ifndef FILTROS_H
#define FILTROS_H

#include "imagen.h"

void suavizar_secuencial(
    const Imagen *entrada,
    Imagen *salida
);

void suavizar_paralelo(
    const Imagen *entrada,
    Imagen *salida,
    int num_hilos
);

#endif
