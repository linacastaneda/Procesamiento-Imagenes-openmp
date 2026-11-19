# Procesamiento Digital de Imágenes RGB con OpenMP

Proyecto académico de **Computación Paralela y Distribuida** de la Universidad Sergio Arboleda. Se implementaron filtros de suavizado RGB en **C**, con versiones secuencial y paralela mediante **OpenMP**, para comparar sus resultados y su rendimiento.

## Funcionalidades

- Lectura de imágenes JPG/PNG y guardado de resultados PNG.
- Filtros de promedio de vecindad **3×3, 7×7 y 15×15**, aplicados a los tres canales RGB.
- Ejecución secuencial y paralela con **1, 2 o 4 hilos**.
- Comparación rápida sobre una fotografía mediante el menú (opción 6).
- Pruebas automatizadas de equivalencia entre algoritmos.
- Experimentos reproducibles con diferentes resoluciones, filtros e hilos, con registros CSV y gráficas PNG (opción 7 o script).

El procesamiento mantiene sin cambios los píxeles de borde. El algoritmo paralelo distribuye filas con `#pragma omp parallel for schedule(static)`.

## Requisitos y entorno

El programa utiliza GCC con soporte de OpenMP y Make. La lectura y escritura de imágenes depende de las cabeceras de la biblioteca **STB**. Para los experimentos automáticos se requiere **Python 3 y Matplotlib**.

**Windows 11 con MSYS2 UCRT64** (entorno utilizado para las mediciones):

```bash
pacman -S --needed make mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-python mingw-w64-ucrt-x86_64-python-matplotlib mingw-w64-ucrt-x86_64-stb
```

La disponibilidad del paquete STB puede variar entre instalaciones; si no está disponible, se pueden proporcionar las cabeceras correspondientes en la ruta configurada para GCC. **El benchmark y las pruebas de filtros no necesitan STB; el menú de JPG/PNG sí.**

En **Ubuntu/WSL** se pueden instalar las dependencias con:

```bash
sudo apt update
sudo apt install build-essential libstb-dev python3 python3-matplotlib
```

## Descargar y compilar

```bash
git clone https://github.com/EnragedMagic/Procesamiento-Imagenes-OpenMP.git
cd Procesamiento-Imagenes-OpenMP

make test
make benchmark
```

Para compilar el menú de imágenes (requiere STB):

```bash
make
make run
```

En Windows con MSYS2 UCRT64, ejecutar estos comandos en esa terminal. Los comandos de Git también pueden ejecutarse desde PowerShell.

## Menú de la aplicación

1. Seleccionar imagen.
2. Seleccionar filtro.
3. Ejecutar procesamiento secuencial.
4. Generar los tres filtros secuenciales.
5. Ejecutar procesamiento paralelo con 1, 2 o 4 hilos.
6. Comparar tiempos secuencial/paralelo sobre la imagen seleccionada.
7. Ejecutar experimentos y generar archivos CSV/gráficas.
8. Salir.

Para seleccionar la fotografía incluida, indicar `imagenes/foto.jpg` desde la raíz del proyecto. Los PNG procesados se guardan en `resultados/`.

La opción 6 ofrece una comparación puntual, mientras que **la metodología experimental** utiliza múltiples repeticiones y tamaños de imagen para obtener resultados más estables.

## Experimentación y métricas

Ejecutar desde la raíz del proyecto:

```bash
make test
make benchmark
python3 scripts/experimentar.py --resoluciones 256 512 1024 2048 --hilos 1 2 4 --filtros 3 7 15 --repeticiones 5
```

Se evalúan **36 configuraciones**: 4 resoluciones, 3 filtros y 3 cantidades de hilos. Cada configuración se mide 5 veces en secuencial y 5 en paralelo, para un total de **360 registros**.

El benchmark genera datos RGB **sintéticos y deterministas**, idénticos para ambos modos, verifica igualdad byte a byte y mide únicamente el tiempo de aplicación del filtro mediante `omp_get_wtime()`. Se excluyen la lectura del JPG, la reserva de memoria y la escritura PNG. No se midieron directamente memoria máxima ni consumo energético.

Las medidas se resumen mediante la mediana:

- **Speedup:** `S(p) = T_secuencial / T_paralelo(p)`.
- **Eficiencia:** `E(p) = S(p) / p`.
- **Diferencia respecto al ideal:** `O_ideal(p) = T_paralelo(p) - T_secuencial / p`.

`O_ideal` **no es una medición aislada** del overhead interno de OpenMP; también puede reflejar otros efectos. El reparto estático de filas no basta para cuantificar el balance de carga real entre hilos.

Los resultados se crean localmente en `resultados_experimentos/`:

- `mediciones.csv`: mediciones individuales y verificaciones.
- `resumen.csv`: medianas y métricas.
- `entorno.txt`: información del sistema.
- `speedup_*.png`, `eficiencia_*.png`, `tiempo_paralelo_s_*.png`: gráficas.

**Nota:** la carpeta de resultados generados está excluida por `.gitignore`. Las evidencias seleccionadas y el informe final se incorporarán en una carpeta `informe/`, para conservarlas de forma controlada en el repositorio.

## Resultados obtenidos

En el equipo de pruebas (AMD Ryzen 7 5700X, 8 núcleos físicos, 16 procesadores lógicos, 32 GB RAM, Windows 11, MSYS2 UCRT64, GCC 16.2.0), para resolución **2048×2048** y **4 hilos**:

| Filtro | Secuencial (s) | Paralelo (s) | Speedup | Eficiencia |
| --- | ---: | ---: | ---: | ---: |
| 3×3 | 0,045 | 0,012 | 3,75× | 93,75 % |
| 7×7 | 0,166 | 0,047 | 3,53× | 88,30 % |
| 15×15 | 0,815 | 0,345 | 2,36× | 59,06 % |

Son resultados experimentales de **una ejecución en un equipo concreto**, no garantías de rendimiento en otras máquinas. Algunas mediciones de muy corta duración se registraron como cero; para esos casos no se debe interpretar un speedup infinito.

## Estructura

```text
include/                 Cabeceras de imagen y filtros
src/                     Programa principal y versiones secuencial/paralela
imagenes/                Fotografía de ejemplo
tests/                   Pruebas RGB, de equivalencia y benchmark
scripts/                 Automatización de experimentos
docs/                    Diseño técnico, dependencias y metodología
resultados_experimentos/  Generados localmente; excluidos de Git
Makefile                 Compilación y pruebas
README.md                Documentación principal
```

La metodología detallada está en [docs/METODOLOGIA_EXPERIMENTAL.md](docs/METODOLOGIA_EXPERIMENTAL.md).

## Equipo

- **Lina María Castañeda**
- **Johan Steven Galeano**
- **Juan Vasquez**

