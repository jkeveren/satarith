#pragma once

#include <limits>

using namespace std;

namespace satarith {
	template<typename T>
	T add(T a, T b) {

		T max = numeric_limits<T>::max();
		if (b > 0 && max - b < a) {
			return max;
		}

		// numeric_limits<float>::min() returns ~0 which is not the minimum possible value.
		T min = numeric_limits<T>::lowest();
		if (b < 0 && min - b > a) {
			return min;
		}

		return a + b;
	}

	// template<typename T>
	// T subtract(T a, T b) {
	// 	return a - b;
	// }

	template<typename T>
	T multiply(T a, T b) {
		// Predict overflow
		T max = numeric_limits<T>::max();

		// Both positive
		if (a > 0 && b > 0 && max / a < b) {
			return max;
		}

		// numeric_limits<float>::min() returns ~0 which is not the minimum possible value.
		T min = numeric_limits<T>::lowest();

		// a positive
		if (a > 0 && b < 0 && min / a > b) {
			return min;
		}

		// b positive
		if (a < 0 && b > 0 && min / b > a) {
			return min;
		}

		// Both negative
		if (a < 0 && b < 0 && max / a > b) {
			return max;
		}
		
		// No overflow predicted
		return a * b;
	}
}