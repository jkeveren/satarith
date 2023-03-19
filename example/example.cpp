#include <iostream>
#include <limits>

#include "satarith.hpp"

using namespace std;

int main () {
	cout << satarith::add<short int>(numeric_limits<short int>::max(), 10000) << endl;
	cout << static_cast<int>(satarith::add<char>(numeric_limits<char>::max(), 100)) << endl;
	return 0;
}
