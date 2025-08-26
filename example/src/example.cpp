#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "../../include/shineknightdev/measure_time.hpp"
#include "../../include/shineknightdev/sample_utils.hpp"

constexpr int MIN_SAMPLE_VALUE = 0;
constexpr int MAX_SAMPLE_VALUE = 10000;

void fillWithRandomInts(std::vector<int>&, size_t);
void sortSample(std::vector<int>, bool);

int main()
{
    // Generate example samples
    constexpr size_t sample_count = 10; // Number of samples to generate
    constexpr size_t max_sample_size = 1000000; // Larger sample size

    std::vector<size_t> sizes = generateSizes(sample_count, max_sample_size, {.round_to = 200, .bias = 1.5});
    auto samples = generateSamples<int>(fillWithRandomInts, sizes);
    bool use_stable_sort = false; // Variable used in the example to decide whether to use sort or stable_sort

    // Measure execution time
    std::cout << "⏱️ Measuring function performance...\n";
    auto result = measureMultipleTimes<std::chrono::microseconds>(sortSample, samples, use_stable_sort);

    // Convert measurement results to a different unit
    auto converted_result = result.convertTo<std::chrono::milliseconds>();

    // Print converted results
    for (size_t i = 0; i < converted_result.times.size(); ++i)
    {
        std::cout << "Sample " << i + 1 << ": " << "| Time: " << converted_result.times[i].count() << " "
                  << converted_result.unit_symbol << "| Sample size: " << samples[i].size() << "\n";
    }

    // Serialize samples to a file
    serializeSamplesToFile(samples, serializeIterable<std::vector<int>>, "data/samples.csv");
    std::cout << "💾 Samples written to data/samples.csv\n";

    // Measure execution time and save the results to a file directly
    measureMultipleTimesToFile<std::chrono::microseconds>(sortSample, samples, "data/times.csv", use_stable_sort);
    std::cout << "💾 Results written to data/times.csv\n";

    return 0;
}

// Example function to measure (sort a sample)
void sortSample(std::vector<int> sample, bool use_stable_sort = false)
{
    if (use_stable_sort)
    {
        std::stable_sort(sample.begin(), sample.end());
        return;
    }
    std::sort(sample.begin(), sample.end());
}

// Example filler function (generate random integers)
void fillWithRandomInts(std::vector<int>& vec, size_t size)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(MIN_SAMPLE_VALUE, MAX_SAMPLE_VALUE);

    vec.clear();
    vec.reserve(size);

    for (size_t i = 0; i < size; ++i) vec.push_back(dist(rng));
}