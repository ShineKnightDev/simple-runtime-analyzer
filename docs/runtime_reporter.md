# Runtime Reporter Module

The **Runtime Reporter Module** (`runtime_reporter.hpp`) provides a set of tools for generating and exporting performance profiling results in various formats. This module integrates with the `RuntimeProfile` structure to offer flexible output options, including console display, file export to CSV, JSON, and text, and stream-based report generation for a wide range of use cases.

[TOC]

## Key Features

* **Multi-format Export**: Supports text, CSV, and JSON output formats.
* **Flexible Output**: Allows for console display, file export, and general stream handling.
* **Type-safe**: Templated on `ChronoDuration` units for consistency with `runtime_analyzer`.
* **Simple API**: Designed for easy and immediate integration.
* **Error Handling**: Provides clear exceptions for file and format-related issues.

-----

## Core Components

### print_report Function

The `print_report` function displays profiling results directly to the standard output (`std::cout`) in a human-readable format.

```cpp
template <ChronoDuration Unit>
void print_report(const RuntimeProfile<Unit>& runtime_profile)
{
    detail::write_text_report_impl(std::cout, runtime_profile);
}
```

### save_report Function

The `save_report` function exports profiling results to a file, automatically detecting the format based on the file extension.

```cpp
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
```

This function has a convenience overload that accepts a base filename as a `std::string` and automatically exports to a `.csv` file.

```cpp
template <ChronoDuration Unit>
void save_report(const RuntimeProfile<Unit>& runtime_profile, const std::string& base_filename = "runtime_report")
{
    const auto filename = std::filesystem::path(base_filename).replace_extension(".csv");
    save_report(runtime_profile, filename);
}
```

### save_reports Function

The `save_reports` function generates all three supported output formats simultaneously.

```cpp
template <ChronoDuration Unit>
void save_reports(const RuntimeProfile<Unit>& runtime_profile, const std::string& base_filename = "runtime_report")
{
    std::filesystem::path base_path(base_filename);

    save_report(runtime_profile, base_path.replace_extension(".csv"));
    save_report(runtime_profile, base_path.replace_extension(".json"));
    save_report(runtime_profile, base_path.replace_extension(".txt"));
}
```

### generate_report Function

The `generate_report` function provides a stream-based interface for report generation, allowing for flexible output handling to any `std::ostream` derivative, such as file streams, string streams, or network streams.

```cpp
template <ChronoDuration Unit, typename Stream>
requires std::derived_from<Stream, std::ostream>
void generate_report(Stream& stream, const RuntimeProfile<Unit>& runtime_profile, const std::string& format = "text")
{
    if (format == "text" || format == "txt") { detail::write_text_report_impl(stream, runtime_profile); }
    else if (format == "csv") { detail::write_csv_report_impl(stream, runtime_profile); }
    else if (format == "json") { detail::write_json_report_impl(stream, runtime_profile); }
    else { throw std::invalid_argument("Unsupported format: " + format); }
}
```

-----

## Technical Considerations

### Implementation Details

The core logic for writing each report format is encapsulated in private helper functions within the `detail` namespace. This design pattern ensures a clean public API and allows for code reuse across different output methods.

* `write_text_report_impl`: Formats the report for human-readable text output.
* `write_csv_report_impl`: Writes the data in a comma-separated format.
* `write_json_report_impl`: Creates a structured JSON array of objects.

```cpp
namespace detail
{

template <ChronoDuration Unit>
void write_text_report_impl(std::ostream& out, const RuntimeProfile<Unit>& profile);

template <ChronoDuration Unit>
void write_csv_report_impl(std::ostream& out, const RuntimeProfile<Unit>& profile);

template <ChronoDuration Unit>
void write_json_report_impl(std::ostream& out, const RuntimeProfile<Unit>& profile);

} // namespace detail
```

### Error Handling

The module provides clear error reporting to the user:

* `save_report` throws `std::runtime_error` if a file cannot be opened.
* `save_report` throws `std::runtime_error` for unsupported file extensions.
* `generate_report` throws `std::invalid_argument` for unsupported format specifiers.

<div class="section_buttons">

| Previous                                       |                                           Next |
|:-----------------------------------------------|-----------------------------------------------:|
| [Runtime Analyzer Module](runtime_analyzer.md) | [Sample Utilities Module](sample_utilities.md) |

</div>
