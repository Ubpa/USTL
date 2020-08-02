#pragma once

#include <tuple>

namespace Ubpa::USTL {
	template<bool... Masks, typename Tuple, typename Init, typename Func>
	constexpr auto tuple_accumulate(Tuple&&, Init&&, Func&&);

	template<bool... Masks, typename Tuple, typename Func>
	constexpr void tuple_for_each(Tuple&&, Func&&);

	template<typename Tuple, typename Func>
	constexpr size_t tuple_find_if(const Tuple&, Func&&);

	template<typename Tuple, typename Elem>
	constexpr size_t tuple_find(const Tuple&, const Elem&);

	template<typename Tuple, typename Elem>
	constexpr bool tuple_constains(const Tuple&, const Elem&);

	template<typename Tuple, typename Func>
	constexpr size_t tuple_count_if(const Tuple&, Func&&);

	template<typename Tuple, typename Elem>
	constexpr size_t tuple_count(const Tuple&, const Elem&);

	template<typename Tuple, typename... Elems>
	constexpr auto tuple_append(Tuple&&, Elems&&...);

	template<typename Tuple, typename... Elems>
	constexpr auto tuple_prepend(Tuple&&, Elems&&...);
}

#include "detail/tuple.inl"
