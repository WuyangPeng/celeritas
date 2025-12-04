#pragma once

#include "database_data_type.h"
#include "database_fwd.h"

#include <boost/call_traits.hpp>

namespace celeritas
{
    template <database_data_type Type>
    struct database_data_Type_traits;

    template <>
    struct database_data_Type_traits<database_data_type::null_type>
    {
        using Type = decltype(nullptr);
    };

    template <>
    struct database_data_Type_traits<database_data_type::string_type>
    {
        using Type = std::string;
    };

    template <>
    struct database_data_Type_traits<database_data_type::string_array_type>
    {
        using Type = std::vector<std::string>;
    };

    template <>
    struct database_data_Type_traits<database_data_type::int32_type>
    {
        using Type = int32_t;
    };

    template <>
    struct database_data_Type_traits<database_data_type::int32_count_type>
    {
        using Type = int32_t;
    };

    template <>
    struct database_data_Type_traits<database_data_type::int32_array_type>
    {
        using Type = std::vector<int32_t>;
    };

    template <>
    struct database_data_Type_traits<database_data_type::int64_type>
    {
        using Type = int64_t;
    };

    template <>
    struct database_data_Type_traits<database_data_type::int64_count_type>
    {
        using Type = int64_t;
    };

    template <>
    struct database_data_Type_traits<database_data_type::int64_array_type>
    {
        using Type = std::vector<int64_t>;
    };

    template <>
    struct database_data_Type_traits<database_data_type::double_type>
    {
        using Type = double;
    };

    template <>
    struct database_data_Type_traits<database_data_type::double_array_type>
    {
        using Type = std::vector<double>;
    };

    template <>
    struct database_data_Type_traits<database_data_type::bool_type>
    {
        using Type = bool;
    };

    template <>
    struct database_data_Type_traits<database_data_type::byte_array_type>
    {
        using Type = std::vector<uint8_t>;
    };

    template <>
    struct database_data_Type_traits<database_data_type::document_type>
    {
        using Type = std::string;
    };

    template <>
    struct database_data_Type_traits<database_data_type::document_array_type>
    {
        using Type = std::vector<std::string>;
    };

    namespace traits
    {
        using string_type = database_data_Type_traits<database_data_type::string_type>::Type;
        using string_array_type = database_data_Type_traits<database_data_type::string_array_type>::Type;
        using int32_type = database_data_Type_traits<database_data_type::int32_type>::Type;
        using int32_count_type = database_data_Type_traits<database_data_type::int32_count_type>::Type;
        using int32_array_type = database_data_Type_traits<database_data_type::int32_array_type>::Type;
        using int64_type = database_data_Type_traits<database_data_type::int64_type>::Type;
        using int64_count_type = database_data_Type_traits<database_data_type::int64_count_type>::Type;
        using int64_array_type = database_data_Type_traits<database_data_type::int64_array_type>::Type;
        using double_type = database_data_Type_traits<database_data_type::double_type>::Type;
        using double_array_type = database_data_Type_traits<database_data_type::double_array_type>::Type;
        using bool_type = database_data_Type_traits<database_data_type::bool_type>::Type;
        using byte_array_type = database_data_Type_traits<database_data_type::byte_array_type>::Type;
        using document_type = database_data_Type_traits<database_data_type::document_type>::Type;
        using document_array_type = database_data_Type_traits<database_data_type::document_array_type>::Type;

        namespace param_type
        {
            using string_type = boost::call_traits<string_type>::param_type;
            using string_array_type = boost::call_traits<string_array_type>::param_type;
            using int32_type = boost::call_traits<int32_type>::param_type;
            using int32_count_type = boost::call_traits<int32_count_type>::param_type;
            using int32_array_type = boost::call_traits<int32_array_type>::param_type;
            using int64_type = boost::call_traits<int64_type>::param_type;
            using int64_count_type = boost::call_traits<int64_count_type>::param_type;
            using int64_array_type = boost::call_traits<int64_array_type>::param_type;
            using double_type = boost::call_traits<double_type>::param_type;
            using double_array_type = boost::call_traits<double_array_type>::param_type;
            using bool_type = boost::call_traits<bool_type>::param_type;
            using byte_array_type = boost::call_traits<byte_array_type>::param_type;
            using document_type = boost::call_traits<document_type>::param_type;
            using document_array_type = boost::call_traits<document_array_type>::param_type;
        }
    }
}