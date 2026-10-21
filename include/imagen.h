
#ifndef IMAGEN_H
#define IMAGEN_H

typedef struct {
    int ancho;
    int alto;
    unsigned char *pixeles;
} Imagen;

int leer_pgm(const char *ruta, Imagen *imagen);
int guardar_pgm(const char *ruta, const Imagen *imagen);
void liberar_imagen(Imagen *imagen);

#endif
