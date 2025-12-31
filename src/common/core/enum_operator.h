#pragma once

#include "enum_cast.h"

#include <iosfwd>
#include <type_traits>

namespace celeritas
{
    template <typename E>
    [[nodiscard]] auto& enum_reinterpret_cast(E& enumerator) noexcept requires(std::is_enum_v<E>)
    {
        using underlying_type = std::underlying_type_t<E>;

        return reinterpret_cast<underlying_type&>(enumerator);
    }

    template <typename E>
    [[nodiscard]] constexpr E operator+(E lhs, std::underlying_type_t<E> rhs) noexcept requires(std::is_enum_v<E>)
    {
        return underlying_cast_enum<E>(enum_cast_underlying(lhs) + rhs);
    }

    template <typename E>
    [[nodiscard]] constexpr E operator+(std::underlying_type_t<E> lhs, E rhs) noexcept requires(std::is_enum_v<E>)
    {
        return rhs + lhs;
    }

    template <typename E>
    E& operator+=(E& lhs, std::underlying_type_t<E> rhs) noexcept requires(std::is_enum_v<E>)
    {
        lhs = lhs + rhs;

        return lhs;
    }

    template <typename E>
    [[nodiscard]] constexpr E operator-(E lhs, std::underlying_type_t<E> rhs) noexcept requires(std::is_enum_v<E>)
    {
        return underlying_cast_enum<E>(enum_cast_underlying(lhs) - rhs);
    }

    template <typename E>
    E& operator-=(E& lhs, std::underlying_type_t<E> rhs) noexcept requires(std::is_enum_v<E>)
    {
        lhs = lhs - rhs;

        return lhs;
    }

    template <typename E, typename T>
    [[nodiscard]] constexpr T operator*(E lhs, T rhs) noexcept requires(std::is_enum_v<E> && std::is_arithmetic_v<T>)
    {
        return enum_cast_underlying(lhs) * rhs;
    }

    template <typename E, typename T>
    [[nodiscard]] constexpr T operator*(T lhs, E rhs) noexcept requires(std::is_enum_v<E> && std::is_arithmetic_v<T>)
    {
        return rhs * lhs;
    }

    template <typename E>
    [[nodiscard]] constexpr E operator-(E lhs) noexcept requires(std::is_enum_v<E> && std::is_signed_v<std::underlying_type_t<E> >)
    {
        return underlying_cast_enum<E>(-enum_cast_underlying(lhs));
    }

    template <typename E>
    E& operator++(E& lhs) noexcept requires(std::is_enum_v<E>)
    {
        auto& result = enum_reinterpret_cast(lhs);

        ++result;

        return lhs;
    }

    template <typename E>
    E operator++(E& lhs, int) noexcept requires(std::is_enum_v<E>)
    {
        const auto result = lhs;
        ++lhs;

        return result;
    }

    template <typename E>
    E& operator--(E& lhs) noexcept requires(std::is_enum_v<E>)
    {
        auto& result = enum_reinterpret_cast(lhs);

        --result;

        return lhs;
    }

    template <typename E>
    E operator--(E& lhs, int) noexcept requires(std::is_enum_v<E>)
    {
        const auto result = lhs;
        --lhs;

        return result;
    }

    template <typename E>
    [[nodiscard]] constexpr E operator&(E lhs, E rhs) noexcept requires(std::is_enum_v<E>)
    {
        return underlying_cast_enum<E>(enum_cast_underlying(lhs) & enum_cast_underlying(rhs));
    }

    template <typename E>
    E& operator&=(E& lhs, E rhs) noexcept requires(std::is_enum_v<E>)
    {
        lhs = lhs & rhs;
        return lhs;
    }

    template <typename E>
    [[nodiscard]] constexpr E operator|(E lhs, E rhs) noexcept requires(std::is_enum_v<E>)
    {
        return underlying_cast_enum<E>(enum_cast_underlying(lhs) | enum_cast_underlying(rhs));
    }

    template <typename E>
    E& operator|=(E& lhs, E rhs) noexcept requires(std::is_enum_v<E>)
    {
        lhs = lhs | rhs;
        return lhs;
    }

    template <typename E>
    [[nodiscard]] constexpr E operator^(E lhs, E rhs) noexcept requires(std::is_enum_v<E>)
    {
        return underlying_cast_enum<E>(enum_cast_underlying(lhs) ^ enum_cast_underlying(rhs));
    }

    template <typename E>
    E& operator^=(E& lhs, E rhs) noexcept requires(std::is_enum_v<E>)
    {
        lhs = lhs ^ rhs;
        return lhs;
    }

    // 使用下面函数需要包含头文件<iostream>。

    template <typename E>
    std::ostream& operator<<(std::ostream& os, E lhs) requires(std::is_enum_v<E>)
    {
        os << enum_cast_underlying(lhs);

        return os;
    }

    template <typename E>
    std::istream& operator>>(std::istream& is, E& lhs) requires(std::is_enum_v<E>)
    {
        auto& result = enum_reinterpret_cast(lhs);

        is >> result;

        return is;
    }

    template <typename E>
    std::wostream& operator<<(std::wostream& os, E lhs) requires(std::is_enum_v<E>)
    {
        os << enum_cast_underlying(lhs);

        return os;
    }

    template <typename E>
    std::wistream& operator>>(std::wistream& is, E& lhs) requires(std::is_enum_v<E>)
    {
        auto& result = enum_reinterpret_cast(lhs);

        is >> result;

        return is;
    }
}
