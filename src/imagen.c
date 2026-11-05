
#include "imagen.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

/* Liberar toda la memoria de una imagen. */
void liberar_imagen(Imagen *imagen) {
    if (imagen == NULL)
        return;

    free(imagen->pixeles);

    imagen->pixeles = NULL;
    imagen->ancho = 0;
    imagen->alto = 0;
    imagen->canales = 0;
}

/* Leer JPG o PNG y almacenar tres canales RGB. */
int leer_imagen(const char *ruta, Imagen *imagen) {
    if (ruta == NULL || imagen == NULL)
        return -1;

    int ancho = 0;
    int alto = 0;
    int canales_originales = 0;

    unsigned char *temporal = stbi_load(
        ruta,
        &ancho,
        &alto,
        &canales_originales,
        3
    );

    if (temporal == NULL)
        return -1;

    if (ancho <= 0 || alto <= 0 ||
        (size_t)ancho > SIZE_MAX / (size_t)alto ||
        (size_t)ancho * (size_t)alto > SIZE_MAX / 3) {
        stbi_image_free(temporal);
        return -1;
    }

    size_t total = (size_t)ancho * (size_t)alto * 3;

    unsigned char *pixeles = malloc(total);

    if (pixeles == NULL) {
        stbi_image_free(temporal);
        return -1;
    }

    memcpy(pixeles, temporal, total);
    stbi_image_free(temporal);

    imagen->ancho = ancho;
    imagen->alto = alto;
    imagen->canales = 3;
    imagen->pixeles = pixeles;

    return 0;
}

/* Guardar la imagen como PNG. */
int guardar_png(const char *ruta, const Imagen *imagen) {
    if (ruta == NULL || imagen == NULL ||
        imagen->pixeles == NULL ||
        imagen->ancho <= 0 ||
        imagen->alto <= 0 ||
        imagen->canales != 3)
        return -1;

    if (imagen->ancho > INT_MAX / imagen->canales)
        return -1;

    int resultado = stbi_write_png(
        ruta,
        imagen->ancho,
        imagen->alto,
        imagen->canales,
        imagen->pixeles,
        imagen->ancho * imagen->canales
    );

    return resultado ? 0 : -1;
}
