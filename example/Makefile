# simple-runtime-analyzer example Makefile
# Usage:
#   make run                       # compile and run the example code and plot results
#   make clear                     # clean binaries and data files

CXX = clang++
CXXFLAGS = -std=c++23 -stdlib=libc++ -O2 -I../include
WARN = -Wall -Wextra -pedantic

BIN_DIR = bin
DATA_DIR = data
TARGET = example
SRC = src/example.cpp

CSV_OUTPUT = $(DATA_DIR)/times.csv
JSON_OUTPUT = $(DATA_DIR)/times.json
TXT_OUTPUT = $(DATA_DIR)/times.txt

.PHONY: all run example example_clear clear

all: run

run:
	@echo "🔧 Compiling $(SRC)..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(WARN) -o $(BIN_DIR)/$(TARGET) $(SRC)

	@echo "🚀 Running $(TARGET)..."
	@mkdir -p $(DATA_DIR)
	@./$(BIN_DIR)/$(TARGET)

	@if [ -f $(CSV_OUTPUT) ]; then \
		echo "🐍 Plotting results with plot.py using $(CSV_OUTPUT)..."; \
		python3 ../scripts/plot.py $(CSV_OUTPUT); \
	elif [ -f $(JSON_OUTPUT) ]; then \
		echo "🐍 Plotting results with plot.py using $(JSON_OUTPUT)..."; \
		python3 ../scripts/plot.py $(JSON_OUTPUT); \
	elif [ -f $(TXT_OUTPUT) ]; then \
		echo "🐍 Plotting results with plot.py using $(TXT_OUTPUT)..."; \
		python3 ../scripts/plot.py $(TXT_OUTPUT); \
	else \
		echo "❌ No output file (.csv, .json, .txt) found at $(DATA_DIR). Skipping plot."; \
	fi

clear:
	@echo "🧹 Cleaning example binaries and data files..."
	@rm -f $(BIN_DIR)/$(TARGET)
	@rm -f $(DATA_DIR)/*.csv
	@rm -f $(DATA_DIR)/*.json
	@rm -f $(DATA_DIR)/*.txt
	@rm -f $(DATA_DIR)/*.png
# Optional: Eliminate directories if they are empty.Be careful with this, it could be unwanted.
# @rmdir --ignore-fail-on-non-empty $(BIN_DIR) $(DATA_DIR)