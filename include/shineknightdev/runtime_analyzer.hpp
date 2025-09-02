#pragma once

#include <chrono>
#include <concepts>
#include <cstddef>
#include <functional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

// [Namespace definition]
namespace sra
{

// clang-format off
// [ChronoDuration concept]
template <typename T>
concept ChronoDuration = requires
{
    requires std::is_same_v<T, std::chrono::nanoseconds> || 
             std::is_same_v<T, std::chrono::microseconds> ||
             std::is_same_v<T, std::chrono::milliseconds> || 
             std::is_same_v<T, std::chrono::seconds> ||
             std::is_same_v<T, std::chrono::minutes> || 
             std::is_same_v<T, std::chrono::hours>;
};
// [<--]

// [HasSize concept]
template <typename T>
concept HasSize = requires(const T& t) { { t.size() } -> std::convertible_to<std::size_t>; };
// [<--]

// [get_unit_symbol function]
template <ChronoDuration T>
inline constexpr std::string_view get_unit_symbol() noexcept
{
    if constexpr (std::is_same_v<T, std::chrono::nanoseconds>) { return "ns"; }
    else if constexpr (std::is_same_v<T, std::chrono::microseconds>) { return "μs"; }
    else if constexpr (std::is_same_v<T, std::chrono::milliseconds>) { return "ms"; }
    else if constexpr (std::is_same_v<T, std::chrono::seconds>) { return "s"; }
    else if constexpr (std::is_same_v<T, std::chrono::minutes>) { return "min"; }
    else if constexpr (std::is_same_v<T, std::chrono::hours>) { return "h"; }
    else { return "units"; }
}
// [<--]
// clang-format on

// [RuntimeProfile struct]
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
// [<--]

// [measure_duration function]
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
// [<--]

// [profile_runtime function]
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
// [<--]

// [Utility functions]
template <ChronoDuration Unit>
[[nodiscard]] inline auto calculate_average(const RuntimeProfile<Unit>& profile) noexcept
{
    if (profile.raw_durations.empty()) return Unit::zero();

    typename Unit::rep total = 0;
    for (const auto& duration : profile.raw_durations) { total += duration.count(); }
    return Unit{total / profile.raw_durations.size()};
}
// [<--]

template <ChronoDuration Unit>
[[nodiscard]] inline auto calculate_total(const RuntimeProfile<Unit>& profile) noexcept
{
    typename Unit::rep total = 0;
    for (const auto& duration : profile.raw_durations) { total += duration.count(); }
    return Unit{total};
}
// [<--]

} // namespace sra
// [<--]