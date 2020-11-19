#include <USTL/memory.h>

#include <iostream>

using namespace Ubpa::USTL;
using namespace std;

#include <memory>
#include <map>
#include <unordered_map>

class A {
public:
	virtual ~A() = default;
	float x, y;
};
class B : public A {
	public:
		float z, w;
};

int main() {
	{ // shared
		shared_object<A> so0;
		shared_object<A> so1{ nullptr };
		const shared_object<A> so2{ new A };
		auto so3 = make_shared_object<B>();
		shared_object<A[]> so4{ new A[5] };
		cout << so4[3].x << endl;
		std::unordered_map<shared_object<int>, size_t> m1; // hash
		std::map<shared_object<int>, size_t> m2; // <
		std::map<shared_object<int>, size_t, std::owner_less<shared_object<int>>> m3; // owner_before
	}
	{ // unique
		unique_object<int[]> uo0;
		unique_object<int[]> uo1{ nullptr };
		unique_object<int[]> uo2{ new int[5] };
		cout << uo2[3] << endl;
		auto uo3 = make_unique_object<B>();
		unique_object<A[]> uo4{ new A[5] };
		std::unordered_map<unique_object<int>, size_t> m1; // hash
		std::map<unique_object<int>, size_t> m2; // <
	}
}
