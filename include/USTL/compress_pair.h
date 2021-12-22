#pragma once

namespace Ubpa::USTL {
	// CLASS TEMPLATE compress_pair
	struct zero_then_variadic_args_t {
		explicit zero_then_variadic_args_t() = default;
	}; // tag type for value-initializing first, constructing second from remaining args

	struct one_then_variadic_args_t {
		explicit one_then_variadic_args_t() = default;
	}; // tag type for constructing first from one arg, constructing second from remaining args
}

#include "details/compress_pair.inl"

namespace Ubpa::USTL {
	// if First is empty and not final, compress it by derived from it
	// [API]
	// constexpr compress_pair(zero_then_variadic_args_t, Args...)
	// constexpr compress_pair(one_then_variadic_args_t, Arg0, Args...)
	// constexpr static compress_pair zero_then_variadic_args(Args...)
	// static constexpr compress_pair one_then_variadic_args(Arg0, Args...)
	// constexpr [const] First&  get_first() [const]
	// constexpr [const] Second& get_second() [const]
	template<typename First, typename Second>
	using compress_pair = details::compress_pair<First, Second>;
}
