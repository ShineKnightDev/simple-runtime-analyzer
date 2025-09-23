# Usage Notes and Best Practices

This section provides practical and safety tips for using the **Simple Runtime Analyzer** library effectively. By following these recommendations, you can avoid common errors and ensure the reliability of your performance analysis.

[TOC]

## General Recommendations

* **Compiler Requirements**: The library makes extensive use of **C++20** features, and specifically **C++23** for `std::views::zip`. Therefore, it's essential to use a compiler that fully supports this version of the standard (e.g., GCC 13+, Clang 16+, or MSVC 19.34+).
* **Error Handling**: For robust code, always wrap file-handling function calls, such as `sra::save_report` and `sra::save_samples`, in `try-catch` blocks to gracefully handle exceptions like `std::runtime_error`.

---

## Tips for Runtime Analyzer

* **Avoid Empty Containers**: When calling `sra::profile_runtime`, ensure that your sample container is not empty. Failure to do so will throw a `std::invalid_argument` exception. You can use `std::ranges::empty(samples)` to perform a safe check before profiling.
* **Specify Time Units**: When using `sra::measure_duration`, it's a good practice to specify the desired time unit, as the default is milliseconds (`ms`). For example, use `measure_duration<std::chrono::nanoseconds>(...)` for high-precision measurements.
* **Handle Empty `RuntimeProfile`**: Statistical analysis functions like `sra::calculate_average` and `sra::calculate_total` handle empty profiles safely. If a profile contains no data, they will simply return a zero value for the corresponding time unit, so you don't need to perform a manual check in your code.

---

## Tips for Sample Utilities

* **Argument Validation**: Before calling `sra::generate_sizes`, note that the function will return an empty container if `sample_count` is 0 or if `max_sample_size` is less than the `round_to` value in the configuration.
* **Use Filler Functions**: The functions you create to fill the samples must comply with the `sra::detail::FillerFunction` concept. This is a design requirement to ensure type safety and the correct operation of `sra::generate_samples`.
* **Valid File Extensions**: When saving samples with `sra::save_samples`, be sure to use the supported file extensions: `.txt`, `.csv`, or `.json`. Using an unsupported extension will throw a `std::runtime_error` exception.
* **Special Characters in JSON**: Serialization to JSON is robust. The `sample_utils.hpp` code uses an internal `detail::escape_json` function to automatically escape special characters in data strings. This ensures the generated JSON file is always valid and can be processed by other programs without syntax errors.

---

## Tips for Runtime Reporter

* **Valid File Extensions**: As with `save_samples`, the `sra::save_report` function only accepts the extensions `.txt`, `.csv`, and `.json`. Incorrect usage of this function will throw a `std::runtime_error` exception.
* **Format Specification**: When using the `sra::generate_report` function to output reports to `std::ostream` or `std::stringstream`, specify the format using one of the valid strings: `"text"`, `"csv"`, or `"json"`. Any other value will throw a `std::invalid_argument` exception.

<div class="section_buttons">

| Previous                  |                                           Next |
|:--------------------------|-----------------------------------------------:|
| [Main Page](../README.md) | [Runtime Analyzer Module](runtime_analyzer.md) |

</div>
