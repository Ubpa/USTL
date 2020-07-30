#pragma once

#include <utility>

namespace Ubpa::USTL::detail {
	template<typename Ints> struct IntegerSequenceTraits;

	template<typename T, T N0, T... Ns>
	struct IntegerSequenceTraits<std::integer_sequence<T, N0, Ns...>> {
		static constexpr size_t head = N0;
		static constexpr auto tail = std::integer_sequence<T, Ns...>{};
	};

	template<typename Tuple, typename Acc, typename Func, bool... Masks, size_t... Ns>
	constexpr auto tuple_accumulate(Tuple&& t, Acc&& acc, Func&& f,
		std::integer_sequence<bool, Masks...>, std::index_sequence<Ns...>)
	{
		if constexpr (sizeof...(Ns) > 0) {
			using IST_Ns = IntegerSequenceTraits<std::index_sequence<Ns...>>;
			if constexpr (sizeof...(Masks) > 0) {
				using IST_Masks = IntegerSequenceTraits<std::integer_sequence<bool, Masks...>>;
				if constexpr (IST_Masks::head) {
					return tuple_accumulate(
						std::forward<Tuple>(t),
						f(std::forward<Acc>(acc), std::get<IST_Ns::head>(std::forward<Tuple>(t))),
						std::forward<Func>(f),
						IST_Masks::tail,
						IST_Ns::tail
					);
				}
				else {
					return tuple_accumulate(
						std::forward<Tuple>(t),
						std::forward<Acc>(acc),
						std::forward<Func>(f),
						IST_Masks::tail,
						IST_Ns::tail
					);
				}
			}
			else {
				return tuple_accumulate(
					std::forward<Tuple>(t),
					f(std::forward<Acc>(acc), std::get<IST_Ns::head>(std::forward<Tuple>(t))),
					std::forward<Func>(f),
					std::integer_sequence<bool>{},
					IST_Ns::tail
				);
			}
		}
		else
			return std::forward<Acc>(acc);
	}
}

namespace Ubpa::USTL {
	template<bool... Masks, typename Tuple, typename Init, typename Func>
	constexpr auto tuple_accumulate(Tuple&& t, Init&& i, Func&& f) {
		constexpr size_t N = std::tuple_size_v<Tuple>;
		static_assert(sizeof...(Masks) <= N);
		return detail::tuple_accumulate(
			std::forward<Tuple>(t),
			std::forward<Init>(i),
			std::forward<Func>(f),
			std::integer_sequence<bool, Masks...>{},
			std::make_index_sequence<N>{}
		);
	}
}
