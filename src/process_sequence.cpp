#include "process_sequence.hpp"
#include <algorithm>

// Example processing: sort, remove duplicates, and compute cumulative sums
void processSequence(std::vector<int>& sequence)
{
    if (sequence.empty()) return;

    std::sort(sequence.begin(), sequence.end());
    auto last = std::unique(sequence.begin(), sequence.end());
    sequence.erase(last, sequence.end());

    for (size_t i = 1; i < sequence.size(); ++i)
    {
        sequence[i] += sequence[i - 1];
    }
}