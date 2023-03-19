#include "satarith.hpp"

namespace satarith_test {
	template<typename T>
	class Tester {
		using table = std::vector<struct record<T>>;

		void test_function(T (*function)(T a, T b), std::string function_name, table tests, table negative_tests) {
			// Add negative tests to tests unless type is unsigned.
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

	public:
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
		
		// void subtract() {
		// 	T min = numeric_limits<T>::lowest();
		// 	// T max = numeric_limits<T>::max();

		// 	table tests = {
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
		// 	table negative_tests = {
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
	void test() {
		Tester<T> tester = Tester<T>();
		tester.add();
		tester.multiply();
	}

	void test() {
		test<int>();
		test<unsigned int>();
		test<float>();

		// Comment these out while developing for improved compile times.
		#ifdef ALL_TYPES
			// Print this here instead of from makefile just in case something is misconfigured. I want to print what is actually happening not what I'm trying to do.
			std::puts("Compiled tests for all types. (make test ALL_TYPES=false [-B] to disable)");

			test<char>();
			test<unsigned char>();
			test<short int>();
			test<unsigned short int>();
			test<long int>();
			test<unsigned long int>();
			test<long long int>();
			test<unsigned long long int>();
			test<double>();
			test<long double>();
		#else
			puts("Warning: Only compiled tests for some types to save compile time. To disable this ensure make is not invoked with ALL_TYPES=false. You'll need to use make -B to force the update.");
		#endif
	}
}
