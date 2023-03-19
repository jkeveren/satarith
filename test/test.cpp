#include <iostream>

using namespace std;

unsigned int test_count;
unsigned int pass_count;
unsigned int fail_count;

// Record for table dirven tests
template<typename T>
struct record {
	T args[20];
	T want;
};

template<typename T>
void test(string name, bool pass, T got, T want) {
	test_count++;
	if (pass) {
		pass_count++;
		return;
	}
	fail_count++;

	cout << name << ": Got: " << got << ", Want: " << want << "." << endl;
}

#include "satarith_test.cpp"

int main() {
	puts("Testing.");

	satarith_test::test();

	cout << "Ran " << test_count << " tests.\n" << fail_count << " failed.\n" << pass_count << " passed." << endl;
	
	return 0;
}
