#include <USTL/compress_pair.h>

#include <iostream>

using namespace Ubpa::USTL;
using namespace std;

struct Empty {};

int main() {
	constexpr auto s1 = sizeof(compress_pair<Empty, int>);
	constexpr auto s2 = sizeof(std::pair<Empty, int>);
	static_assert(sizeof(compress_pair<Empty, int>) < sizeof(std::pair<Empty, int>));
	cout
		<< "sizeof(compress_pair<Empty, int>): " << s1 << endl
		<< "sizeof(std::pair<Empty, int>)    : " << s2 << endl;
}
