#pragma once
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

template <typename Unit>
struct MeasurementResult
{
    std::vector<Unit> times;
    std::string unit_symbol;
    Unit unit_value;

    MeasurementResult(std::vector<Unit>&& _times, std::string _unit_symbol, Unit _unit_value)
        : times(std::move(_times)), unit_symbol(std::move(_unit_symbol)), unit_value(_unit_value)
    {}

    template <typename OtherUnit>
    MeasurementResult(const MeasurementResult<OtherUnit>& other)
    {
        unit_value = Unit{1};
        unit_symbol = getSymbol<Unit>();

        if constexpr (std::is_same_v<Unit, OtherUnit>) { times = other.times; }
        else
        {
            for (const auto& t : other.times) { times.push_back(std::chrono::duration_cast<Unit>(t)); }
        }
    }

    template <typename UnitTo>
    MeasurementResult<UnitTo> convertTo() const
    {
        return MeasurementResult<UnitTo>(*this);
    }

private:
    template <typename T>
    static std::string getSymbol()
    {
        if constexpr (std::is_same_v<T, std::chrono::nanoseconds>) { return "ns"; }
        else if constexpr (std::is_same_v<T, std::chrono::microseconds>) { return "μs"; }
        else if constexpr (std::is_same_v<T, std::chrono::milliseconds>) { return "ms"; }
        else if constexpr (std::is_same_v<T, std::chrono::seconds>) { return "s"; }
        else { return "units"; }
    }
};

//#region Time units
// clang-format off
template <typename Unit> std::string getUnitSymbol();

template <> inline std::string getUnitSymbol<std::chrono::nanoseconds>() { return "ns"; }

template <> inline std::string getUnitSymbol<std::chrono::microseconds>() { return "μs"; }

template <> inline std::string getUnitSymbol<std::chrono::milliseconds>() { return "ms"; }

template <> inline std::string getUnitSymbol<std::chrono::seconds>() { return "s"; }

template <> inline std::string getUnitSymbol<std::chrono::minutes>() { return "min"; }

template <> inline std::string getUnitSymbol<std::chrono::hours>() { return "h"; }
// clang-format on
//#endregion

// Measure execution time of a function
template <typename Unit = std::chrono::milliseconds, typename Func, typename... Args>
auto measureTime(Func&& func, Args&&... args)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<Unit>(end - start);
    return duration;
}

template <typename Unit = std::chrono::milliseconds, typename Func, typename Container, typename... Args>
auto measureMultipleTimes(Func&& func, const Container& samples, Args&&... args)
{
    std::vector<Unit> times;
    times.reserve(samples.size());

    for (const auto& sample : samples)
    {
        auto duration = measureTime<Unit>([&func, &sample, &args...]() { std::invoke(func, sample, args...); });
        times.push_back(duration);
    }

    return MeasurementResult<Unit>{std::move(times), getUnitSymbol<Unit>(), Unit{1}};
}

template <typename Unit = std::chrono::microseconds, typename Func, typename Container, typename... Args>
void measureMultipleTimesToFile(Func&& func, const Container& samples, const std::string& filename, Args&&... args)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string ext = std::filesystem::path(filename).extension().string();
    size_t sample_count = 1;

    std::string unit_str = std::is_same_v<Unit, std::chrono::nanoseconds>    ? "ns"
                           : std::is_same_v<Unit, std::chrono::microseconds> ? "μs"
                           : std::is_same_v<Unit, std::chrono::milliseconds> ? "ms"
                           : std::is_same_v<Unit, std::chrono::seconds>      ? "s"
                           : std::is_same_v<Unit, std::chrono::minutes>      ? "min"
                           : std::is_same_v<Unit, std::chrono::hours>        ? "h"
                                                                             : "units";

    if (ext == ".txt")
    {
        for (const auto& sample : samples)
        {
            auto duration = measureTime<Unit>([&func, &sample, &args...]() { std::invoke(func, sample, args...); });

            file << "Sample " << sample_count++ << ": " << "| Time: " << duration.count() << " " << unit_str
                 << "| Sample size: " << sample.size() << "\n";
        }
    }
    else if (ext == ".csv")
    {
        file << "sample_id,time_unit,time_value,sample_size\n";
        for (const auto& sample : samples)
        {
            auto duration = measureTime<Unit>([&func, &sample, &args...]() { std::invoke(func, sample, args...); });

            file << sample_count++ << "," << unit_str << "," << duration.count() << "," << sample.size() << "\n";
        }
    }
    else if (ext == ".json")
    {
        file << "[\n";
        for (auto it = samples.begin(); it != samples.end(); ++it)
        {
            auto duration = measureTime<Unit>([&func, &it, &args...]() { std::invoke(func, *it, args...); });

            file << "  {\n"
                 << "    \"sample_id\": " << sample_count++ << ",\n"
                 << "    \"time_unit\": \"" << unit_str << "\",\n"
                 << "    \"time_value\": " << duration.count() << ",\n"
                 << "    \"sample_size\": " << it->size() << "\n"
                 << "  }";

            if (std::next(it) != samples.end()) { file << ","; }

            file << "\n";
        }
        file << "]\n";
    }
    else { std::cerr << "Error: Unsupported file extension '" << ext << "'\n"; }
}