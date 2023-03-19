#include "satarith.hpp"

#include <vector>

namespace satarith_test {
	template<typename T>
	void add() {
		T min = numeric_limits<T>::lowest();
		T max = numeric_limits<T>::max();

		vector<struct record<T>> table = {
			// Non-overflow
			{{1, 1}, 2},
			{{5, 10}, 15},

			// Overflow
			{{max, 1}, max},
			{{1, max}, max},
		};

		// Skip negative number tests for unsigned types.
		if (!is_unsigned<T>::value) {
			// Ignore narrowing warning because T is never unsigned here.
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wnarrowing"
			vector<struct record<T>> negatives = {
				// Non-overflow
				{{-1, 1}, 0},
				{{1, -1}, 0},

				// Overflow
				{{-1, min}, min},
				{{min, -1}, min},
			};
			#pragma GCC diagnostic pop

			table.insert(table.end(), negatives.begin(), negatives.end());
		}

		// cout << table.size() << endl;

		string type_name = static_cast<string>(typeid(T).name());

		for (int i = 0; i < static_cast<int>(table.size()); i++) {
			struct record<T> record = table[i];

			T got = satarith::add<T>(record.args[0], record.args[1]);

			string name = "add<" + type_name + ">(" + to_string(record.args[0]) + ", " + to_string(record.args[1]) + ")";
			test<T>(name, got == record.want, got, record.want);
		}
	}

	template<typename T>
	void multiply() {
		T min = numeric_limits<T>::lowest();
		T max = numeric_limits<T>::max();
			
		T max_2 = max / 2;
		T min_2 = min / 2;

		vector<struct record<T>> table = {
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

		// Skip negative number tests for unsigned types.
		if (!is_unsigned<T>::value) {
			// Ignore narrowing warning because T is never unsigned here.
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wnarrowing"
			vector<struct record<T>> negatives = {
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

			table.insert(table.end(), negatives.begin(), negatives.end());
		}

		for (int i = 0; i < static_cast<int>(table.size()); i++) {
			struct record<T> record = table[i];

			T got = satarith::multiply<T>(record.args[0], record.args[1]);

			string name = "multiply(" + to_string(record.args[0]) + ", " + to_string(record.args[1]) + ")";
			test<T>(name, got == record.want, got, record.want);
		}
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
	}
}
