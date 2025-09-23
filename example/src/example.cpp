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
#include "shineknightdev/sample_utilities.hpp"

// --------------------------------------------------------------------------------------------------------------------
// Example Application for Simple Runtime Analyzer
//
// This example demonstrates the complete workflow of the runtime analysis library,
// showcasing its main features through a practical performance benchmarking scenario.
// --------------------------------------------------------------------------------------------------------------------

// Forward declarations for example functions
void sort_sample(std::vector<int> sample, bool use_stable_sort);
void bubble_sort(std::vector<int> sample);
void fill_with_random_ints(std::vector<int>& vec, size_t size);
int execute_command(const std::string& command);
bool generate_plot(const std::vector<std::string>& data_files);

// --------------------------------------------------------------------------------------------------------------------
// Main Application Entry Point
// --------------------------------------------------------------------------------------------------------------------
int main()
{
    // ----------------------------------------------------------------------------------------------------------------
    // Configuration Phase
    // ----------------------------------------------------------------------------------------------------------------
    constexpr size_t sample_count = 10;
    constexpr size_t max_sample_size = 100000;
    const sra::SampleSizeConfig size_config = {.round_to = 200, .bias = 1.5};

    std::cout << "=== Simple Runtime Analyzer Example ===\n\n";

    // ----------------------------------------------------------------------------------------------------------------
    // 1. Single Operation Timing Demonstration
    // ----------------------------------------------------------------------------------------------------------------
    std::cout << "1. Single Operation Timing Measurement\n";
    std::cout << "--------------------------------------\n";

    std::cout << "⏱️  Executing baseline timing measurement...\n";
    auto single_op_duration = sra::measure_duration<std::chrono::milliseconds>([] {
        // Simulate a computational operation with thread sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    });

    std::cout << "   Duration: " << single_op_duration.count() << " ms\n\n";

    // ----------------------------------------------------------------------------------------------------------------
    // 2. Test Data Generation
    // ----------------------------------------------------------------------------------------------------------------
    std::cout << "2. Test Data Generation\n";
    std::cout << "-----------------------\n";

    std::cout << "📊 Generating sample size distribution...\n";
    std::vector<size_t> sizes = sra::generate_sizes(sample_count, max_sample_size, size_config);

    std::cout << "🧪 Creating sample data with random integers...\n";
    auto samples = sra::generate_samples<int>(fill_with_random_ints, sizes);

    std::cout << "   Generated " << samples.size() << " samples with sizes ranging from " << sizes.front() << " to "
              << sizes.back() << " elements\n\n";

    // ----------------------------------------------------------------------------------------------------------------
    // 3. Runtime Profiling Execution
    // ----------------------------------------------------------------------------------------------------------------
    std::cout << "3. Runtime Performance Profiling\n";
    std::cout << "--------------------------------\n";

    std::cout << "🚀 Profiling sorting algorithm performance...\n";
    auto intro_sort_profile_in_us = sra::profile_runtime<std::chrono::microseconds>(sort_sample, samples, false);
    auto merge_sort_profile_in_us = sra::profile_runtime<std::chrono::microseconds>(sort_sample, samples, true);
    auto bubble_sort_profile_in_us = sra::profile_runtime<std::chrono::microseconds>(bubble_sort, samples);

    std::cout << "   Profiling completed with " << samples.size() << " data points\n\n";

    // ----------------------------------------------------------------------------------------------------------------
    // 4. Data Processing and Conversion
    // ----------------------------------------------------------------------------------------------------------------
    std::cout << "4. Data Processing\n";
    std::cout << "------------------\n";

    std::cout << "🔄 Converting time units for analysis...\n";
    auto intro_sort_profile_in_ns = intro_sort_profile_in_us.convert_to<std::chrono::nanoseconds>();
    auto merge_sort_profile_in_ns = merge_sort_profile_in_us.convert_to<std::chrono::nanoseconds>();
    auto bubble_sort_profile_in_ns = bubble_sort_profile_in_us.convert_to<std::chrono::nanoseconds>();

    std::cout << "   Time unit conversion: microseconds → nanoseconds\n\n";

    // ----------------------------------------------------------------------------------------------------------------
    // 5. Results Reporting and Export
    // ----------------------------------------------------------------------------------------------------------------
    std::cout << "5. Results Reporting\n";
    std::cout << "--------------------\n";

    std::cout << "📈 Generating performance reports...\n";

    // Display concise console report
    std::cout << "\n--- Performance Summary ---\n";
    std::cout << "IntroSort Algorithm:\n";
    sra::print_report(intro_sort_profile_in_ns);
    std::cout << "---------------------------\n";
    std::cout << "\nMergeSort Algorithm:\n";
    sra::print_report(merge_sort_profile_in_ns);
    std::cout << "---------------------------\n";
    std::cout << "\nBubbleSort Algorithm:\n";
    sra::print_report(bubble_sort_profile_in_ns);
    std::cout << "---------------------------\n\n";

    // Export data in multiple formats for further analysis
    sra::save_report(intro_sort_profile_in_ns, std::filesystem::path("data/intro_sort_report.csv"));
    sra::save_report(intro_sort_profile_in_ns, std::filesystem::path("data/intro_sort_report.json"));
    sra::save_report(intro_sort_profile_in_ns, std::filesystem::path("data/intro_sort_report.txt"));
    sra::save_reports(merge_sort_profile_in_ns, std::filesystem::path("data/merge_sort_report"));
    sra::save_reports(bubble_sort_profile_in_ns, std::filesystem::path("data/bubble_sort_report"));

    std::cout << "💾 Reports exported to data/\n\n";

    // ----------------------------------------------------------------------------------------------------------------
    // 6. Sample Data Preservation
    // ----------------------------------------------------------------------------------------------------------------
    std::cout << "6. Data Preservation\n";
    std::cout << "--------------------\n";

    std::cout << "💾 Archiving generated sample data...\n";
    sra::save_samples(samples, sra::serialize_iterable<std::vector<int>>, "data/samples.csv");
    sra::save_samples(samples, sra::serialize_iterable<std::vector<int>>, "data/samples.json");

    std::cout << "   Sample data archived to data/samples.{csv,json}\n\n";

    // ----------------------------------------------------------------------------------------------------------------
    // 7. Data Visualization
    // ----------------------------------------------------------------------------------------------------------------
    std::cout << "7. Data Visualization\n";
    std::cout << "---------------------\n";

    std::cout << "📊 Generating performance visualization...\n";
    if (generate_plot({"data/intro_sort_report.csv","data/merge_sort_report.csv" ,"data/bubble_sort_report.csv"}))
    {
        std::cout << "   ✅ Visualization generated: data/report.png\n";
    }
    else
    {
        std::cerr << "   ❌ Visualization generation failed\n";
    }

    // ----------------------------------------------------------------------------------------------------------------
    // 8. Summary and Conclusion
    // ----------------------------------------------------------------------------------------------------------------
    std::cout << "\n8. Execution Summary\n";
    std::cout << "--------------------\n";

    std::cout << "✅ Example execution completed successfully\n";
    std::cout << "✅ All library features demonstrated\n";
    std::cout << "✅ Data exported for further analysis\n";
    std::cout << "✅ Performance profile generated and visualized\n\n";

    std::cout << "=== Example Completed ===\n";

    return 0;
}

// --------------------------------------------------------------------------------------------------------------------
// Function Implementations
// --------------------------------------------------------------------------------------------------------------------

void sort_sample(std::vector<int> sample, bool use_stable_sort)
{
    if (use_stable_sort) { std::stable_sort(sample.begin(), sample.end()); }
    else
    {
        std::sort(sample.begin(), sample.end());
    }
}

void bubble_sort(std::vector<int> sample)
{
    size_t n = sample.size();
    bool swapped;
    for (size_t i = 0; i < n - 1; ++i)
    {
        swapped = false;

        for (size_t j = 0; j < n - i - 1; ++j)
        {
            if (sample[j] > sample[j + 1])
            {
                std::swap(sample[j], sample[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) { break; }
    }
}

void fill_with_random_ints(std::vector<int>& vec, size_t size)
{
    // Static random number generator for consistent performance
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<int> dist(0, 10000);

    vec.clear();
    vec.reserve(size);

    for (size_t i = 0; i < size; ++i) { vec.push_back(dist(rng)); }
}

int execute_command(const std::string& command)
{
#ifdef _WIN32
    return std::system(("cmd /c \"" + command + "\"").c_str());
#else
    return std::system(command.c_str());
#endif
}

bool generate_plot(const std::vector<std::string>& data_files)
{
    std::string plot_script_path = PLOT_SCRIPT_PATH;

    // Validate script existence
    if (!std::filesystem::exists(plot_script_path))
    {
        std::cerr << "❌ Visualization script not found at: " << plot_script_path << "\n";
        return false;
    }

    // Validate data file existences
    for (const auto& data_file : data_files)
    {
        if (!std::filesystem::exists(data_file))
        {
            std::cerr << "❌ Input data file not found: " << data_file << "\n";
            return false;
        }
    }

    // Construct and execute visualization command
    std::ostringstream command;

    command << "python3 \"" << plot_script_path << "\"";

    for (const auto& data_file : data_files) { command << " \"" << data_file << "\""; }

    int result = execute_command(command.str());
    return result == 0;
}