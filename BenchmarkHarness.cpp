#include "MultiplyingAlgorithm.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

using Clock = std::chrono::steady_clock;

// Times `fn()` and returns elapsed nanoseconds.
template <typename Fn>
long long timeIt(Fn&& fn) {
    auto start = Clock::now();
    fn();
    auto end = Clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

int main(int argc, char** argv) {
    // Digit sizes to sweep. Log-spaced so the log-log plot has even coverage.
    std::vector<int> digitSizes = {
        8, 16, 32, 64, 128, 256, 512, 1024,
        2048, 4096, 8192, 16384, 32768, 65536
    };

    // Karatsuba cutoffs (in limbs, ~9 decimal digits each) to compare.
    std::vector<size_t> cutoffs = { 1, 4, 8, 16, 32, 64 };

    int trialsPerSize = 15;
    std::string outPath = "results/benchmark.csv";
    if (argc > 1) outPath = argv[1];

    std::mt19937_64 rng(42);
    std::ofstream out(outPath);
    if (!out) {
        std::cerr << "Could not open output file: " << outPath << "\n";
        return 1;
    }
    out << "algorithm,cutoff,digits,trial,time_ns\n";

    for (int digits : digitSizes) {
        for (int trial = 0; trial < trialsPerSize; ++trial) {
            BigInt a = randomBigInt(digits, rng);
            BigInt b = randomBigInt(digits, rng);

            // Schoolbook baseline (cutoff column left as 0/NA -- not applicable).
            BigInt resultSchoolbook;
            long long tSchool = timeIt([&] { resultSchoolbook = schoolBookAlgorithm(a, b); });
            out << "schoolbook,NA," << digits << "," << trial << "," << tSchool << "\n";

            // Karatsuba at each cutoff.
            for (size_t cutoff : cutoffs) {
                BigInt resultK;
                long long tK = timeIt([&] { resultK = karatsubaAlgorithm(a, b, cutoff); });
                out << "karatsuba," << cutoff << "," << digits << "," << trial << "," << tK << "\n";
            }
        }
        std::cerr << "Finished digit size " << digits << "\n";
    }

    std::cout << "Results written to " << outPath << "\n";
    return 0;
}