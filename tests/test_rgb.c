
#include "imagen.h"
#include "filtros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANCHO 21
#define ALTO 21
#define CANALES 3

int probar_filtro(int tamano) {
    unsigned char original[ANCHO * ALTO * CANALES];
    unsigned char resultado[ANCHO * ALTO * CANALES];

    /* Crear una imagen RGB uniforme */
    for (int i = 0; i < ANCHO * ALTO; i++) {
        original[i * 3] = 10;
        original[i * 3 + 1] = 20;
        original[i * 3 + 2] = 30;
    }

    /* Colocar un pixel diferente en el centro */
    int centro = ((ALTO / 2) * ANCHO + ANCHO / 2) * 3;

    original[centro] = 100;
    original[centro + 1] = 110;
    original[centro + 2] = 120;

    memset(resultado, 0, sizeof(resultado));

    Imagen entrada = {ANCHO, ALTO, original, CANALES};
    Imagen salida = {ANCHO, ALTO, resultado, CANALES};

    suavizar_secuencial(&entrada, &salida, tamano);

    int radio = tamano / 2;
    int correcto = 1;

    for (int fila = 0; fila < ALTO; fila++) {
        for (int col = 0; col < ANCHO; col++) {
            int indice = (fila * ANCHO + col) * 3;

            int dentro_ventana =
                abs(fila - ALTO / 2) <= radio &&
                abs(col - ANCHO / 2) <= radio;

            int procesable =
                fila >= radio && fila < ALTO - radio &&
                col >= radio && col < ANCHO - radio;

            for (int canal = 0; canal < 3; canal++) {
                int base = 10 + canal * 10;
                int esperado = original[indice + canal];

                if (procesable) {
                    esperado = base;

                    if (dentro_ventana) {
                        esperado =
                            base + 90 / (tamano * tamano);
                    }
                }

                if (resultado[indice + canal] != esperado)
                    correcto = 0;
            }
        }
    }

    printf("Filtro %dx%d: %s\n",
           tamano, tamano,
           correcto ? "OK" : "ERROR");

    return correcto;
}

int main(void) {
    int correcto = 1;

    correcto &= probar_filtro(3);
    correcto &= probar_filtro(7);
    correcto &= probar_filtro(15);

    printf("\nResultado general: %s\n",
           correcto ? "TODAS LAS PRUEBAS OK" : "ERROR");

    return correcto ? EXIT_SUCCESS : EXIT_FAILURE;
}
