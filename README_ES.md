> 📄 Este README también está disponible en [English](README.md)

# 📊 Simple Runtime Analyzer

Medición de Tiempos de Ejecución y Visualización con Python:

Este proyecto es un ejemplo básico diseñado para mostrar cómo medir tiempos de ejecución en C++, procesar datos, y graficar resultados con Python. Para el ejemplo se utiliza una funcion de generación de secuencias aleatorias, procesamiento de datos, y salida de métricas a archivo para luego visualizar la relación entre tamaño de entrada y tiempo de ejecución, Este enfoque resulta útil para comparar el rendimiento entre distintos algoritmos, analizar las diferencias entre tiempos reales y teóricos, y profundizar en análisis de eficiencia computacional.

---

## 📁 Estructura de Archivos

| Archivo / Script              | Descripción |
|------------------------------|-------------|
| `src/measure_times.cpp`      | Punto de entrada principal. Genera secuencias aleatorias, mide tiempos de ejecución de `processSequence` y guarda los resultados en un archivo `.txt`. |
| `src/process_sequence.hpp`   | Declaración de la función `processSequence`, encargada de procesar las secuencias. |
| `src/process_sequence.cpp`   | Implementación de `processSequence`: ordena, elimina duplicados y calcula sumas acumuladas. |
| `scripts/plot.py`            | Script en Python que lee las métricas desde un archivo y genera una gráfica en formato PNG. |
| `Makefile`                   | Automatiza la compilación de los archivos C++ y ejecuta tanto el binario como el script de Python. Incluye targets como `all`, `run`, y `clean`. |

---

## ⚙️ Funcionamiento del Programa

1. **Generación de Datos**  
   - Se crean vectores de enteros aleatorios con valores entre `0` y `10000`.
   - Los tamaños de las secuencias son variados: desde 1,000 hasta 7,000,000 elementos.

2. **Procesamiento de Secuencias**  
   - Las secuencias se ordenan (`std::sort`), se eliminan duplicados (`std::unique + erase`) y se les calcula la suma acumulada desde el segundo elemento en adelante.

3. **Medición de Tiempo**  
   - Se utiliza `std::chrono::high_resolution_clock` para capturar el tiempo antes y después del procesamiento.
   - El resultado se guarda en `runtime_metrics.txt`.

4. **Visualización**  
   - El script `plot.py` lee el archivo `.txt`, extrae pares de tamaño-tiempo y genera una gráfica con `matplotlib`.

---

## 🐍 Requisitos de Python

Asegúrate de tener instalado lo siguiente en tu sistema:

- **Python 3.x**
- **matplotlib**  
  Puedes instalarlo con:

  ```bash
  pip install matplotlib
  ```

---

## 🧱 Requisitos de Compilación

Para compilar y ejecutar el programa:

- **Compilador C++ moderno** compatible con C++20 (ej. `clang++` o `g++`)
- **Make** (para usar el Makefile)
- **Python 3** con matplotlib instalado

### ✅ Compilar y Ejecutar

Desde la raíz del proyecto, ejecuta:

```bash
make run
```

Esto compila el programa, corre las pruebas de tiempo y genera la gráfica.

### 🧹 Limpiar archivos generados

```bash
make clean
```

> 📜 Este proyecto está bajo la [Licencia MIT](LICENSE) © 2025 Diego Osorio (ShineKnightDev).
