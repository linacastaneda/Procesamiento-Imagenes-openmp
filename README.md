
# Procesamiento Digital de Imágenes RGB con OpenMP

## 1. Descripción del proyecto

Proyecto académico de Computación Paralela y Distribuida.

El objetivo es implementar y comparar el rendimiento de algoritmos de procesamiento digital de imágenes mediante programación secuencial y paralela utilizando C y OpenMP.

La aplicación procesa fotografías JPG y PNG utilizando filtros de suavizado por promedio.

### Filtros disponibles

- 3x3: suavizado ligero.
- 7x7: suavizado moderado.
- 15x15: suavizado intenso.

El procesamiento se realiza sobre los tres canales RGB.

## 2. Estado del desarrollo

Actualmente funciona:

- Lectura de imágenes JPG y PNG.
- Procesamiento RGB.
- Filtro secuencial configurable.
- Menú interactivo.
- Generación automática de las tres versiones.
- Guardado de imágenes procesadas en PNG.
- Pruebas automáticas mediante Makefile.

Pendiente de integración:

- Algoritmo paralelo con OpenMP.
- Comparación secuencial y paralela.
- Mediciones de tiempo y memoria.
- Cálculo de speedup y eficiencia.
- Generación automática de gráficas SVG.

## 3. Tecnologías

- Lenguaje C11.
- GCC.
- Ubuntu mediante WSL 2.
- Visual Studio Code.
- Git y GitHub.
- Biblioteca STB para JPG y PNG.
- OpenMP para paralelismo.

## 4. Requisitos

Se necesita:

- Ubuntu o WSL con Ubuntu.
- GCC.
- Make.
- Git.
- Biblioteca libstb-dev.

Para instalar las dependencias en Ubuntu:

```bash
sudo apt update
sudo apt install build-essential git libstb-dev
```

No se requiere Python ni un entorno virtual.

## 5. Descargar el proyecto

Clonar el repositorio:

```bash
git clone https://github.com/linacastaneda/Procesamiento-Imagenes-openmp.git
```

Entrar al directorio:

```bash
cd Procesamiento-Imagenes-openmp
```

Consultar la rama en la que se está trabajando:

```bash
git branch --show-current
```

IMPORTANTE: mientras los módulos estén en desarrollo,
cada integrante debe utilizar su rama correspondiente.

## 6. Compilación

Compilar el programa:

```bash
make
```

Ejecutar pruebas automáticas:

```bash
make test
```

Limpiar ejecutables:

```bash
make clean
```

## 7. Ejecutar la aplicación

Iniciar el programa:

```bash
make run
```

Aparecerá el menú principal:

1. Seleccionar imagen.
2. Seleccionar filtro.
3. Ejecutar secuencial.
4. Generar los tres filtros.
5. Ejecutar paralelo (pendiente).
6. Comparar algoritmos (pendiente).
7. Experimentos y gráficas (pendiente).
8. Salir.

## 8. Selección de fotografías

Las fotografías deben guardarse en la carpeta:

`imagenes/`

Actualmente existe una imagen de ejemplo:

`imagenes/foto.jpg`

Cuando el programa solicite la ruta, escribir:

```text
imagenes/foto.jpg
```

También pueden utilizarse otras fotografías JPG o PNG.

Por ejemplo:

```text
imagenes/paisaje.jpg
imagenes/ciudad.png
imagenes/prueba.jpg
```

Las rutas son relativas a la carpeta principal del proyecto.

## 9. Procesamiento automático

Para generar los tres filtros:

1. Ejecutar `make run`.
2. Seleccionar la opción 1.
3. Introducir `imagenes/foto.jpg`.
4. Seleccionar la opción 4.
5. Esperar a que termine el procesamiento.

Se generarán archivos similares a:

```text
resultados/foto_secuencial_3x3_001.png
resultados/foto_secuencial_7x7_001.png
resultados/foto_secuencial_15x15_001.png
```

Los nombres incluyen:

- Nombre de la fotografía.
- Tipo de procesamiento.
- Tamaño del filtro.
- Número consecutivo.

Así se evita sobrescribir resultados durante el uso normal.

## 10. Pruebas

Ejecutar:

```bash
make test
```

Resultado esperado:

```text
Filtro 3x3: OK
Filtro 7x7: OK
Filtro 15x15: OK

Resultado general: TODAS LAS PRUEBAS OK
```

Estas pruebas verifican matemáticamente el filtro secuencial RGB.

## 11. Estructura del proyecto

```text
procesamiento-imagenes-openmp/
├── include/
│   ├── imagen.h
│   └── filtros.h
├── src/
│   ├── main.c
│   ├── imagen.c
│   └── filtro_secuencial.c
├── imagenes/
│   └── foto.jpg
├── resultados/
├── tests/
│   └── test_rgb.c
├── docs/
│   ├── DISENO_TECNICO.md
│   └── DEPENDENCIAS.md
├── Makefile
├── .gitignore
└── README.md
```

## 12. Organización del equipo

### Integrante 1 — Secuencial

Rama: `secuencial`

Responsabilidades:

- Lectura de imágenes.
- Procesamiento secuencial RGB.
- Menú base.
- Pruebas funcionales.

### Integrante 2 — Paralelo

Rama: `paralelo`

Responsabilidades:

- Implementación paralela con OpenMP.
- Configuración de hilos.
- Validación de equivalencia.
- Integración al menú.

### Integrante 3 — Experimentos

Rama: `experimentos`

Responsabilidades:

- Medición de tiempos.
- Análisis de memoria.
- Speedup y eficiencia.
- Registro CSV.
- Gráficas SVG.
- Resultados experimentales.

## 13. Integración

Cada integrante desarrolla su módulo en su rama.

Las modificaciones deben integrarse mediante Pull Requests a `main`.

Antes de modificar funciones compartidas, revisar:

`docs/DISENO_TECNICO.md`

Todos los integrantes deben conocer el funcionamiento completo del proyecto.

## 14. Resultados experimentales

Los tiempos y las métricas deben obtenerse mediante ejecuciones reales.

No deben utilizarse datos simulados como resultados definitivos.

Los experimentos deberán comparar las mismas imágenes y tamaños de filtro en las versiones secuencial y paralela.
```
