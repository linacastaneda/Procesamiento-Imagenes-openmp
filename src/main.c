
#include "imagen.h"
#include "filtros.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_RUTA 512
#define MAX_DESTINO 1024

/* Leer texto de forma segura desde la terminal. */
static int leer_linea(char *texto, size_t capacidad) {
    if (fgets(texto, (int)capacidad, stdin) == NULL)
        return 0;

    size_t longitud = strlen(texto);

    if (longitud > 0 && texto[longitud - 1] == '\n') {
        texto[longitud - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }

    return 1;
}

/* Leer un número entero del menú. */
static int leer_numero(void) {
    char texto[32];
    char extra;

    if (!leer_linea(texto, sizeof(texto)))
        return -1;

    int numero;

    if (sscanf(texto, " %d %c", &numero, &extra) != 1)
        return -1;

    return numero;
}

/* Obtener nombre base de la imagen, sin extensión. */
static void obtener_nombre(
    const char *ruta,
    char *nombre,
    size_t capacidad
) {
    const char *inicio = strrchr(ruta, '/');
    inicio = inicio ? inicio + 1 : ruta;

    size_t longitud = strcspn(inicio, ".");
    if (longitud >= capacidad)
        longitud = capacidad - 1;

    for (size_t i = 0; i < longitud; i++) {
        unsigned char c = (unsigned char)inicio[i];
        nombre[i] = isalnum(c) || c == '-' || c == '_'
                    ? (char)c : '_';
    }

    nombre[longitud] = '\0';

    if (longitud == 0)
        snprintf(nombre, capacidad, "imagen");
}

/* Verificar si ya existe un archivo. */
static int existe_archivo(const char *ruta) {
    FILE *archivo = fopen(ruta, "rb");

    if (archivo != NULL) {
        fclose(archivo);
        return 1;
    }

    return 0;
}

/* Crear nombre diferente para cada resultado. */
static int generar_destino(
    const char *ruta_original,
    int tamano,
    char *destino,
    size_t capacidad
) {
    char nombre[256];

    obtener_nombre(ruta_original, nombre, sizeof(nombre));

    for (int n = 1; n <= 9999; n++) {
        int longitud = snprintf(
            destino,
            capacidad,
            "resultados/%s_secuencial_%dx%d_%03d.png",
            nombre, tamano, tamano, n
        );

        if (longitud < 0 ||
            (size_t)longitud >= capacidad)
            return 0;

        if (!existe_archivo(destino))
            return 1;
    }

    return 0;
}

/* Procesar una imagen con el filtro seleccionado. */
static int procesar_imagen(
    const char *ruta,
    int tamano_filtro
) {
    Imagen entrada = {0};
    Imagen salida = {0};
    char destino[MAX_DESTINO];

    if (leer_imagen(ruta, &entrada) != 0) {
        printf("ERROR: no se pudo abrir la imagen.\n");
        return 0;
    }

    if ((size_t)entrada.ancho >
        SIZE_MAX / (size_t)entrada.alto ||
        (size_t)entrada.ancho * (size_t)entrada.alto >
        SIZE_MAX / (size_t)entrada.canales) {

        liberar_imagen(&entrada);
        return 0;
    }

    size_t total =
        (size_t)entrada.ancho *
        (size_t)entrada.alto *
        (size_t)entrada.canales;

    salida.ancho = entrada.ancho;
    salida.alto = entrada.alto;
    salida.canales = entrada.canales;
    salida.pixeles = malloc(total);

    if (salida.pixeles == NULL) {
        printf("ERROR: memoria insuficiente.\n");
        liberar_imagen(&entrada);
        return 0;
    }

    if (!generar_destino(
            ruta, tamano_filtro,
            destino, sizeof(destino))) {

        printf("ERROR: no se pudo crear el nombre.\n");
        liberar_imagen(&entrada);
        liberar_imagen(&salida);
        return 0;
    }

    printf("\nProcesando filtro %dx%d...\n",
           tamano_filtro, tamano_filtro);

    suavizar_secuencial(
        &entrada, &salida, tamano_filtro
    );

    int correcto = guardar_png(destino, &salida) == 0;

    if (correcto) {
        printf("Procesamiento completado.\n");
        printf("Resolucion: %d x %d\n",
               entrada.ancho, entrada.alto);
        printf("Canales: RGB\n");
        printf("Archivo: %s\n", destino);
    } else {
        printf("ERROR: no se pudo guardar el PNG.\n");
    }

    liberar_imagen(&entrada);
    liberar_imagen(&salida);

    return correcto;
}

int main(void) {
    char ruta[MAX_RUTA] = "";
    int tamano_filtro = 3;
    int opcion;

    do {
        printf("\n====================================\n");
        printf("  PROCESAMIENTO DIGITAL RGB - C\n");
        printf("====================================\n");

        printf("Imagen: %s\n",
               ruta[0] ? ruta : "No seleccionada");

        printf("Filtro: %dx%d\n",
               tamano_filtro, tamano_filtro);

        printf("\n1. Seleccionar imagen\n");
        printf("2. Seleccionar filtro\n");
        printf("3. Ejecutar secuencial\n");
        printf("4. Generar los tres filtros\n");
        printf("5. Ejecutar paralelo (pendiente)\n");
        printf("6. Comparar algoritmos (pendiente)\n");
        printf("7. Experimentos y graficas (pendiente)\n");
        printf("8. Salir\n");
        printf("\nOpcion: ");

        opcion = leer_numero();

        switch (opcion) {
            case 1:
                printf("Ruta JPG/PNG: ");

                if (!leer_linea(ruta, sizeof(ruta))) {
                    printf("Error al leer la ruta.\n");
                    opcion = 8;
                    break;
                }

                if (ruta[0] == '\0')
                    printf("La ruta no puede estar vacia.\n");

                break;

            case 2: {
                printf("1. Suavizado 3x3\n");
                printf("2. Suavizado 7x7\n");
                printf("3. Suavizado 15x15\n");
                printf("Seleccion: ");

                int seleccion = leer_numero();

                if (seleccion == 1)
                    tamano_filtro = 3;
                else if (seleccion == 2)
                    tamano_filtro = 7;
                else if (seleccion == 3)
                    tamano_filtro = 15;
                else
                    printf("Filtro no valido.\n");

                break;
            }

            case 3:
                if (ruta[0] == '\0') {
                    printf("Primero seleccione una imagen.\n");
                    break;
                }

                procesar_imagen(ruta, tamano_filtro);
                break;

            case 4: {
                if (ruta[0] == '\0') {
                    printf("Primero seleccione una imagen.\n");
                    break;
                }

                int filtros[] = {3, 7, 15};
                int exitos = 0;

                for (int i = 0; i < 3; i++) {
                    if (procesar_imagen(ruta, filtros[i]))
                        exitos++;
                }

                printf("\nFiltros completados: %d/3\n",
                       exitos);
                break;
            }

            case 5:
            case 6:
            case 7:
                printf("Modulo pendiente de integrar.\n");
                break;

            case 8:
                printf("Programa finalizado.\n");
                break;

            default:
                printf("Opcion no valida.\n");
        }

    } while (opcion != 8);

    return 0;
}
