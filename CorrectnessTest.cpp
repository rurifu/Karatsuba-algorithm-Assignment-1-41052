#include "MultiplyingAlgorithm.hpp"
#include <iostream>
#include <random>

// Cross-checks karatsubaAlgorithm against schoolbookAlgorithm (the trusted
// reference) across many random sizes and a few edge cases. This is NOT a
// proof of correctness against an independent implementation. This is for
//checking if both of the algorithms work as intended/comes to the same result.

int main() {
    std::mt19937_64 rng(12345);
    int failures = 0;
    int trials = 500;

    std::uniform_int_distribution<int> digitDist(1, 400);
    std::uniform_int_distribution<size_t> cutoffDist(1, 20);

    for (int t = 0; t < trials; ++t) {
        int da = digitDist(rng);
        int db = digitDist(rng);
        size_t cutoff = cutoffDist(rng);

        BigInt a = randomBigInt(da, rng);
        BigInt b = randomBigInt(db, rng);

        BigInt expected = schoolBookAlgorithm(a, b);
        BigInt actual = karatsubaAlgorithm(a, b, cutoff);

        if (!(expected == actual)) {
            std::cerr << "MISMATCH at trial " << t
                << " (digits " << da << "x" << db
                << ", cutoff " << cutoff << ")\n";
            ++failures;
        }
    }

    // A few explicit edge cases.
    {
        BigInt zero = BigInt::fromDecimalString("0");
        BigInt x = randomBigInt(50, rng);
        if (!(schoolBookAlgorithm(zero, x) == karatsubaAlgorithm(zero, x, 4))) {
            std::cerr << "MISMATCH on zero case\n";
            ++failures;
        }
    }
    {
        // Wildly unbalanced operand sizes.
        BigInt big = randomBigInt(500, rng);
        BigInt small = randomBigInt(3, rng);
        if (!(schoolBookAlgorithm(big, small) == karatsubaAlgorithm(big, small, 4))) {
            std::cerr << "MISMATCH on unbalanced-size case\n";
            ++failures;
        }
    }

    if (failures == 0) {
        std::cout << "All " << trials + 2 << " correctness checks passed.\n";
        return 0;
    }
    else {
        std::cout << failures << " checks FAILED.\n";
        return 1;
    }
}