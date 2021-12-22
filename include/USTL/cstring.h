#pragma once

#include <array>
#include <string_view>
#include <cassert>

namespace Ubpa::USTL {
	template <std::size_t N>
	class cstring {
		static_assert(N > 0, "cstring requires size greater than 0.");

		std::array<char, N + 1> chars_;

		template <std::size_t... I>
		constexpr cstring(std::string_view str, std::index_sequence<I...>) noexcept : chars_{ {str[I]..., '\0'} } {}

		template <std::size_t... N1, std::size_t... N2>
		constexpr cstring(
			std::string_view a, std::index_sequence<N1...>,
			std::string_view b, std::index_sequence<N2...>
		) noexcept : chars_{ {a[N1]..., b[N2]..., '\0'} } {}

	public:
		using value_type = const char;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using pointer = const char*;
		using const_pointer = const char*;
		using reference = const char&;
		using const_reference = const char&;

		using iterator = const char*;
		using const_iterator = const char*;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		constexpr explicit cstring(char c) noexcept : chars_{ {c,'\0'} } {
			static_assert(N == 1);
		}

		constexpr explicit cstring(std::string_view str) noexcept : cstring{ str, std::make_index_sequence<N>{} } {
			assert(str.size() == N);
		}

		template<size_t N1, size_t N2>
		constexpr explicit cstring(cstring<N1> a, cstring<N2> b) noexcept
			: cstring{ a, std::make_index_sequence<N1>{}, b, std::make_index_sequence<N2>{} }
		{
			static_assert(N1 + N2 == N);
		}

		constexpr cstring() = delete;

		constexpr cstring(const cstring&) = default;

		constexpr cstring(cstring&&) = default;

		~cstring() = default;

		cstring& operator=(const cstring&) = default;

		cstring& operator=(cstring&&) = default;

		constexpr const_pointer data() const noexcept { return chars_.data(); }

		constexpr size_type size() const noexcept { return N; }

		constexpr const_iterator begin() const noexcept { return data(); }

		constexpr const_iterator end() const noexcept { return data() + size(); }

		constexpr const_iterator cbegin() const noexcept { return begin(); }

		constexpr const_iterator cend() const noexcept { return end(); }

		constexpr const_reverse_iterator rbegin() const noexcept { return end(); }

		constexpr const_reverse_iterator rend() const noexcept { return begin(); }

		constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }

		constexpr const_reverse_iterator crend() const noexcept { return rend(); }

		constexpr const_reference operator[](size_type i) const noexcept { return assert(i < size()), chars_[i]; }

		constexpr const_reference at(size_type i) const { return assert(i < size()), chars_.at(i); }

		constexpr const_reference front() const noexcept { return chars_[0]; }

		constexpr const_reference back() const noexcept { return chars_[N]; }

		constexpr size_type length() const noexcept { return size(); }

		constexpr bool empty() const noexcept { return false; }

		constexpr int compare(std::string_view str) const noexcept {
			return std::string_view{ data(), size() }.compare(str);
		}

		constexpr const char* c_str() const noexcept { return data(); }

		template <typename Char = char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>>
		std::basic_string<Char, Traits, Allocator> str() const { return { begin(), end() }; }

		constexpr operator std::string_view() const noexcept { return { data(), size() }; }

		constexpr explicit operator const char* () const noexcept { return data(); }

		template <typename Char = char, typename Traits = std::char_traits<Char>, typename Allocator = std::allocator<Char>>
		explicit operator std::basic_string<Char, Traits, Allocator>() const { return { begin(), end() }; }
	};

	cstring(char)->cstring<1>;
	template<size_t N>
	cstring(const char(&)[N])->cstring<N - 1>;
	template<size_t N1, size_t N2>
	cstring(const char(&)[N1], const char(&)[N2])->cstring<N1 + N2 - 2>;
	template<size_t N1, size_t N2>
	cstring(cstring<N1>, cstring<N2>)->cstring<N1 + N2>;

	template <std::size_t N>
	constexpr bool operator==(const cstring<N>& lhs, std::string_view rhs) noexcept {
		return lhs.compare(rhs) == 0;
	}

	template <std::size_t N>
	constexpr bool operator==(std::string_view lhs, const cstring<N>& rhs) noexcept {
		return lhs.compare(rhs) == 0;
	}

	template <std::size_t N>
	constexpr bool operator!=(const cstring<N>& lhs, std::string_view rhs) noexcept {
		return lhs.compare(rhs) != 0;
	}

	template <std::size_t N>
	constexpr bool operator!=(std::string_view lhs, const cstring<N>& rhs) noexcept {
		return lhs.compare(rhs) != 0;
	}

	template <std::size_t N>
	constexpr bool operator>(const cstring<N>& lhs, std::string_view rhs) noexcept {
		return lhs.compare(rhs) > 0;
	}

	template <std::size_t N>
	constexpr bool operator>(std::string_view lhs, const cstring<N>& rhs) noexcept {
		return lhs.compare(rhs) > 0;
	}

	template <std::size_t N>
	constexpr bool operator>=(const cstring<N>& lhs, std::string_view rhs) noexcept {
		return lhs.compare(rhs) >= 0;
	}

	template <std::size_t N>
	constexpr bool operator>=(std::string_view lhs, const cstring<N>& rhs) noexcept {
		return lhs.compare(rhs) >= 0;
	}

	template <std::size_t N>
	constexpr bool operator<(const cstring<N>& lhs, std::string_view rhs) noexcept {
		return lhs.compare(rhs) < 0;
	}

	template <std::size_t N>
	constexpr bool operator<(std::string_view lhs, const cstring<N>& rhs) noexcept {
		return lhs.compare(rhs) < 0;
	}

	template <std::size_t N>
	constexpr bool operator<=(const cstring<N>& lhs, std::string_view rhs) noexcept {
		return lhs.compare(rhs) <= 0;
	}

	template <std::size_t N>
	constexpr bool operator<=(std::string_view lhs, const cstring<N>& rhs) noexcept {
		return lhs.compare(rhs) <= 0;
	}

	template <typename Char, typename Traits, std::size_t N>
	std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, const cstring<N>& str) {
		for (const auto c : std::string_view{ str })
			os.put(c);

		return os;
	}
}

namespace std {
	template<size_t Index, size_t N>
	constexpr char get(Ubpa::USTL::cstring<N> cstr) noexcept {
		static_assert(Index < N);
		return cstr[Index];
	}
}

#include "details/cstring.inl"

namespace Ubpa::USTL
{
	// 2-10, 16
	template<auto Num, size_t System = 10>
	constexpr auto cstring_integer = details::cstring_integer_<Num, System>();
}
