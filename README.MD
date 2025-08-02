> 📄 This README is also available in [Español](README_ES.md)

# 📊 Simple Runtime Analyzer

Execution Time Measurement and Visualization with Python:

This project is a basic example designed to demonstrate how to measure execution times in C++, process data, and visualize results using Python. It features a function that generates random sequences, processes the data, and outputs performance metrics to a file, which are then visualized to analyze the relationship between input size and execution time. This approach is useful for comparing the performance of different algorithms, exploring the gap between real and theoretical runtimes, and deepening understanding of computational efficiency analysis.

---

## 📁 File Structure

| File / Script                | Description |
|-----------------------------|-------------|
| `src/measure_times.cpp`     | Main entry point. Generates random sequences, measures `processSequence` execution time, and saves results to a `.txt` file. |
| `src/process_sequence.hpp`  | Declaration of the `processSequence` function, which handles sequence processing. |
| `src/process_sequence.cpp`  | Implementation of `processSequence`: sorts the sequence, removes duplicates, and calculates cumulative sums. |
| `scripts/plot.py`           | Python script that reads the metrics file and generates a PNG graph. |
| `Makefile`                  | Automates the compilation of C++ files and executes both the binary and Python script. Includes targets such as `all`, `run`, and `clean`. |

---

## ⚙️ Program Workflow

1. **Data Generation**  
   - Vectors of random integers are generated with values between `0` and `10000`.  
   - Sequence sizes vary from 1,000 to 7,000,000 elements.

2. **Sequence Processing**  
   - Sequences are sorted (`std::sort`), duplicates are removed (`std::unique` + `erase`), and cumulative sums are computed from the second element onward.

3. **Time Measurement**  
   - `std::chrono::high_resolution_clock` is used to capture timestamps before and after processing.  
   - Results are saved in `runtime_metrics.txt`.

4. **Visualization**  
   - The script `plot.py` reads the `.txt` file, extracts size-time pairs, and generates a graph using `matplotlib`.

---

## 🐍 Python Requirements

Ensure the following dependencies are installed:

- **Python 3.x**  
- **matplotlib**

You can install `matplotlib` with:

```bash
pip install matplotlib
```

---

## 🧱 Compilation Requirements

To compile and run the program:

- A **modern C++ compiler** supporting C++20 (e.g., `clang++` or `g++`)
- **Make** (to use the Makefile)
- **Python 3** with `matplotlib` installed

### ✅ Compile and Run

From the root directory of the project, run:

```bash
make run
```

This will compile the program, measure execution times, and generate the graph.

### 🧹 Clean Generated Files

```bash
make clean
```

> 📜 This project is licensed under the [MIT License](LICENSE) © 2025 Diego Osorio (ShineKnightDev).
