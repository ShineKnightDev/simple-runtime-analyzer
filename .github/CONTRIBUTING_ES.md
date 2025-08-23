> 📄 Esta GUIA DE CONTRIBUCIÓN también está disponible en [English](/.github/CONTRIBUTING.md)

# 🛠️ Guía para contribuir

¡Gracias por tu interés en contribuir a **Simple Runtime Analyzer**! Este proyecto busca ser modular, educativo y reproducible. Aquí te explicamos cómo colaborar de forma efectiva.

---

## 📦 Tipos de contribuciones bienvenidas

- Nuevos algoritmos o funciones de benchmarking
- Mejoras en la documentación técnica (`.dox`, ejemplos, README)
- Correcciones de errores o mejoras de rendimiento
- Scripts de visualización o análisis adicionales
- Traducciones o mejoras en el onboarding

---

## 🧰 Requisitos técnicos

Antes de enviar tu contribución, asegúrate de:

- Usar C++17 o superior
- Seguir las convenciones de estilo del proyecto
  - [Clang Format](../.clang-format.yml)
  - [PEP 8](https://peps.python.org/pep-0008)
- Mantener la modularidad y claridad en las interfaces
- Documentar funciones públicas con comentarios compatibles con Doxygen
- Incluir ejemplos de uso si agregas nuevas funciones

---

## 📁 Organización del proyecto

- `include/`: headers públicos
- `docs/`: documentación técnica (`.dox`, ejemplos, notas)
- `example/`: casos de uso
- `data/`: archivos generados por pruebas

---

## 🧪 Cómo probar tu contribución

1. Asegúrate de que el proyecto compila con `make`.
2. Ejecuta `example.cpp` o crea un nuevo archivo en `examples/`.
3. Verifica que los archivos `.csv` y `.json` se generan correctamente.
4. Usa `plot.py` para visualizar los resultados.
5. Si agregas funciones nuevas, incluye un ejemplo y documentación en `docs/extras/`.

---

## 📝 Cómo enviar tu contribución

1. Haz un fork del repositorio.
2. Crea una rama descriptiva: `feature/sort-heap` o `fix/overflow-error`.
3. Realiza tus cambios y asegúrate de que compilan.
4. Agrega documentación y ejemplos si aplica.
5. Abre un Pull Request explicando:
   - Qué problema resuelve
   - Cómo se usa
   - Qué archivos modificaste

---

## 📣 Código de conducta

Este proyecto sigue el [Código de Conducta](CODE_OF_CONDUCT_ES.md). Por favor, sé respetuoso y colaborativo.

---

## 🧩 ¿Preguntas o sugerencias?

Puedes abrir un Issue para:

- Proponer nuevas funciones
- Reportar errores
- Pedir ayuda con el onboarding

¡Gracias por ayudar a mejorar este proyecto!
