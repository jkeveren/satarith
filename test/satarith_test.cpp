#include "satarith.hpp"

namespace satarith_test {
	template<typename T>
	using table = std::vector<struct record<T>>;

	template<typename T>
	void test_function(T (*function)(T a, T b), std::string function_name, table<T> tests, table<T> negative_tests) {
		// Add negative tests to tests unless type is negative.
		if (!is_unsigned<T>::value) {
			tests.insert(tests.end(), negative_tests.begin(), negative_tests.end());
		}

		std::string type_name = static_cast<std::string>(typeid(T).name());

		for (int i = 0; i < static_cast<int>(tests.size()); i++) {
			struct record<T> record = tests[i];

			T got = function(record.args[0], record.args[1]);

			string name = function_name + "<" + type_name + ">(" + to_string(record.args[0]) + ", " + to_string(record.args[1]) + ")";
			test<T>(name, got == record.want, got, record.want);
		}
	}

	template<typename T>
	void add() {
		T min = numeric_limits<T>::lowest();
		T max = numeric_limits<T>::max();

		table<T> tests = {
			// Non-overflow
			{{1, 1}, 2},
			{{5, 10}, 15},

			// Overflow
			{{max, 1}, max},
			{{1, max}, max},
		};

		// Ignore narrowing warning for negative tests because they are not run for unsigned types.
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		table<T> negative_tests = {
			// Non-overflow
			{{-1, 1}, 0},
			{{1, -1}, 0},

			// Overflow
			{{-1, min}, min},
			{{min, -1}, min},
		};
		#pragma GCC diagnostic pop

		test_function<T>(satarith::add<T>, "add", tests, negative_tests);
	}
	
	// template<typename T>
	// void subtract() {
	// 	T min = numeric_limits<T>::lowest();
	// 	// T max = numeric_limits<T>::max();

	// 	table<T> tests = {
	// 		// Non-overflow
	// 		{{5, 1}, 4},
	// 		{{10, 5}, 5},

	// 		// Overflow
	// 		{{min, 1}, min},
	// 		// {{1, max}, max},
	// 	};
		
	// 	// Ignore narrowing warning for negative tests because they are not run for unsigned types.
	// 	#pragma GCC diagnostic push
	// 	#pragma GCC diagnostic ignored "-Wnarrowing"
	// 	table<T> negative_tests = {
	// 		// // Non-overflow
	// 		// {{-1, 1}, 0},
	// 		// {{1, -1}, 0},

	// 		// // Overflow
	// 		// {{-1, min}, min},
	// 		// {{min, -1}, min},
	// 	};
	// 	#pragma GCC diagnostic pop

	// 	test_function<T>(satarith::subtract<T>, "subtract", tests, negative_tests);
	// }

	template<typename T>
	void multiply() {
		T min = numeric_limits<T>::lowest();
		T max = numeric_limits<T>::max();
			
		T max_2 = max / 2;
		T min_2 = min / 2;

		table<T> tests = {
			// Non-overflow
			{{1, 1}, 1},
			{{5, 10}, 50},

			// Overflow
			// Use max / 2 * 3 to avoid cases where overflow would also result in the correct result.
			// For example max * n == max because it overflows and wraps all the way back up n times.

			// Overflow
			// Both positive
			{{max_2, 3}, max},
			{{3, max_2}, max},
		};
		
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wnarrowing"
		table<T> negative_tests = {
			// Non-overflow
			{{-1, 1}, -1},
			{{1, -1}, -1},
			{{-1, -1}, 1},

			// Overflow
			// a positive
			{{max_2, -3}, min},
			{{2, min_2}, min},

			// b positive
			{{min_2, 3}, min},
			{{-3, max_2}, min},
			
			// Both negative
			{{min_2, -3}, max},
			{{-3, min_2}, max},
		};
		#pragma GCC diagnostic pop

		test_function<T>(satarith::multiply<T>, "multiply", tests, negative_tests);
	}

	#define call_with_arithmetic_types(f)\
		f<char>();\
		f<unsigned char>();\
		f<short int>();\
		f<unsigned short int>();\
		f<int>();\
		f<unsigned int>();\
		f<long int>();\
		f<unsigned long int>();\
		f<long long int>();\
		f<unsigned long long int>();\
		f<float>();\
		f<double>();\
		f<long double>();

	void test() {
		call_with_arithmetic_types(add);
		call_with_arithmetic_types(multiply);
		// subtract<short int>();
	}
}
