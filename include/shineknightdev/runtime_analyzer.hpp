#include <chrono>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <functional>

template <typename T>
inline std::string get_unit_symbol()
{
    if constexpr (std::is_same_v<T, std::chrono::nanoseconds>) { return "ns"; }
    else if constexpr (std::is_same_v<T, std::chrono::microseconds>) { return "μs"; }
    else if constexpr (std::is_same_v<T, std::chrono::milliseconds>) { return "ms"; }
    else if constexpr (std::is_same_v<T, std::chrono::seconds>) { return "s"; }
    else if constexpr (std::is_same_v<T, std::chrono::minutes>) { return "min"; }
    else if constexpr (std::is_same_v<T, std::chrono::hours>) { return "h"; }
    else { return "units"; }
}

template <typename Unit>
struct RuntimeProfile
{
    std::vector<Unit> raw_durations;
    std::vector<size_t> sample_sizes;
    std::string unit_symbol;
    Unit unit_value;

    RuntimeProfile(std::vector<Unit>&& _raw_durations,
                   std::vector<size_t> _sample_sizes,
                   std::string _unit_symbol,
                   Unit _unit_value)
        : raw_durations(std::move(_raw_durations))
        , sample_sizes(std::move(_sample_sizes))
        , unit_symbol(std::move(_unit_symbol))
        , unit_value(_unit_value)
    {}

    template <typename OtherUnit>
    RuntimeProfile(const RuntimeProfile<OtherUnit>& other)
    {
        unit_value = Unit{1};
        unit_symbol = get_unit_symbol<Unit>();
        sample_sizes = other.sample_sizes;

        if constexpr (std::is_same_v<Unit, OtherUnit>) { raw_durations = other.raw_durations; }
        else
        {
            raw_durations.reserve(other.raw_durations.size());
            for (const auto& t : other.raw_durations) { raw_durations.push_back(std::chrono::duration_cast<Unit>(t)); }
        }
    }

    template <typename UnitTo>
    RuntimeProfile<UnitTo> convert_to() const
    {
        return RuntimeProfile<UnitTo>(*this);
    }

};

// Measure execution time of a function
template <typename Unit = std::chrono::milliseconds, typename Func, typename... Args>
auto measure_duration(Func&& func, Args&&... args)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<Unit>(end - start);
    return duration;
}

template <typename Unit = std::chrono::milliseconds, typename Func, typename Container, typename... Args>
auto profile_runtime(Func&& func, const Container& samples, Args&&... args)
{
    std::vector<Unit> raw_durations;
    std::vector<size_t> sample_sizes;
    raw_durations.reserve(samples.size());

    for (const auto& sample : samples)
    {
        auto duration = measure_duration<Unit>([&func, &sample, &args...]() { std::invoke(func, sample, args...); });
        raw_durations.push_back(duration);
        sample_sizes.push_back(sample.size());
    }

    return RuntimeProfile<Unit>{std::move(raw_durations), std::move(sample_sizes), get_unit_symbol<Unit>(), Unit{1}};
}