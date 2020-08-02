#include <USTL/cstring.h>

#include <iostream>

using namespace Ubpa::USTL;
using namespace std;

int main() {
	constexpr cstring a{ "123" };
	constexpr cstring b{ '4' };
	constexpr cstring c{ a, b };
	cout << c << endl;
	cout << reinterpret_cast<const void*>(c.data()) << endl;
	cout << cstring_integer<255, 2> << endl;
	cout << cstring_integer<255, 8> << endl;
	cout << cstring_integer<255, 10> << endl;
	cout << cstring_integer<255, 16> << endl;
	cout << cstring{ cstring{"__arg_"}, cstring_integer<1> } << endl;
}
