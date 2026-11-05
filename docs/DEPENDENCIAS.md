
# Dependencias y configuración del entorno

## Sistema recomendado

Ubuntu mediante WSL 2.

También es posible utilizar una distribución Linux compatible.

## Herramientas necesarias

| Herramienta | Función |
|---|---|
| GCC | Compilación de código C |
| Make | Automatización de compilación |
| Git | Control de versiones |
| STB | Lectura JPG/PNG y escritura PNG |
| OpenMP | Programación paralela |

## Instalación en Ubuntu

```bash
sudo apt update
sudo apt install build-essential git libstb-dev
```

## Verificar GCC

```bash
gcc --version
```

## Verificar Make

```bash
make --version
```

## Verificar Git

```bash
git --version
```

## Verificar STB

```bash
ls /usr/include/stb/stb_image.h
ls /usr/include/stb/stb_image_write.h
```

## Compilación secuencial

Desde la carpeta principal:

```bash
make
```

## Ejecutar pruebas

```bash
make test
```

## Ejecutar programa

```bash
make run
```

## OpenMP

GCC incluye soporte de OpenMP mediante la opción:

```bash
-fopenmp
```

La integración definitiva se realizará cuando esté implementado `src/filtro_paralelo.c`.

## Notas

- Las imágenes de entrada se almacenan en `imagenes/`.
- Las imágenes procesadas se generan en `resultados/`.
- Los datos experimentales se incorporarán cuando esté listo el módulo de medición.
```
