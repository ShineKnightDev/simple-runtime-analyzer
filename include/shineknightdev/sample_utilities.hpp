#pragma once

#include <cmath>
#include <cstddef>
#include <filesystem>
#include <format>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <ranges>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace sra
{

namespace detail
{

// Concept to check if a type can be inserted into an ostream
template <typename T>
concept StreamInsertable = requires(std::ostream& os, const T& t) {
    { os << t } -> std::same_as<std::ostream&>;
};

// Concept for a function that can fill a vector of type T
template <typename F, typename T>
concept FillerFunction = std::invocable<F, std::vector<T>&, size_t>;

// Concept for a function that serializes a sample
template <typename S, typename Sample>
concept SampleSerializer =
    std::invocable<S, const Sample&> && std::is_convertible_v<std::invoke_result_t<S, const Sample&>, std::string>;

inline std::string escape_json(const std::string& s) noexcept
{
    std::ostringstream o;
    for (auto c : s)
    {
        switch (c)
        {
        case '"': o << "\\\""; break;
        case '\\': o << "\\\\"; break;
        case '\b': o << "\\b"; break;
        case '\f': o << "\\f"; break;
        case '\n': o << "\\n"; break;
        case '\r': o << "\\r"; break;
        case '\t': o << "\\t"; break;
        default:
            if ('\x00' <= c && c <= '\x1f')
            {
                o << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
            }
            else { o << c; }
        }
    }
    return o.str();
}

} // namespace detail

struct SampleSizeConfig
{
    size_t round_to = 100;
    double bias = 1.0;
};

inline size_t round_to(size_t value, size_t multiple) noexcept
{
    if (multiple == 0) return value;
    return ((value + multiple / 2) / multiple) * multiple;
}

[[nodiscard]] inline std::vector<size_t>
generate_sizes(size_t sample_count, size_t max_sample_size, const SampleSizeConfig& config = {})
{
    if (sample_count == 0 || max_sample_size < config.round_to) { return {}; }
    if (sample_count == 1) { return {max_sample_size}; }

    const double log_min = std::log10(static_cast<double>(config.round_to));
    const double log_max = std::log10(static_cast<double>(max_sample_size));

    std::set<size_t> seen;

    // Oversample with bias
    const size_t oversample = sample_count * 3;
    for (size_t i = 0; i < oversample; ++i)
    {
        const double t = std::pow(static_cast<double>(i) / (oversample - 1), config.bias);
        const double log_size = log_min + t * (log_max - log_min);
        const size_t raw_size = static_cast<size_t>(std::round(std::pow(10.0, log_size)));
        const size_t rounded = round_to(raw_size, config.round_to);

        if (rounded <= max_sample_size) { seen.insert(rounded); }
    }

    // Ensure max_sample_size is included
    if (max_sample_size % config.round_to == 0) { seen.insert(max_sample_size); }

    std::vector<size_t> all_sizes(seen.begin(), seen.end());

    // Fill in gaps if needed
    while (all_sizes.size() < sample_count)
    {
        const size_t last = all_sizes.back();
        const size_t next = last + config.round_to;
        if (next <= max_sample_size) { all_sizes.push_back(next); }
        else { break; }
    }

    // Uniform downsampling
    std::vector<size_t> final_sizes;
    if (sample_count > 0 && !all_sizes.empty())
    {
        final_sizes.reserve(sample_count);
        const size_t available = all_sizes.size();

        if (sample_count == 1) { final_sizes.push_back(all_sizes.back()); }
        else
        {
            for (size_t i = 0; i < sample_count; ++i)
            {
                const double t = static_cast<double>(i) / (sample_count - 1);
                const size_t index = static_cast<size_t>(std::round(t * (available - 1)));
                final_sizes.push_back(all_sizes[index]);
            }
        }
    }

    // Ensure last value is max_sample_size
    if (!final_sizes.empty() && max_sample_size % config.round_to == 0) { final_sizes.back() = max_sample_size; }

    return final_sizes;
}

[[nodiscard]] inline std::vector<size_t> generate_sizes(size_t sample_count, size_t max_sample_size)
{
    return generate_sizes(sample_count, max_sample_size, SampleSizeConfig{});
}

template <typename T, detail::FillerFunction<T> F>
[[nodiscard]] std::vector<std::vector<T>> generate_samples(F&& filler, const std::vector<size_t>& sizes)
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

template <std::ranges::range Iterable>
requires detail::StreamInsertable<std::ranges::range_value_t<Iterable>>
[[nodiscard]] std::string serialize_iterable(const Iterable& container)
{
    if (std::ranges::empty(container)) { return "[]"; }

    auto it = std::ranges::begin(container);
    std::string result = std::format("[{}", *it);

    for (++it; it != std::ranges::end(container); ++it) { result += std::format(", {}", *it); }
    result += "]";

    return result;
}

template <std::ranges::range Container, detail::SampleSerializer<std::ranges::range_value_t<Container>> Serializer>
void save_samples(const Container& samples, Serializer&& serializer, const std::filesystem::path& filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) { throw std::runtime_error("Error: Could not open file " + filename.string()); }

    const auto ext = filename.extension();

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
        bool first = true;
        for (const auto& sample : samples)
        {
            if (!first) { file << ",\n"; }
            file << "  \"" << detail::escape_json(serializer(sample)) << "\"";
            first = false;
        }
        file << "\n]\n";
    }
    else { throw std::runtime_error("Error: Unsupported file extension " + ext.string()); }
}

template <std::ranges::range Container>
requires detail::StreamInsertable<std::ranges::range_value_t<Container>>
void save_samples(const Container& samples, const std::filesystem::path& filename)
{
    save_samples(samples, serialize_iterable<Container>, filename);
}

} // namespace sra