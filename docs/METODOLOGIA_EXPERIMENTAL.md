# Metodología experimental y análisis (integrante 3)

## Objetivo
Comparar el **mismo filtro RGB** en C, en modo secuencial y en modo OpenMP, con distintos tamaños y cantidades de hilos. Los datos publicados deben medirse en el equipo que se documenta, no inventarse.

## Preparación
En Ubuntu/WSL: `sudo apt install build-essential libstb-dev python3 python3-matplotlib` (si matplotlib no está disponible, los CSV sí se generan). `make test` comprueba los píxeles de ambos filtros; `make` compila el menú (requiere STB); `make benchmark` solo requiere GCC/OpenMP.

## Procedimiento reproducible
1. `make test && make benchmark`.
2. `python3 scripts/experimentar.py --resoluciones 256 512 1024 2048 --hilos 1 2 4 --filtros 3 7 15 --repeticiones 5`.
3. El ejecutable construye una **imagen RGB sintética determinista** para cada tamaño; ambos algoritmos reciben los mismos píxeles y un destino independiente.
4. Antes de las mediciones se verifica identidad **byte a byte**, incluido el borde. En cada repetición se alterna el orden secuencial/paralelo para reducir sesgos por calentamiento y caché. El cronómetro `omp_get_wtime` rodea solamente el filtro; **excluye** lectura de JPG, reserva de memoria y escritura PNG. El benchmark no mide memoria ni energía.
5. Cada combinación produce `repeticiones` tiempos para cada modo. Se usa la **mediana**, robusta ante valores atípicos.
6. Se guarda el entorno de CPU, SO, compilador y versión en `resultados_experimentos/entorno.txt`. Para comparar equipos, ejecutar en el mismo equipo, sin aplicaciones pesadas y con carga similar.

## Métricas
- Speedup: `S(p) = T_secuencial / T_paralelo(p)`.
- Eficiencia: `E(p) = S(p) / p` (×100 para porcentaje).
- Sobrecosto relativo al ideal: `O_ideal(p) = T_paralelo(p) - T_secuencial / p` en segundos. **No equivale al overhead aislado de OpenMP**: también captura efectos de caché, memoria, planificación y desequilibrio. Puede resultar negativo por ruido o diferencias de optimización.
- Escalabilidad fuerte: observar `S(p)` y `E(p)` al aumentar `p`, manteniendo fija la resolución.
- Escalabilidad con tamaño de problema: repetir las mismas cantidades de hilos con resoluciones mayores.
- Balance de carga: el bucle paralelo distribuye filas con `schedule(static)`. El reparto teórico difiere en como máximo una fila; eso **no prueba** que los tiempos reales de cada hilo sean iguales. Para cuantificar desequilibrio real habría que instrumentar cada hilo.

## Resultados
- `mediciones.csv`: filas individuales, reales, con resolución, filtro, modo, hilos, repetición, tiempo y verificación.
- `resumen.csv`: medianas, speedup, eficiencia y costo respecto al ideal.
- `speedup_k*_*.png`, `eficiencia_k*_*.png`, `tiempo_paralelo_s_k*_*.png`: gráficas.

## Discusión e informe IEEE
Indicar CPU, núcleos/hilos, SO, versión GCC y bandera `-O2 -fopenmp`; tabla de medianas con unidades; figuras con ejes, leyendas y pie; discutir si `S(p)` se aproxima a `p`, si `E(p)` cae y cómo cambian resultados por resolución/filtro. Diferenciar latencias medidas de conclusiones inferidas; reconocer ruido y no generalizar fuera del hardware probado. Añadir referencias bibliográficas comprobables y numeradas [1], [2] en estilo IEEE.

## Limitaciones
La imagen sintética no tiene archivos JPG/PNG, pero utiliza exactamente los mismos filtros RGB que el menú: mide el núcleo de procesamiento. La opción 6 del menú hace una comparación rápida sobre imagen real, **una ejecución**, y no sustituye el protocolo repetido de la opción 7. El orden, ruido del sistema, optimizaciones, CPU y frecuencia pueden influir.
