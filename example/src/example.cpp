#include <algorithm>
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "shineknightdev/runtime_analyzer.hpp"
#include "shineknightdev/runtime_reporter.hpp"
#include "shineknightdev/sample_utils.hpp"

// -----------------------------------------------------------------------------
// Example Application for Simple Runtime Analyzer Toolkit
//
// This example demonstrates the complete workflow of the runtime analysis toolkit,
// showcasing its main features through a practical performance benchmarking scenario.
// -----------------------------------------------------------------------------

// Forward declarations for example functions
void sort_sample(std::vector<int> sample, bool use_stable_sort);
void fill_with_random_ints(std::vector<int>& vec, size_t size);
int execute_command(const std::string& command);
bool generate_plot(const std::string& data_file);

// -----------------------------------------------------------------------------
// Main Application Entry Point
// -----------------------------------------------------------------------------
int main()
{
    // -------------------------------------------------------------------------
    // Configuration Phase
    // -------------------------------------------------------------------------
    constexpr size_t sample_count = 10;
    constexpr size_t max_sample_size = 1'000'000;
    const sra::SampleSizeConfig size_config = {.round_to = 200, .bias = 1.5};
    const bool use_stable_sort = false; // Configuration parameter for the test function

    std::cout << "=== Simple Runtime Analyzer Example ===\n\n";

    // -------------------------------------------------------------------------
    // 1. Single Operation Timing Demonstration
    // -------------------------------------------------------------------------
    std::cout << "1. Single Operation Timing Measurement\n";
    std::cout << "--------------------------------------\n";

    std::cout << "⏱️  Executing baseline timing measurement...\n";
    auto single_op_duration = sra::measure_duration<std::chrono::milliseconds>([] 
    {
        // Simulate a computational operation with thread sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    });

    std::cout << "   Duration: " << single_op_duration.count() << " ms\n\n";

    // -------------------------------------------------------------------------
    // 2. Test Data Generation
    // -------------------------------------------------------------------------
    std::cout << "2. Test Data Generation\n";
    std::cout << "-----------------------\n";

    std::cout << "📊 Generating sample size distribution...\n";
    std::vector<size_t> sizes = sra::generate_sizes(sample_count, max_sample_size, size_config);

    std::cout << "🧪 Creating sample data with random integers...\n";
    auto samples = sra::generate_samples<int>(fill_with_random_ints, sizes);

    std::cout << "   Generated " << samples.size() << " samples with sizes ranging from " << sizes.front() << " to "
              << sizes.back() << " elements\n\n";

    // -------------------------------------------------------------------------
    // 3. Runtime Profiling Execution
    // -------------------------------------------------------------------------
    std::cout << "3. Runtime Performance Profiling\n";
    std::cout << "--------------------------------\n";

    std::cout << "🚀 Profiling sorting algorithm performance...\n";
    auto profile_in_us = sra::profile_runtime<std::chrono::microseconds>(sort_sample, samples, use_stable_sort);

    std::cout << "   Profiling completed with " << profile_in_us.size() << " data points\n\n";

    // -------------------------------------------------------------------------
    // 4. Data Processing and Conversion
    // -------------------------------------------------------------------------
    std::cout << "4. Data Processing\n";
    std::cout << "------------------\n";

    std::cout << "🔄 Converting time units for analysis...\n";
    auto profile_in_ms = profile_in_us.convert_to<std::chrono::milliseconds>();

    std::cout << "   Time unit conversion: microseconds → milliseconds\n\n";

    // -------------------------------------------------------------------------
    // 5. Results Reporting and Export
    // -------------------------------------------------------------------------
    std::cout << "5. Results Reporting\n";
    std::cout << "--------------------\n";

    std::cout << "📈 Generating performance reports...\n";

    // Display concise console report
    std::cout << "\n--- Performance Summary ---\n";
    sra::print_report(profile_in_ms);
    std::cout << "---------------------------\n\n";

    // Export data in multiple formats for further analysis
    sra::save_report(profile_in_ms, std::filesystem::path("data/report.csv"));
    sra::save_report(profile_in_ms, std::filesystem::path("data/report.json"));
    sra::save_report(profile_in_ms, std::filesystem::path("data/report.txt"));

    std::cout << "💾 Reports exported to data/report.{csv,json,txt}\n\n";

    // -------------------------------------------------------------------------
    // 6. Sample Data Preservation
    // -------------------------------------------------------------------------
    std::cout << "6. Data Preservation\n";
    std::cout << "--------------------\n";

    std::cout << "💾 Archiving generated sample data...\n";
    sra::save_samples(samples, sra::serialize_iterable<std::vector<int>>, "data/samples.csv");
    sra::save_samples(samples, sra::serialize_iterable<std::vector<int>>, "data/samples.json");

    std::cout << "   Sample data archived to data/samples.{csv,json}\n\n";

    // -------------------------------------------------------------------------
    // 7. Data Visualization
    // -------------------------------------------------------------------------
    std::cout << "7. Data Visualization\n";
    std::cout << "---------------------\n";

    std::cout << "📊 Generating performance visualization...\n";
    if (generate_plot("data/report.csv")) { std::cout << "   ✅ Visualization generated: data/report.png\n"; }
    else
    {
        std::cerr << "   ❌ Visualization generation failed\n";
    }

    // -------------------------------------------------------------------------
    // 8. Summary and Conclusion
    // -------------------------------------------------------------------------
    std::cout << "\n8. Execution Summary\n";
    std::cout << "--------------------\n";

    std::cout << "✅ Example execution completed successfully\n";
    std::cout << "✅ All toolkit features demonstrated\n";
    std::cout << "✅ Data exported for further analysis\n";
    std::cout << "✅ Performance profile generated and visualized\n\n";

    std::cout << "=== Example Completed ===\n";

    return 0;
}

// -----------------------------------------------------------------------------
// Function Implementations
// -----------------------------------------------------------------------------

/// @brief Benchmark function for sorting algorithm performance analysis
/// @param sample Vector of integers to be sorted (passed by value for isolation)
/// @param use_stable_sort Boolean flag to select sorting algorithm variant
///
/// This function is designed for performance profiling and takes the sample
/// by value to ensure each execution operates on a fresh, unmodified data set,
/// eliminating side effects between profiling iterations.
void sort_sample(std::vector<int> sample, bool use_stable_sort)
{
    if (use_stable_sort) { std::stable_sort(sample.begin(), sample.end()); }
    else
    {
        std::sort(sample.begin(), sample.end());
    }
}

/// @brief Data generation function for creating test samples
/// @param vec Reference to vector to be filled with random integers
/// @param size Target number of elements to generate
///
/// Utilizes a static random number generator instance for efficiency,
/// ensuring consistent random number generation across multiple calls
/// while maintaining thread safety through local static variables.
void fill_with_random_ints(std::vector<int>& vec, size_t size)
{
    // Static random number generator for consistent performance
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<int> dist(0, 10000);

    vec.clear();
    vec.reserve(size);

    for (size_t i = 0; i < size; ++i) { vec.push_back(dist(rng)); }
}

/// @brief Cross-platform command execution utility
/// @param command System command to execute
/// @return Integer exit code from the executed command
///
/// Provides platform-agnostic command execution with proper argument
/// quoting and shell invocation. On Windows, uses cmd.exe with secure
/// quoting; on Unix-like systems, executes directly through the shell.
int execute_command(const std::string& command)
{
#ifdef _WIN32
    return std::system(("cmd /c \"" + command + "\"").c_str());
#else
    return std::system(command.c_str());
#endif
}

/// @brief Data visualization gateway function
/// @param data_file Path to the CSV data file for visualization
/// @return Boolean indicating successful plot generation
///
/// Interfaces with the Python visualization script to generate
/// performance plots. Validates file existence and constructs
/// appropriate system commands with path quoting for reliability.
bool generate_plot(const std::string& data_file)
{
    std::string plot_script_path = PLOT_SCRIPT_PATH;

    // Validate script existence
    if (!std::filesystem::exists(plot_script_path))
    {
        std::cerr << "❌ Visualization script not found at: " << plot_script_path << "\n";
        return false;
    }

    // Validate data file existence
    if (!std::filesystem::exists(data_file))
    {
        std::cerr << "❌ Input data file not found: " << data_file << "\n";
        return false;
    }

    // Construct and execute visualization command
    std::stringstream command;
    command << "python3 \"" << plot_script_path << "\" \"" << data_file << "\"";

    int result = execute_command(command.str());
    return result == 0;
}