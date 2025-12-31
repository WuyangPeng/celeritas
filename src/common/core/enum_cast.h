#pragma once

#include <type_traits>

namespace celeritas
{
    // 枚举和整型之间的转换

    template <typename E>
    [[nodiscard]] constexpr auto enum_cast_underlying(E enumerator) noexcept requires(std::is_enum_v<E>)
    {
        using underlying_type = std::underlying_type_t<E>;

        return static_cast<underlying_type>(enumerator);
    }

    template <typename T, typename E>
    [[nodiscard]] constexpr T enum_cast_underlying(E enumerator) noexcept requires(std::is_enum_v<E>)
    {
        return static_cast<T>(enumerator);
    }

    template <typename E>
    [[nodiscard]] constexpr E underlying_cast_enum(std::underlying_type_t<E> value) noexcept requires(std::is_enum_v<E>)
    {
        return static_cast<E>(value);
    }

    template <typename E>
    void underlying_cast_enum_ptr(std::underlying_type_t<E> value, E* ptr) noexcept requires(std::is_enum_v<E>)
    {
        // 指针允许为空。
        if (ptr != nullptr)
        {
            *ptr = underlying_cast_enum<E>(value);
        }
    }
}
