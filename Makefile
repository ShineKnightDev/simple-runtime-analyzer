CXX = clang++
CXXFLAGS = -Wall -Wextra -O2 -std=c++20

SRC_DIR = src
BIN_DIR = bin

PROCESS_SEQUENCE_SRC = $(SRC_DIR)/process_sequence.cpp
MEASURE_TIMES_SRC = $(SRC_DIR)/measure_times.cpp
PROCESS_SEQUENCE_OBJ = $(BIN_DIR)/process_sequence.o
MEASURE_TIMES_OBJ = $(BIN_DIR)/measure_times.o
EXECUTABLE = $(BIN_DIR)/measure_times

RUNTIME_METRICS_TXT = runtime_metrics.txt
RUNTIME_METRICS_PLOT = runtime_metrics_plot.png

all: directories $(EXECUTABLE)

directories:
	mkdir -p $(BIN_DIR)

$(PROCESS_SEQUENCE_OBJ): $(PROCESS_SEQUENCE_SRC) $(SRC_DIR)/process_sequence.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(MEASURE_TIMES_OBJ): $(MEASURE_TIMES_SRC) $(SRC_DIR)/process_sequence.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXECUTABLE): $(MEASURE_TIMES_OBJ) $(PROCESS_SEQUENCE_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: all
	./$(EXECUTABLE)
	python3 scripts/plot.py

clean:
	rm -rf $(BIN_DIR) $(RUNTIME_METRICS_TXT) $(RUNTIME_METRICS_PLOT)

.PHONY: all clean run directories