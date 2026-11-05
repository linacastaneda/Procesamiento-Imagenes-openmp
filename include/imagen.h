
#ifndef IMAGEN_H
#define IMAGEN_H

typedef struct {
    int ancho;
    int alto;
    unsigned char *pixeles;
    int canales;  /* 1 = grises, 3 = RGB */
} Imagen;

/* Funciones PGM existentes */
int leer_pgm(const char *ruta, Imagen *imagen);
int guardar_pgm(const char *ruta, const Imagen *imagen);

/* Funciones JPG y PNG */
int leer_imagen(const char *ruta, Imagen *imagen);
int guardar_png(const char *ruta, const Imagen *imagen);

/* Liberar memoria */
void liberar_imagen(Imagen *imagen);

#endif
