> 📄 Este README también está disponible en [English](../README.md)

# Simple Runtime Analyzer

Un **toolkit de C++** ligero y modular para medir el tiempo de ejecución y generar muestras. Este proyecto es ideal para fines educativos, ayudándote a comparar el rendimiento de algoritmos, analizar el comportamiento en tiempo de ejecución y visualizar métricas con un diseño simple y extensible.

-----

## ✨ Características

Este toolkit proporciona un conjunto de potentes utilidades para optimizar tu análisis de rendimiento.

* **⏱️ Medición con Precisión:** Mide el tiempo de ejecución de cualquier función, lambda o objeto "callable" con alta precisión usando `std::chrono`.
* **📈 Generación de Datos:** Genera fácilmente muestras de entrada con tamaños configurables y sesgo logarítmico para pruebas robustas.
* **💾 Salida Flexible:** Serializa tus resultados y muestras generadas en archivos `.csv`, `.txt` y `.json`, lo que facilita el intercambio y análisis de tus datos.
* **📊 Visualización:** Un script complementario de Python utiliza `matplotlib` para graficar automáticamente tus resultados, proporcionando una representación visual clara de las tendencias de rendimiento.

-----

## 🚀 Empezando

### Requisitos

Para usar este toolkit, necesitarás lo siguiente:

* **C++:** Un compilador compatible con C++20 (`g++`, `clang++`, etc.).
* **Python:** Python 3.7+ con la biblioteca `matplotlib`.

Puedes instalar `matplotlib` con `pip`:

```bash
pip install matplotlib
```

### Instalación

El toolkit está diseñado para ser fácilmente integrado en tu proyecto. Simplemente descarga el contenido del directorio `include` y agrégalo a la ruta de inclusión de tu proyecto. Para un entorno de desarrollo completo, puedes descargar todo el repositorio.

### Uso

Este proyecto incluye un `Makefile` versátil para simplificar tu flujo de trabajo.

#### Ejecutar un archivo personalizado

Para compilar, ejecutar y graficar tu propio archivo fuente de C++, usa el comando `run` y especifica tu archivo fuente con la variable `SRC`.

```bash
make run SRC=mi_programa.cpp
```

Este comando:

1. Compilará `mi_programa.cpp` usando los encabezados en el directorio `include/`.
2. Ejecutará el binario compilado.
3. Buscará archivos de salida `.csv`, `.json` o `.txt` en el directorio `data/`.
4. Ejecutará el script `scripts/plot.py` en el archivo de salida encontrado para generar un gráfico.

#### Ejecutar el ejemplo

Para ver un ejemplo completo y funcional del toolkit, usa el comando `example`.

```bash
make example
```

Esto ejecutará el archivo `example.cpp` proporcionado y generará los datos y el gráfico correspondientes.

#### Limpiar tu proyecto

Para eliminar todos los binarios, archivos de datos y gráficos generados por tu proyecto, usa el comando `clear`.

```bash
make clear
```

Para limpiar los archivos del ejemplo oficial, usa `example_clear`.

```bash
make example_clear
```

-----

## 📚 Documentación

Para una mirada a fondo de todas las funciones, clases y uso detallado, consulta la documentación completa de Doxygen.

**[Ver la documentación completa aquí](../docs/html/index.html)**

-----

## 🤝 Comunidad y Contribuciones

Si te gusta el proyecto y te parece útil, considera dale una estrella ⭐️ al repositorio para mostrar tu apoyo y [sigueme en GitHub](https://github.com/ShineKnightDev) para estar al tanto de mis nuevos proyectos.
Acepto contribuciones de todos. Ya sea que estés reportando un error, sugiriendo una nueva característica o enviando un cambio de código, tu ayuda es valiosa.

> ⚖️ **[Código de Conducta](CODE_OF_CONDUCT_ES.md)**: Un conjunto de reglas para asegurar que la comunidad siga siendo un lugar acogedor e inclusivo.

> 👷🏻‍♂️ **[Guía para Contribuir](CONTRIBUTING.md)**: Detalles sobre cómo empezar, incluyendo cómo reportar errores, sugerir características y enviar tu primer "pull request".

> 💸 **[Patrocinar](https://github.com/sponsors/ShineKnightDev)**: Si encuentras útil este toolkit, puedes apoyarme patrocinando nuevos proyectos.

> 📜 Este proyecto está bajo la [Licencia MIT © 2025 Diego Osorio (ShineKnightDev)](LICENSE.md).
