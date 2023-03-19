#include <iostream>
#include <vector>

// using namespace std;

unsigned int test_count;
unsigned int pass_count;
unsigned int fail_count;

// Record for table dirven tests
template<typename T>
struct record {
	std::vector<T> args;
	T want;
};

template<typename T>
void test(std::string name, bool pass, T got, T want) {
	test_count++;
	if (pass) {
		pass_count++;
		return;
	}
	fail_count++;

	std::cout << name << ": Got: " << got << ", Want: " << want << "." << std::endl;
}

#include "satarith_test.cpp"

int main() {
	puts("Testing.");

	satarith_test::test();

	std::cout << "Ran " << test_count << " tests.\n" << fail_count << " failed.\n" << pass_count << " passed." << std::endl;
	
	return 0;
}
