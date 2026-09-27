#include "MultiplyingAlgorithm.hpp"

BigInt schoolBookAlgorithm(const BigInt& a, const BigInt& b) {

	if (a.isZero() || b.isZero()) return BigInt();

	BigInt r;
	r.d.assign(a.d.size() + b.d.size(), 0);

	for (size_t i = 0; i < a.d.size(); ++i) {
		std::ignore;
	}

};