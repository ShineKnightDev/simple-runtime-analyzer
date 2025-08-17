#pragma once
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <random>
#include <set>
#include <vector>

struct SampleSizeConfig
{
    size_t round_to = 100;
    double bias = 1.0;
};

size_t roundTo(size_t value, size_t multiple)
{
    return ((value + multiple / 2) / multiple) * multiple;
}

std::vector<size_t> generateSizes(size_t sample_count, size_t max_sample_size, const SampleSizeConfig& config = {})
{
    std::vector<size_t> result;
    if (sample_count == 0 || max_sample_size < config.round_to) { return result; }

    double log_min = std::log10(static_cast<double>(config.round_to));
    double log_max = std::log10(static_cast<double>(max_sample_size));

    std::set<size_t> seen;

    // Oversample with bias
    size_t oversample = sample_count * 3;
    for (size_t i = 0; i < oversample; ++i)
    {
        double t = std::pow(static_cast<double>(i) / (oversample - 1), config.bias);
        double log_size = log_min + t * (log_max - log_min);
        size_t raw_size = static_cast<size_t>(std::round(std::pow(10.0, log_size)));
        size_t rounded = roundTo(raw_size, config.round_to);
        if (rounded <= max_sample_size) { seen.insert(rounded); }
    }

    // Ensure max_sample_size is included
    if (max_sample_size % config.round_to == 0) { seen.insert(max_sample_size); }

    std::vector<size_t> all_sizes(seen.begin(), seen.end());

    // Fill in gaps if needed
    while (all_sizes.size() < sample_count)
    {
        size_t last = all_sizes.back();
        size_t next = last + config.round_to;
        if (next <= max_sample_size) { all_sizes.push_back(next); }
        else { break; }
    }

    // Uniform downsampling
    std::vector<size_t> final_sizes;
    final_sizes.reserve(sample_count);
    size_t available = all_sizes.size();
    for (size_t i = 0; i < sample_count; ++i)
    {
        double t = static_cast<double>(i) / (sample_count - 1);
        size_t index = static_cast<size_t>(std::round(t * (available - 1)));
        final_sizes.push_back(all_sizes[index]);
    }

    // Step 5: Ensure last value is max_sample_size
    if (max_sample_size % config.round_to == 0) { final_sizes.back() = max_sample_size; }

    result = final_sizes;

    return result;
}

template <typename T, typename F>
std::vector<std::vector<T>> generateSamples(F&& filler, std::vector<size_t> sizes)
{
    std::vector<std::vector<T>> result;
    result.reserve(sizes.size());

    for (size_t size : sizes)
    {
        std::vector<T> sample;
        filler(sample, size);
        result.push_back(std::move(sample));
    }

    return result;
}

template <typename Iterable>
std::string serializeIterable(const Iterable& container)
{
    std::ostringstream oss;
    oss << "[";
    auto it = container.begin();
    while (it != container.end())
    {
        oss << *it;
        ++it;
        if (it != container.end()) { oss << ", "; }
    }
    oss << "]";
    return oss.str();
}

template <typename Container, typename Serializer>
void serializeSamplesToFile(const Container& samples, Serializer&& serializer, const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string ext = std::filesystem::path(filename).extension().string();

    if (ext == ".txt")
    {
        for (const auto& sample : samples) { file << serializer(sample) << "\n"; }
    }
    else if (ext == ".csv")
    {
        file << "sample_id,sample_data\n";
        size_t id = 1;
        for (const auto& sample : samples) { file << id++ << "," << serializer(sample) << "\n"; }
    }
    else if (ext == ".json")
    {
        file << "[\n";
        for (auto it = samples.begin(); it != samples.end(); ++it)
        {
            file << "  \"" << serializer(*it) << "\"";
            if (std::next(it) != samples.end()) { file << ","; }
            file << "\n";
        }
        file << "]\n";
    }
    else { std::cerr << "Error: Unsupported file extension '" << ext << "'\n"; }
}