#include "satarith.hpp"

namespace satarith_test {
	template<typename T>
	struct Tester {
		using table = std::vector<struct record<T>>;
		std::string type_string;

		void test_function(T (*function)(T, T), std::string function_name, table tests, table negative_tests) {
			// Add negative_tests to tests unless type is unsigned.
			if (!is_unsigned<T>::value) {
				tests.insert(tests.end(), negative_tests.begin(), negative_tests.end());
			}

			// std::string type_name = static_cast<std::string>(typeid(T).name());

			for (int i = 0; i < static_cast<int>(tests.size()); i++) {
				struct record<T> record = tests[i];

				T got = function(record.args[0], record.args[1]);

				string name = function_name + "<" + type_string + ">(" + to_string(record.args[0]) + ", " + to_string(record.args[1]) + ")";
				test<T>(name, got == record.want, got, record.want);
			}
		}

		void add() {
			T min = numeric_limits<T>::lowest();
			T max = numeric_limits<T>::max();

			table tests = {
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
			table negative_tests = {
				// Non-overflow
				{{-1, 1}, 0},
				{{1, -1}, 0},

				// Overflow
				{{-1, min}, min},
				{{min, -1}, min},
			};
			#pragma GCC diagnostic pop

			test_function(satarith::add<T>, "add", tests, negative_tests);
		}
		
		void subtract() {
			T min = numeric_limits<T>::lowest();
			T max = numeric_limits<T>::max();

			table tests = {
				// Non-overflow
				{{5, 1}, 4},
				{{10, 5}, 5},

				// Overflow
				{{min, 1}, min},
			};
			
			// Ignore narrowing warning for negative tests because they are not run for unsigned types.
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wnarrowing"
			table negative_tests = {
				// Non-overflow
				{{-1, 1}, -2},
				{{1, -1}, 2},

				// Overflow
				{{-1, max}, min},
				{{min, 1}, min},
			};
			#pragma GCC diagnostic pop

			test_function(satarith::subtract<T>, "subtract", tests, negative_tests);
		}

		void multiply() {
			T min = numeric_limits<T>::lowest();
			T max = numeric_limits<T>::max();
				
			T max_2 = max / 2;
			T min_2 = min / 2;

			table tests = {
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
			
			// Ignore narrowing warning for negative tests because they are not run for unsigned types.
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wnarrowing"
			table negative_tests = {
				// Non-overflow
				{{-1, 1}, -1},
				{{1, -1}, -1},
				{{-1, -1}, 1},

				// Overflow
				// a positiveunsigned
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

			test_function(satarith::multiply<T>, "multiply", tests, negative_tests);
		}
	};

	template<typename T>
	void test(std::string type_string) {
		Tester<T> tester = {type_string};
		tester.add();
		tester.multiply();
		tester.subtract();
	}

	void test() {
		test<signed int>("signed int");
		test<unsigned int>("unsigned int");
		test<float>("float");

		// Test all numeric types.
		// Probably redundant but worth making sure.
		#ifdef ALL_TYPES
			// Print this here instead of from makefile just in case something is misconfigured. I want to print what is actually happening not what I'm trying to do.
			std::puts("Compiled tests for all types. (make test ALL_TYPES=false [-B] to disable)");

			test<signed char>       ("signed char");
			test<unsigned char>     ("unsigned char");
			test<signed short>      ("signed short");
			test<unsigned short>    ("unsigned short");
			test<signed long>       ("signed long");
			test<unsigned long>     ("unsigned long");
			test<signed long long>  ("signed long long");
			test<unsigned long long>("unsigned long long");
			test<double>            ("double");
			test<long double>       ("long double");
		#else
			std::puts("Warning: Only compiled tests for some types to save compile time. To disable this ensure make is not invoked with ALL_TYPES=false. You'll need to use make -B to force the update.");
		#endif
	}
}
