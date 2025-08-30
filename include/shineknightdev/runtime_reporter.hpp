#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <vector>

#include "shineknightdev/runtime_analyzer"

template <typename Unit>
void print_report(const RuntimeProfile<Unit>& runtime_profile)
{
    for (size_t i = 0; i < runtime_profile.raw_durations.size(); ++i)
    {
        std::cout << "Sample " << i + 1 << ": " << "| Time: " << runtime_profile.raw_durations[i].count() << " "
                  << runtime_profile.unit_symbol << "| Sample size: " << runtime_profile.sample_sizes[i] << "\n";
    }
}

template <typename Unit>
void save_report(const RuntimeProfile<Unit>& runtime_profile, const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: Could not open file " + filename);
    }

    std::string ext = std::filesystem::path(filename).extension().string();

    if (ext == ".txt")
    {
        for (size_t i = 0; i < runtime_profile.raw_durations.size(); ++i)
        {
            file << "Sample " << i + 1 << ": " << "| Time: " << runtime_profile.raw_durations[i].count() << " "
                 << runtime_profile.unit_symbol << "| Sample size: " << runtime_profile.sample_sizes[i] << "\n";
        }
    }
    else if (ext == ".csv")
    {
        file << "sample_id,time_unit,time_value,sample_size\n";
        for (size_t i = 0; i < runtime_profile.raw_durations.size(); ++i)
        {
            file << i + 1 << "," << runtime_profile.unit_symbol << "," << runtime_profile.raw_durations[i].count()
                 << "," << runtime_profile.sample_sizes[i] << "\n";
        }
    }
    else if (ext == ".json")
    {
        file << "[\n";
        for (size_t i = 0; i < runtime_profile.raw_durations.size(); ++i)
        {
            file << "  {\n"
                 << "    \"sample_id\": " << i + 1 << ",\n"
                 << "    \"time_unit\": \"" << runtime_profile.unit_symbol << "\",\n"
                 << "    \"time_value\": " << runtime_profile.raw_durations[i].count() << ",\n"
                 << "    \"sample_size\": " << runtime_profile.sample_sizes[i] << "\n"
                 << "  }";

            if (i + 1 != runtime_profile.raw_durations.size()) { file << ","; }

            file << "\n";
        }
        file << "]\n";
    }
    else { throw std::runtime_error("Error: Unsupported file extension "+ ext);}
}