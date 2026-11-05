# Diseño técnico y plan de integración

**Estado:** especificación propuesta del equipo. El código RGB configurable y el menú desarrollados localmente en WSL deben validarse y publicarse antes de integrar otras ramas.

## Objetivo
Aplicación en C compilada con GCC en Ubuntu/WSL que lea imágenes JPG/PNG, aplique un filtro RGB de media configurable (3x3, 7x7 o 15x15), ejecute una versión secuencial y otra paralela con OpenMP y compare rendimiento. Guardar resultados PNG, datos experimentales CSV y gráficas SVG.

## Reglas comunes e interfaces
```c
typedef struct {
    int ancho;
    int alto;
    unsigned char *pixeles;
    int canales; // 1=grises, 3=RGB
} Imagen;

void suavizar_secuencial(const Imagen *entrada, Imagen *salida, int tamano_filtro);
void suavizar_paralelo(const Imagen *entrada, Imagen *salida, int tamano_filtro, int num_hilos);
```
**Importante:** comprobar la definición final en `include/imagen.h` y `include/filtros.h` una vez que se integre el código RGB local de la integrante 1: el remoto puede conservar todavía la interfaz anterior.

- Para la aplicación interactiva se utilizará RGB de 8 bits (3 canales). Las pruebas PGM de 1 canal pueden conservarse como regresión.
- `entrada` y `salida` tienen igual ancho/alto/canales; búferes separados y memoria suficiente. No modificar `entrada`.
- Filtros válidos: 3, 7, 15 (impares). Radio `r=tamano_filtro/2`. Píxeles donde la ventana no cabe: copiar original sin modificar.
- Para cada píxel interior y canal, sumar los `tamano_filtro*tamano_filtro` valores de la ventana de la **imagen de entrada** y guardar `(unsigned char)(suma/(tamano_filtro*tamano_filtro))` (división entera truncada). La versión paralela debe usar exactamente esa regla.
- OpenMP: repartir filas/píxeles de salida sin escrituras compartidas; cada hilo escribe posiciones distintas. Evitar modificar el búfer de origen; no usar sincronización innecesaria.
- La comparación debe comprobar igualdad píxel a píxel para la misma entrada y filtro. Si no son iguales, reportar error, no presentar resultados como correctos.
- Manejar rutas inexistentes, errores de memoria, tamaños de entrada inválidos y tamaños de filtro no admitidos. Ninguna función debe sobrescribir memoria fuera del búfer.

## Menú planificado (no todo está implementado)
1. Seleccionar JPG/PNG.
2. Elegir 3x3, 7x7 o 15x15.
3. Ejecutar filtro secuencial y guardar PNG.
4. Ejecutar filtro paralelo con número de hilos configurable y guardar PNG.
5. Comparar secuencial/paralelo: misma imagen/filtro, validación de igualdad, tiempos, speedup y eficiencia.
6. Experimentos automáticos: >=3 resoluciones, 1/2/4 hilos si hardware lo permite, varios intentos.
7. Ver resumen y generar gráficas SVG a partir de los CSV.
8. Salir.

**Ideal:** además del menú, ofrecer CLI no interactiva para automatización, por ejemplo
`./programa --imagen imagenes/foto.jpg --filtro 15 --modo comparar --hilos 4`.
Es una especificación futura, **no** un comando implementado todavía.

## Experimentos y reporte
- Medir el **cálculo del filtro** por separado de carga JPG/PNG y guardado PNG; en Linux se puede utilizar `omp_get_wtime()` para ambos algoritmos.
- Registrar cada repetición: fecha, identificador de imagen, ancho, alto, canales, tamaño de filtro, modo, hilos, repetición, tiempo en segundos, validez de salida.
- Ejecutar ambas versiones con la misma imagen y filtro. Presentar medianas o promedio claramente indicado, conservando datos crudos.
- `speedup = T_secuencial / T_paralelo`; `eficiencia = speedup / hilos`. No tomar un hilo OpenMP como línea base secuencial si hay implementación secuencial separada.
- Calcular tamaño de búfer de imagen (ancho*alto*canales), distinguiéndolo de la **memoria real del proceso**. Si se informa RSS/pico RSS, documentar método, alcance y limitaciones; medir ejecuciones separadas para una comparación justa.
- Analizar overhead, balance de carga, escalabilidad; **no inventar datos**. Las gráficas SVG (tiempos, speedup, eficiencia, memoria si se mide confiablemente) deben derivarse de CSV reales.
- Datos de prueba con al menos tres tamaños de imagen y hilos 1, 2, 4 si el hardware lo permite; condiciones reproducibles y características del equipo documentadas.

## Responsabilidades
- **Integrante 1:** `src/imagen.c`, `src/filtro_secuencial.c`, lectura/escritura, pruebas RGB y estructura de menú en `src/main.c`.
- **Integrante 2:** `src/filtro_paralelo.c`, OpenMP, hilos y comprobación de igualdad con secuencial.
- **Integrante 3:** módulo de tiempos/memoria, repeticiones, CSV, métricas, gráficos SVG y diseño experimental.
- **Todos:** integración, README, informe, fuentes, revisión cruzada, demostración y sustentación.

## Flujo de Git
Trabajar en ramas `secuencial`, `paralelo`, `experimentos`; llevar cambios a `main` mediante Pull Requests. Antes de empezar, cada persona debe actualizar su rama con `main` y verificar interfaces actuales. Evitar editar simultáneamente `src/main.c` o archivos `.h` sin coordinar.
