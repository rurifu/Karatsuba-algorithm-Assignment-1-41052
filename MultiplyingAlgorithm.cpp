#include "MultiplyingAlgorithm.hpp"

BigInt schoolBookAlgorithm(const BigInt& a, const BigInt& b) {

	if (a.isZero() || b.isZero()) return BigInt();

	BigInt r;
	r.d.assign(a.d.size() + b.d.size(), 0);

	for (size_t i = 0; i < a.d.size(); ++i) {
		uint64_t carry = 0;
		for (size_t j = 0; j < b.d.size(); ++j) {
			uint64_t cur = r.d[i + j] + carry;
			if (j < b.d.size()) {
				cur += static_cast<uint64_t>(a.d[i] * b.d[j]);
			}
			r.d[i + j] = static_cast<uint32_t>(cur % BigInt::BASE);
			carry = cur / BigInt::BASE;
		}
	}

};