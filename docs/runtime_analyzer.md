# Runtime Analyzer Module

The **Runtime Analyzer Module** (`runtime_analyzer.hpp`) is a set of utilities for C++ developers designed to measure and profile the performance of functions and algorithms. This tool provides high-precision timing capabilities, compatibility with multiple time units, and functions for statistical analysis of results. Its design is based on modern C++23 concepts and ranges to ensure type safety and flexibility.

[TOC]

## Key Features

* **Accurate Timing**: Offers nanosecond to hour timing resolution using `std::chrono`.
* **Concept-Based Design**: Employs C++23 concepts to create type-safe interfaces.
* **Flexible Units**: Allows for automatic conversion between different time units.
* **Statistical Profiling**: Facilitates multi-sample profiling with the ability to track the size of each sample.
* **Modern C++**: Requires C++23, with support for concepts and ranges.

-----

## Concepts and Type Requirements

The module uses C++ concepts to enforce type safety and define clear requirements for templates.

* **`ChronoDuration`**: This concept limits template types to standard `std::chrono` durations (e.g., `nanoseconds`, `microseconds`, `milliseconds`).
* **`HasSize`**: This concept requires a container to have a `size()` method that returns a type convertible to `std::size_t`.

-----

## Core Components

### RuntimeProfile Structure

The `RuntimeProfile` struct is a templated container that stores timing results along with associated sample metadata, such as sample sizes. It is type-safe, supports conversion between time units, and uses move semantics for efficient handling of large data sets.

```cpp
template <ChronoDuration Unit>
struct RuntimeProfile
{
    std::vector<Unit> raw_durations;
    std::vector<size_t> sample_sizes;
    std::string unit_symbol;

    // Main constructor
    RuntimeProfile(std::vector<Unit>&& _raw_durations, std::vector<size_t>&& _sample_sizes) noexcept
        : raw_durations(std::move(_raw_durations))
        , sample_sizes(std::move(_sample_sizes))
        , unit_symbol(get_unit_symbol<Unit>())
    {}

    // Conversion constructor
    template <ChronoDuration OtherUnit>
    explicit RuntimeProfile(const RuntimeProfile<OtherUnit>& other)
        : sample_sizes(other.sample_sizes), unit_symbol(get_unit_symbol<Unit>())
    {
        raw_durations.reserve(other.raw_durations.size());
        if constexpr (std::is_same_v<Unit, OtherUnit>) { raw_durations = other.raw_durations; }
        else
        {
            for (const auto& t : other.raw_durations) { raw_durations.push_back(std::chrono::duration_cast<Unit>(t)); }
        }
    }

    // Conversion method
    template <ChronoDuration UnitTo>
    [[nodiscard]] RuntimeProfile<UnitTo> convert_to() const
    {
        return RuntimeProfile<UnitTo>(*this);
    }

    // Utility methods
    [[nodiscard]] std::size_t size() const noexcept { return raw_durations.size(); }
    [[nodiscard]] bool empty() const noexcept { return raw_durations.empty(); }
};
```

### measure_duration Function

The `measure_duration` function measures the time it takes for a single function or expression to execute. It's compatible with both value-returning (`non-void`) and non-value-returning (`void`) functions, using perfect forwarding for arguments. The time unit can be specified at compile time.

```cpp
template <ChronoDuration Unit = std::chrono::milliseconds, typename Func, typename... Args>
requires std::invocable<Func, Args...>
[[nodiscard]] auto measure_duration(Func&& func, Args&&... args)
{
    const auto start = std::chrono::high_resolution_clock::now();

    if constexpr (std::is_void_v<std::invoke_result_t<Func, Args...>>)
    {
        std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
    }
    else
    {
        [[maybe_unused]] auto result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
    }

    const auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<Unit>(end - start);
}
```

### profile_runtime Function

The `profile_runtime` function performs multi-sample profiling over a range of inputs. It requires that the samples container conforms to the `HasSize` concept. The function captures and forwards arguments safely, returning an aggregated `RuntimeProfile` object with all the results.

```cpp
template <ChronoDuration Unit = std::chrono::milliseconds,
          typename Func,
          std::ranges::range Container,
          typename... Args>
requires HasSize<std::ranges::range_value_t<Container>> &&
         std::invocable<Func, const std::ranges::range_value_t<Container>&, Args...>
[[nodiscard]] auto profile_runtime(Func&& func, const Container& samples, Args&&... args)
{
    if (std::ranges::empty(samples))
    {
        throw std::invalid_argument("Cannot profile runtime with empty samples container");
    }

    std::vector<Unit> raw_durations;
    std::vector<size_t> sample_sizes;
    const auto num_samples = std::ranges::size(samples);

    raw_durations.reserve(num_samples);
    sample_sizes.reserve(num_samples);

    // Capture arguments safely
    auto invoke_func = [&func, ... captured_args = std::forward<Args>(args)](const auto& sample) mutable {
        std::invoke(func, sample, std::forward<Args>(captured_args)...);
    };

    for (const auto& sample : samples)
    {
        auto duration = measure_duration<Unit>([&invoke_func, &sample]() { invoke_func(sample); });

        raw_durations.emplace_back(duration);
        sample_sizes.emplace_back(sample.size());
    }

    return RuntimeProfile<Unit>{std::move(raw_durations), std::move(sample_sizes)};
}
```

-----

## Utility Functions

* **`calculate_average`**: Computes the arithmetic mean of the durations recorded in a `RuntimeProfile`. It returns `Unit::zero()` for empty profiles.

    ```cpp
    template <ChronoDuration Unit>
    [[nodiscard]] inline auto calculate_average(const RuntimeProfile<Unit>& profile) noexcept
    {
        if (profile.raw_durations.empty()) return Unit::zero();

        typename Unit::rep total = 0;
        for (const auto& duration : profile.raw_durations) { total += duration.count(); }
        return Unit{total / profile.raw_durations.size()};
    }
    ```

* **`calculate_total`**: Computes the sum of all durations in a `RuntimeProfile`.

    ```cpp
    template <ChronoDuration Unit>
    [[nodiscard]] inline auto calculate_total(const RuntimeProfile<Unit>& profile) noexcept
    {
        typename Unit::rep total = 0;
        for (const auto& duration : profile.raw_durations) { total += duration.count(); }
        return Unit{total};
    }
    ```

-----

## Technical Considerations

The module is designed with minimal overhead and robust error handling in mind. Type validations are enforced at compile-time by concepts, and the `profile_runtime` function throws a `std::invalid_argument` if an empty container is provided. The utility functions provide a no-throw guarantee (`noexcept`).

<div class="section_buttons">

| Previous                      |                                           Next |
|:------------------------------|-----------------------------------------------:|
| [Usage Notes](usage_notes.md) | [Runtime Reporter Module](runtime_reporter.md) |

</div>
