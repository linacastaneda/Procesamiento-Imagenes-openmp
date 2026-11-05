# Diseño técnico y plan de integración

**Estado:** módulo secuencial RGB y menú base integrados en `main` mediante el Pull Request #4. Los módulos paralelo y de experimentos están pendientes de integrar.

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
**Importante:** `include/imagen.h` y `include/filtros.h` en `main` son las interfaces compartidas vigentes. La implementación secuencial admite imágenes RGB de 8 bits (3 canales). No modificar firmas sin coordinar los cambios con los demás integrantes.

- La aplicación implementada utiliza RGB de 8 bits (3 canales). La versión actual no incluye pruebas PGM; `tests/test_rgb.c` verifica matemáticamente los tres tamaños de filtro.
- `entrada` y `salida` tienen igual ancho/alto/canales; búferes separados y memoria suficiente. No modificar `entrada`.
- Filtros válidos: 3, 7, 15 (impares). Radio `r=tamano_filtro/2`. Píxeles donde la ventana no cabe: copiar original sin modificar.
- Para cada píxel interior y canal, sumar los `tamano_filtro*tamano_filtro` valores de la ventana de la **imagen de entrada** y guardar `(unsigned char)(suma/(tamano_filtro*tamano_filtro))` (división entera truncada). La versión paralela debe usar exactamente esa regla.
- OpenMP: repartir filas/píxeles de salida sin escrituras compartidas; cada hilo escribe posiciones distintas. Evitar modificar el búfer de origen; no usar sincronización innecesaria.
- La comparación debe comprobar igualdad píxel a píxel para la misma entrada y filtro. Si no son iguales, reportar error, no presentar resultados como correctos.
- Manejar rutas inexistentes, errores de memoria, tamaños de entrada inválidos y tamaños de filtro no admitidos. Ninguna función debe sobrescribir memoria fuera del búfer.

## Menú actual (módulo secuencial)
1. Seleccionar JPG/PNG.
2. Elegir 3x3, 7x7 o 15x15.
3. Ejecutar filtro secuencial y guardar PNG.
4. Generar automáticamente las tres versiones secuenciales.
5. Ejecutar paralelo (**pendiente**).
6. Comparar secuencial/paralelo (**pendiente**).
7. Experimentos y gráficas (**pendiente**).
8. Salir.

**Integración prevista:** la opción paralela debe permitir configurar hilos; la comparación deberá validar igualdad, medir tiempos, speedup y eficiencia. Los experimentos cubrirán al menos tres resoluciones, 1/2/4 hilos (cuando el hardware lo permita), repeticiones y gráficas SVG derivadas de los CSV reales.

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
