#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include "shineknightdev/runtime_analyzer"
#include "shineknightdev/runtime_reporter"
#include "shineknightdev/sample_utils"
                 
constexpr int MIN_SAMPLE_VALUE = 0;
constexpr int MAX_SAMPLE_VALUE = 10000;

void fill_with_random_ints(std::vector<int>&, size_t);
void sort_sample(std::vector<int>, bool);

int main()
{
    // Generate example samples
    constexpr size_t sample_count = 10; // Number of samples to generate
    constexpr size_t max_sample_size = 1000000; // Larger sample size

    std::vector<size_t> sizes = generate_sizes(sample_count, max_sample_size, {.round_to = 200, .bias = 1.5});
    auto samples = generate_samples<int>(fill_with_random_ints, sizes);
    bool use_stable_sort = false; // Variable used in the example to decide whether to use sort or stable_sort

    // Measure execution time
    std::cout << "⏱️ Measuring function performance...\n";
    auto result = profile_runtime<std::chrono::microseconds>(sort_sample, samples, use_stable_sort);

    // Convert measurement results to a different unit
    auto converted_result = result.convert_to<std::chrono::milliseconds>();

    // Print report to console
    print_report(converted_result);

    // Save report to file
    std::string csv_filename = "data/report.csv";
    save_report(converted_result, csv_filename);
    std::cout << "💾 Report written to " << csv_filename << "\n";

    // Serialize samples to a file
    save_samples(samples, serialize_iterable<std::vector<int>>, "data/samples.csv");
    std::cout << "💾 Samples written to data/samples.csv\n";

    return 0;
}

// Example function to measure (sort a sample)
void sort_sample(std::vector<int> sample, bool use_stable_sort = false)
{
    if (use_stable_sort)
    {
        std::stable_sort(sample.begin(), sample.end());
        return;
    }
    std::sort(sample.begin(), sample.end());
}

// Example filler function (generate random integers)
void fill_with_random_ints(std::vector<int>& vec, size_t size)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(MIN_SAMPLE_VALUE, MAX_SAMPLE_VALUE);

    vec.clear();
    vec.reserve(size);

    for (size_t i = 0; i < size; ++i) vec.push_back(dist(rng));
}