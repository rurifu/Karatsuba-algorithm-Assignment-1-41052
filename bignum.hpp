#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <random>
#include <algorithm>
#include <stdexcept>

// A big (arbitrary-precision) non-negative integer.
// Stored little-endian in base BASE (one limb ~ 9 decimal digits),
// so limb[0] is the least significant chunk.
struct BigInt {
    static constexpr uint64_t BASE = 1'000'000'000ULL; // 1e9
    std::vector<uint32_t> d; // limbs, little-endian, no leading (high-order) zero limbs except {0}

    BigInt() : d{ 0 } {}

    static BigInt fromDecimalString(const std::string& s) {
        BigInt r;
        r.d.clear();
        int len = static_cast<int>(s.size());
        for (int i = len; i > 0; i -= 9) {
            int start = std::max(0, i - 9);
            r.d.push_back(static_cast<uint32_t>(std::stoul(s.substr(start, i - start))));
        }
        if (r.d.empty()) r.d.push_back(0);
        r.trim();
        return r;
    }

    std::string toDecimalString() const {
        std::string s = std::to_string(d.back());
        for (int i = static_cast<int>(d.size()) - 2; i >= 0; --i) {
            std::string chunk = std::to_string(d[i]);
            s += std::string(9 - chunk.size(), '0') + chunk;
        }
        return s;
    }

    void trim() {
        while (d.size() > 1 && d.back() == 0) d.pop_back();
    }

    bool isZero() const { return d.size() == 1 && d[0] == 0; }

    // Number of limbs (used for splitting / choosing recursion cutoff).
    size_t size() const { return d.size(); }

    // limbs [lo, hi) as a standalone BigInt (used by Karatsuba to split).
    BigInt slice(size_t lo, size_t hi) const {
        BigInt r;
        r.d.clear();
        for (size_t i = lo; i < hi && i < d.size(); ++i) r.d.push_back(d[i]);
        if (r.d.empty()) r.d.push_back(0);
        r.trim();
        return r;
    }

    // Multiply by BASE^k, i.e. prepend k zero limbs.
    BigInt shiftLimbs(size_t k) const {
        if (isZero()) return BigInt();
        BigInt r;
        r.d.assign(k, 0);
        r.d.insert(r.d.end(), d.begin(), d.end());
        return r;
    }

    friend bool operator<(const BigInt& a, const BigInt& b) {
        if (a.d.size() != b.d.size()) return a.d.size() < b.d.size();
        for (size_t i = a.d.size(); i-- > 0;)
            if (a.d[i] != b.d[i]) return a.d[i] < b.d[i];
        return false;
    }
    friend bool operator==(const BigInt& a, const BigInt& b) { return a.d == b.d; }

    friend BigInt operator+(const BigInt& a, const BigInt& b) {
        BigInt r;
        r.d.assign(std::max(a.d.size(), b.d.size()) + 1, 0);
        uint64_t carry = 0;
        for (size_t i = 0; i < r.d.size(); ++i) {
            uint64_t s = carry;
            if (i < a.d.size()) s += a.d[i];
            if (i < b.d.size()) s += b.d[i];
            r.d[i] = static_cast<uint32_t>(s % BASE);
            carry = s / BASE;
        }
        r.trim();
        return r;
    }

    // Requires a >= b (caller's responsibility; true for every use in Karatsuba here).
    friend BigInt operator-(const BigInt& a, const BigInt& b) {
        if (a < b) throw std::invalid_argument("BigInt subtraction requires a >= b");
        BigInt r;
        r.d.assign(a.d.size(), 0);
        int64_t borrow = 0;
        for (size_t i = 0; i < a.d.size(); ++i) {
            int64_t v = static_cast<int64_t>(a.d[i]) - borrow - (i < b.d.size() ? b.d[i] : 0);
            if (v < 0) { v += BASE; borrow = 1; }
            else { borrow = 0; }
            r.d[i] = static_cast<uint32_t>(v);
        }
        r.trim();
        return r;
    }
};

// Generate a random BigInt with approximately `numDigits` decimal digits.
inline BigInt randomBigInt(int numDigits, std::mt19937_64& rng) {
    std::string s;
    std::uniform_int_distribution<int> firstDigit(1, 9);
    std::uniform_int_distribution<int> anyDigit(0, 9);
    s += static_cast<char>('0' + firstDigit(rng));
    for (int i = 1; i < numDigits; ++i) s += static_cast<char>('0' + anyDigit(rng));
    return BigInt::fromDecimalString(s);
}