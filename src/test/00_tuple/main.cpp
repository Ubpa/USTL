#include <USTL/tuple.h>

int main() {
	constexpr auto rst = Ubpa::USTL::tuple_accumulate<true, false, true>(std::tuple{ 1,2,3 }, 6, [](auto&& acc, auto i) {
		return acc + i;
	});
	static_assert(rst == 10);
}
