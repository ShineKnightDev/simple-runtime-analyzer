#pragma once

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>

#include "shineknightdev/runtime_analyzer.hpp"

// [Namespace definition]
namespace sra
{

// [detail namespace for implementation details]
namespace detail
{
// [write_text_report_impl function]
template <ChronoDuration Unit>
void write_text_report_impl(std::ostream& out, const RuntimeProfile<Unit>& profile)
{
    size_t i = 0;
    for (const auto& [duration, size] : std::views::zip(profile.raw_durations, profile.sample_sizes))
    {
        out << "Sample " << ++i << ": "
            << "| Time: " << duration.count() << " " << profile.unit_symbol << " | Sample size: " << size << "\n";
    }
}
// [<--]

// [write_csv_report_impl function]
template <ChronoDuration Unit>
void write_csv_report_impl(std::ostream& out, const RuntimeProfile<Unit>& profile)
{
    out << "sample_id,time_unit,time_value,sample_size\n";
    size_t i = 0;
    for (const auto& [duration, size] : std::views::zip(profile.raw_durations, profile.sample_sizes))
    {
        out << ++i << "," << profile.unit_symbol << "," << duration.count() << "," << size << "\n";
    }
}
// [<--]

// [write_json_report_impl function]
template <ChronoDuration Unit>
void write_json_report_impl(std::ostream& out, const RuntimeProfile<Unit>& profile)
{
    out << "[\n";
    bool first = true;
    size_t i = 0;

    for (const auto& [duration, size] : std::views::zip(profile.raw_durations, profile.sample_sizes))
    {
        if (!first) { out << ",\n"; }

        out << "  {\n"
            << "    \"sample_id\": " << ++i << ",\n"
            << "    \"time_unit\": \"" << profile.unit_symbol << "\",\n"
            << "    \"time_value\": " << duration.count() << ",\n"
            << "    \"sample_size\": " << size << "\n"
            << "  }";

        first = false;
    }
    out << "\n]\n";
}
// [<--]

} // namespace detail
// [<--]

// [print_report function]
template <ChronoDuration Unit>
void print_report(const RuntimeProfile<Unit>& runtime_profile)
{
    detail::write_text_report_impl(std::cout, runtime_profile);
}
// [<--]

// [save_report function]
template <ChronoDuration Unit>
void save_report(const RuntimeProfile<Unit>& runtime_profile, const std::filesystem::path& filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) { throw std::runtime_error("Error: Could not open file " + filename.string()); }

    const auto ext = filename.extension();

    if (ext == ".txt") { detail::write_text_report_impl(file, runtime_profile); }
    else if (ext == ".csv") { detail::write_csv_report_impl(file, runtime_profile); }
    else if (ext == ".json") { detail::write_json_report_impl(file, runtime_profile); }
    else { throw std::runtime_error("Error: Unsupported file extension " + ext.string()); }
}
// [<--]

// [save_report overload with default filename]
template <ChronoDuration Unit>
void save_report(const RuntimeProfile<Unit>& runtime_profile, const std::string& base_filename = "runtime_report")
{
    const auto filename = std::filesystem::path(base_filename).replace_extension(".csv");
    save_report(runtime_profile, filename);
}
// [<--]

// [Utility function to generate multiple report formats]
template <ChronoDuration Unit>
void save_reports(const RuntimeProfile<Unit>& runtime_profile, const std::string& base_filename = "runtime_report")
{
    save_report(runtime_profile, base_filename + ".csv");
    save_report(runtime_profile, base_filename + ".json");
    save_report(runtime_profile, base_filename + ".txt");
}
// [<--]

// [Stream-based report generation]
template <ChronoDuration Unit, typename Stream>
requires std::derived_from<Stream, std::ostream>
void generate_report(Stream& stream, const RuntimeProfile<Unit>& runtime_profile, const std::string& format = "text")
{
    if (format == "text" || format == "txt") { detail::write_text_report_impl(stream, runtime_profile); }
    else if (format == "csv") { detail::write_csv_report_impl(stream, runtime_profile); }
    else if (format == "json") { detail::write_json_report_impl(stream, runtime_profile); }
    else { throw std::invalid_argument("Unsupported format: " + format); }
}
// [<--]

} // namespace sra
// [<--]