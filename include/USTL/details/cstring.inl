#pragma once

namespace Ubpa::USTL::details {
	template<auto Num, size_t System>
	constexpr auto cstring_integer_in_system_between_3_to_10() noexcept {
		using T = decltype(Num);
		static_assert(std::is_integral_v<T>);
		if constexpr (std::is_unsigned_v<T>) {
			if constexpr (Num < System)
				return cstring{ '0' + Num };
			else
				return cstring{ cstring_integer_in_system_between_3_to_10<Num / System, System>(), cstring{ '0' + Num % System } };
		}
		else
		{
			if constexpr (Num < 0) {
				return cstring{ cstring{'-'}, cstring_integer_in_system_between_3_to_10<-Num, System>() };
			}
			else {
				if constexpr (Num < System)
					return cstring{ '0' + Num };
				else
					return cstring{ cstring_integer_in_system_between_3_to_10<Num / System, System>(), cstring{ '0' + Num % System } };
			}
		}
	}

	template<auto Num>
	constexpr auto cstring_integer_in_decimal() noexcept {
		return cstring_integer_in_system_between_3_to_10<Num, 10>();
	}

	template<auto Num>
	constexpr auto cstring_integer_in_octal() noexcept {
		return cstring_integer_in_system_between_3_to_10<Num, 8>();
	}

	template<auto Num>
	constexpr auto cstring_integer_in_binary() noexcept {
		using T = decltype(Num);
		static_assert(std::is_integral_v<T>);
		if constexpr (std::is_unsigned_v<T>) {
			if constexpr (Num < 2)
				return cstring{ '0' + Num };
			else
				return cstring{ cstring_integer_in_binary<(Num >> 1)>(), cstring{ '0' + (Num & 1) } };
		}
		else
		{
			if constexpr (Num < 0) {
				return cstring{ cstring{'-'}, cstring_integer_in_binary<-Num>() };
			}
			else {
				if constexpr (Num < 2)
					return cstring{ '0' + Num };
				else
					return cstring{ cstring_integer_in_binary<(Num >> 1)>(), cstring{ '0' + (Num & 1) } };
			}
		}
	}

	template<auto Num>
	constexpr auto cstring_integer_in_hex() noexcept {
		using T = decltype(Num);
		static_assert(std::is_integral_v<T>);
		constexpr auto decimal2hex = [](auto num) {
			if (num < 10)
				return '0' + num;
			else
				return 'A' + num - 10;
		};
		if constexpr (std::is_unsigned_v<T>) {
			if constexpr (Num < 16)
				return cstring{ decimal2hex(Num) };
			else
				return cstring{ cstring_integer_in_hex<(Num >> 4)>(), cstring{ decimal2hex(Num & 0xF) } };
		}
		else
		{
			if constexpr (Num < 0) {
				return cstring{ cstring{'-'}, cstring_integer_in_hex<-Num>() };
			}
			else {
				if constexpr (Num < 16)
					return cstring{ decimal2hex(Num) };
				else
					return cstring{ cstring_integer_in_hex<(Num >> 4)>(), cstring{ decimal2hex(Num & 0xF) } };
			}
		}
	}

	template<auto Num, size_t System>
	constexpr auto cstring_integer_() noexcept {
		if constexpr (System == 2)
			return cstring_integer_in_binary<Num>();
		else if constexpr (3 <= System && System <= 10)
			return cstring_integer_in_system_between_3_to_10<Num, System>();
		else if constexpr (System == 16)
			return cstring_integer_in_hex<Num>();
		else
			static_assert(false);
	}
}
