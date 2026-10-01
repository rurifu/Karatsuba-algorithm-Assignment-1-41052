# Karatsuba-algorithm-Assignment-1-41052
Assignment 1 Mini Project for the course Advanced Algorithm 41052. This repository will focus mainly on the implementation of Karatsuba Algorithm and studies on how it compares to the grade school multiplication technique.

##Layout
```
bignum.hpp                 BigInt representation
MultiplyingAlgorithm.hpp   Declarations for both multiplication algorithms
MultiplyingAlgorithm.cpp   schoolBookAlgorithm and karatsubaAlgorithm
CorrectnessTest.cpp        Randomized correctness check (Karatsuba vs schoolBook)
BenchmarkHarness.cpp              Benchmarking harness -> results/benchmark-Release.csv
PlottingResult.py            Turns benchmark-Release.csv into plots + exponent fits
results/                   Output CSVs and plots land here
```

## Build

It requires CMake >= 3.10 and a C++17 compiler. Then type in the PowerShell

```
mkdir build && cd build
cmake ..
make
```

This produces two binaries in `build/`: `CorrectnessTest` and `BenchmarkHarness`.

## Run

Correctness check (should print "All N correctness checks passed."):

```
./build/CorrectnessTest
```

Benchmark (writes `results/benchmark-Release.csv`; takes a few seconds at the larger digit sizes

Edit `digitSizes` in `src/benchmark.cpp` to shrink the sweep for a quick run:

```
mkdir -p results
./build/benchmark results/benchmark.csv
```

Plots (requires `matplotlib`, `numpy`):

```
pip install matplotlib numpy
python3 scripts/plot_results.py results/benchmark.csv
```
