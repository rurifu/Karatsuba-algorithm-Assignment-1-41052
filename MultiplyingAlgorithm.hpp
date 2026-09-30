#ifndef KARATSUBA_PROJECT_MULTIPLYALGORITHM_HPP
#define KARATSUBA_PROJECT_MULTIPLYALGORITHM_HPP

#include "bignum.hpp"

BigInt schoolBookAlgorithm(const BigInt& a, const BigInt& b);
BigInt karatsubaAlgorithm(const BigInt& a, const BigInt& b, size_t cutoff);

#endif //KARATSUBA_PROJECT_MULTIPLYALGORITHM_HPP