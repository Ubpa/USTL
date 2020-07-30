#pragma once

#include <tuple>

namespace Ubpa::USTL {
	template<bool... Masks, typename Tuple, typename Init, typename Func>
	constexpr auto tuple_accumulate(Tuple&&, Init&&, Func&&);
}

#include "detail/tuple.inl"
