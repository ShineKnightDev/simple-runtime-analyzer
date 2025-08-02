#include "process_sequence.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>

// Generates a random sequence of integers with values between 0 and 10,000
std::vector<int> generateRandomSequence(size_t n)
{
    std::vector<int> sequence(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 10000);

    for (size_t i = 0; i < n; ++i)
    {
        sequence[i] = distrib(gen);
    }
    return sequence;
}

int main()
{
    // Vector of sequence sizes to test
    const std::vector<size_t> sizes =
    {
        1000, 5000, 10000, 50000,
        100000, 500000, 1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000
    };

    std::ofstream file("runtime_metrics.txt");
    if (!file)
    {
        std::cerr << "Error opening file runtime_metrics.txt" << std::endl;
        return 1;
    }

    for (const auto& size : sizes)
    {
        auto sequence = generateRandomSequence(size);

        auto start = std::chrono::high_resolution_clock::now();
        processSequence(sequence);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;

        file << size << " " << duration.count() << "\n";
        std::cout << "Size: " << size
                  << ", Time: " << duration.count()
                  << " seconds\n";
    }

    file.close();
    return 0;
}