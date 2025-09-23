> 📄 This CONTRIBUTING GUIDE is also available in [Español](/.github/CONTRIBUTING_ES.md)

# 🛠️ Contribution Guide

Thank you for your interest in contributing to **Simple Runtime Analyzer**! This project aims to be modular, educational, and reproducible. Here's how you can contribute effectively.

---

## 📦 Welcome Contributions

- New benchmarking algorithms or functions
- Technical documentation improvements
- Bug fixes or performance enhancements
- Additional visualization or analysis scripts
- Translations or onboarding improvements

---

## 🧰 Technical Requirements

Before submitting your contribution, please ensure:

- Use C++23
- Follow project style conventions
  - [Clang Format](../.clang-format.yml)
  - [PEP 8](https://peps.python.org/pep-0008)
- Maintain modularity and clear interfaces
- Document public functions with Doxygen-compatible comments
- Include usage examples for new functions

---

## 🧪 Testing Your Contribution

1. Verify the project builds with CMake
2. Run `example.cpp` or create a new file in `examples/`
3. Check that `.csv` and `.json` files generate correctly
4. Use `plot.py` to visualize results
5. For new functions, include an example and documentation in `docs/extras/`

---

## 📝 Submission Process

1. Fork the repository
2. Create a descriptive branch: `fix/overflow-error`
3. Make your changes and ensure they compile
4. Add documentation and examples if applicable
5. Open a Pull Request explaining:
   - The problem it solves
   - How to use it
   - Which files were modified

---

## 📣 Code of Conduct

This project follows the [Code of Conduct](../CODE_OF_CONDUCT.md). Please be respectful and collaborative.

---

## 🧩 Questions or Suggestions?

Open an Issue to:

- Propose new features
- Report bugs
- Request onboarding help

Thank you for helping improve this project!
