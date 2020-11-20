#pragma once

#include <type_traits>

namespace Ubpa::USTL::details {
	template<typename First, typename Second, bool = std::is_empty_v<First> && !std::is_final_v<First>>
	class compress_pair final : private First { // store a pair of values, deriving from empty first
    public:
        constexpr compress_pair() noexcept(
            std::conjunction_v<std::is_nothrow_default_constructible<First>, std::is_nothrow_default_constructible<Second>>)
            : First{}, second{} {}

        template <typename... Args>
        constexpr compress_pair(zero_then_variadic_args_t, Args&&... args) noexcept (
            std::conjunction_v<std::is_nothrow_default_constructible<First>, std::is_nothrow_constructible<Second, Args...>>)
            : First{}, second{ std::forward<Args>(args)... } {}

        template <typename Arg0, typename... Args>
        constexpr compress_pair(one_then_variadic_args_t, Arg0&& arg0, Args&&... args) noexcept (
            std::conjunction_v<std::is_nothrow_constructible<First, Arg0>, std::is_nothrow_constructible<Second, Args...>>)
            : First{ std::forward<Arg0>(arg0) }, second{ std::forward<Args>(args)... } {}

        template <typename... Args>
        static constexpr compress_pair zero_then_variadic_args(Args&&... args) noexcept (
            std::is_nothrow_constructible_v<compress_pair, zero_then_variadic_args_t, Args...>)
        {
            return { zero_then_variadic_args_t{}, std::forward<Args>(args)... };
        }

        template <typename Arg0, typename... Args>
        static constexpr compress_pair one_then_variadic_args(Arg0&& arg0, Args&&... args) noexcept (
            std::is_nothrow_constructible_v<compress_pair, one_then_variadic_args_t, Arg0, Args...>)
        {
            return { one_then_variadic_args_t{}, std::forward<Arg0>(arg0), std::forward<Args>(args)... };
        }

        constexpr First& get_first() noexcept { return *this; }
        constexpr const First& get_first() const noexcept { return *this; }
        constexpr Second& get_second() noexcept { return second; }
        constexpr const Second& get_second() const noexcept { return second; }

    private:
        Second second;
    };

    template <class First, class Second>
    class compress_pair<First, Second, false> final { // store a pair of values, not deriving from first
    public:
        constexpr compress_pair() noexcept(
            std::conjunction_v<std::is_nothrow_default_constructible<First>, std::is_nothrow_default_constructible<Second>>)
            : first{}, second{} {}

        template <class... Args>
        constexpr explicit compress_pair(zero_then_variadic_args_t, Args&&... args) noexcept (
            std::conjunction_v<std::is_nothrow_default_constructible<First>, std::is_nothrow_constructible<Second, Args...>>)
            : first{}, second{ std::forward<Args>(args)... } {}

        template <class Arg0, class... Args>
        constexpr compress_pair(one_then_variadic_args_t, Arg0&& arg0, Args&&... args) noexcept (
            std::conjunction_v<std::is_nothrow_constructible<First, Arg0>, std::is_nothrow_constructible<Second, Args...>>)
            : first{ std::forward<Arg0>(arg0) }, second{ std::forward<Args>(args)... } {}

        constexpr First& get_first() noexcept { return first; }
        constexpr const First& get_first() const noexcept { return first; }
        constexpr Second& get_second() noexcept { return second; }
        constexpr const Second& get_second() const noexcept { return second; }

    private:
        First first;
        Second second;
    };
}
