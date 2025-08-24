> 📄 This README is also available in [Español](.github/README_ES.md)

# Simple Runtime Analyzer

A lightweight, modular **C++ toolkit** for measuring runtime and generating samples. This project is ideal for educational purposes, helping you compare algorithm performance, analyze runtime behavior, and visualize execution metrics with a simple and extensible design.

-----

## ✨ Features

This toolkit provides a set of powerful utilities to streamline your performance analysis.

* **⏱️ Precision Timing:** Measure the execution time of any function, lambda, or callable object with high precision using `std::chrono`.
* **📈 Data Generation:** Easily generate input samples with configurable sizes and logarithmic bias for robust testing.
* **💾 Flexible Output:** Serialize your results and generated samples to `.csv`, `.txt`, and `.json` files, making it easy to share and analyze your data.
* **📊 Visualization:** A companion Python script uses `matplotlib` to automatically plot your results, providing a clear visual representation of performance trends.

-----

## 🚀 Getting Started

### Prerequisites

To use this toolkit, you'll need the following:

* **C++:** A C++23 compliant compiler (`g++`, `clang++`, etc.).
* **Python:** Python 3.7+ with the `matplotlib` library.

You can install `matplotlib` with `pip`:

```bash
pip install matplotlib
```

### Installation

The toolkit is designed to be easily integrated into your project. Simply download the contents of the `include` directory and add them to your project's include path. For a complete development environment, you can download the entire repository.

### Usage

This project includes a versatile `Makefile` to simplify your workflow.

#### Running a Custom File

To compile, run, and plot your own C++ source file, use the `run` command and specify your source file with the `SRC` variable.

```bash
make run SRC=my_program.cpp
```

This command will:

1. Compile `my_program.cpp` using the headers in the `include/` directory.
2. Execute the compiled binary.
3. Check for `.csv`, `.json`, or `.txt` output files in the `data/` directory.
4. Run the `scripts/plot.py` script on the found output file to generate a graph.

#### Running the Example

To see a complete, working example of the toolkit's functionality, use the `example` command.

```bash
make example
```

This will run the provided `example.cpp` file and generate its corresponding data and plot.

#### Cleaning Your Project

To remove all generated binaries, data files, and plots from your project, use the `clear` command.

```bash
make clear
```

To clean the files from the official example, use `example_clear`.

```bash
make example_clear
```

-----

## 📚 Documentation

For an in-depth look at all the functions, classes, and detailed usage, refer to the full Doxygen documentation.

**[View Full Documentation Here](docs/html/index.html)**

-----

## 🤝 Community & Contributing

If you like the project and it seems useful, consider give a star ⭐️ to the repository to show your support and [Follow me on Github](https://github.com/ShineKnightDev) To be aware of my new projects.
I accept all contributions. Whether you are reporting an error, suggesting a new feature or sending a change of code, your help is valuable.

> ⚖️ **[Code of Conduct](CODE_OF_CONDUCT.md)**: A set of rules to ensure the community remains a welcoming and inclusive place.

> 👷🏻‍♂️ **[Contributing Guide](.github/CONTRIBUTING.md)**: Details on how to get started, including how to report bugs, suggest features, and submit your first pull request.

> 💸 **[Sponsor](https://github.com/sponsors/ShineKnightDev)**: If you find this Toolkit useful, you can support me by sponsoring new projects.

> 📜 This project is licensed under the [MIT License © 2025 Diego Osorio (ShineKnightDev)](LICENSE.md).
