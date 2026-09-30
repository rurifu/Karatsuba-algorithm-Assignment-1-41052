#include "MultiplyingAlgorithm.hpp"

BigInt schoolBookAlgorithm(const BigInt& a, const BigInt& b) {

	if (a.isZero() || b.isZero()) return BigInt();

	BigInt r;
	r.d.assign(a.d.size() + b.d.size(), 0);

	for (size_t i = 0; i < a.d.size(); ++i) {
		uint64_t carry = 0;
		for (size_t j = 0; j < b.d.size() || carry; ++j) {
			uint64_t cur = r.d[i + j] + carry;
			if (j < b.d.size()) {
				cur += static_cast<uint64_t>(a.d[i]) * b.d[j];
			}
			r.d[i + j] = static_cast<uint32_t>(cur % BigInt::BASE);
			carry = cur / BigInt::BASE;
		}
	}

};

BigInt karatsubaAlgorithm(const BigInt& a, const BigInt& b, size_t cutoff) {

	if (a.size() <= cutoff || b.size() <= cutoff) {
		return schoolBookAlgorithm(a, b);
	}

	size_t m = std::max(a.size(), b.size()) / 2;

	BigInt a0 = a.slice(0, std::min(m, a.size()));
	BigInt a1 = a.slice(std::min(m, a.size()), a.size());
	BigInt b0 = b.slice(0, std::min(m, b.size()));
	BigInt b1 = b.slice(std::min(m, b.size()), b.size());

	BigInt z0 = karatsubaAlgorithm(a0, b0, cutoff);
	BigInt z2 = karatsubaAlgorithm(a1, b1, cutoff);

	BigInt aSum = a0 + a1;
	BigInt bSum = b0 + b1;
	BigInt z1 = karatsubaAlgorithm(aSum, bSum, cutoff) - z0 - z2;

	return z2.shiftLimbs(2 * m) + z1.shiftLimbs(m) + z0;
};