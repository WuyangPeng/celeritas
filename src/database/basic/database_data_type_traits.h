#pragma once

#include "database/database_fwd.h"
#include "database_data_type.h"

#include <boost/call_traits.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace celeritas
{
    template <database_data_type Type>
    struct database_data_type_traits;

    template <>
    struct database_data_type_traits<database_data_type::null_type>
    {
        using type = decltype(nullptr);
        using element_type = type;
    };

    template <>
    struct database_data_type_traits<database_data_type::string_type>
    {
        using type = std::string;
        using element_type = type;
    };

    template <>
    struct database_data_type_traits<database_data_type::string_array_type>
    {
        using type = std::vector<std::string>;
        using element_type = type::value_type;
    };

    template <>
    struct database_data_type_traits<database_data_type::int32_type>
    {
        using type = int32_t;
        using element_type = type;
    };

    template <>
    struct database_data_type_traits<database_data_type::int32_count_type>
    {
        using type = int32_t;
        using element_type = type;
    };

    template <>
    struct database_data_type_traits<database_data_type::int32_array_type>
    {
        using type = std::vector<int32_t>;
        using element_type = type::value_type;
    };

    template <>
    struct database_data_type_traits<database_data_type::int64_type>
    {
        using type = int64_t;
        using element_type = type;
    };

    template <>
    struct database_data_type_traits<database_data_type::int64_count_type>
    {
        using type = int64_t;
        using element_type = type;
    };

    template <>
    struct database_data_type_traits<database_data_type::int64_array_type>
    {
        using type = std::vector<int64_t>;
        using element_type = type::value_type;
    };

    template <>
    struct database_data_type_traits<database_data_type::double_type>
    {
        using type = double;
        using element_type = type;
    };

    template <>
    struct database_data_type_traits<database_data_type::double_array_type>
    {
        using type = std::vector<double>;
        using element_type = type::value_type;
    };

    template <>
    struct database_data_type_traits<database_data_type::bool_type>
    {
        using type = bool;
        using element_type = type;
    };

    template <>
    struct database_data_type_traits<database_data_type::byte_array_type>
    {
        using type = std::vector<uint8_t>;
        using element_type = type::value_type;
    };

    template <>
    struct database_data_type_traits<database_data_type::document_type>
    {
        using type = std::vector<basis_database>;
        using element_type = type;
    };

    template <>
    struct database_data_type_traits<database_data_type::document_array_type>
    {
        using type = std::vector<std::vector<basis_database> >;
        using element_type = type::value_type;
    };

    namespace traits
    {
        using string_type = database_data_type_traits<database_data_type::string_type>::type;
        using string_array_type = database_data_type_traits<database_data_type::string_array_type>::type;
        using string_array_element_type = database_data_type_traits<database_data_type::string_array_type>::element_type;
        using int32_type = database_data_type_traits<database_data_type::int32_type>::type;
        using int32_count_type = database_data_type_traits<database_data_type::int32_count_type>::type;
        using int32_array_type = database_data_type_traits<database_data_type::int32_array_type>::type;
        using int32_array_element_type = database_data_type_traits<database_data_type::int32_array_type>::element_type;
        using int64_type = database_data_type_traits<database_data_type::int64_type>::type;
        using int64_count_type = database_data_type_traits<database_data_type::int64_count_type>::type;
        using int64_array_type = database_data_type_traits<database_data_type::int64_array_type>::type;
        using int64_array_element_type = database_data_type_traits<database_data_type::int64_array_type>::element_type;
        using double_type = database_data_type_traits<database_data_type::double_type>::type;
        using double_array_type = database_data_type_traits<database_data_type::double_array_type>::type;
        using double_array_element_type = database_data_type_traits<database_data_type::double_array_type>::element_type;
        using bool_type = database_data_type_traits<database_data_type::bool_type>::type;
        using byte_array_type = database_data_type_traits<database_data_type::byte_array_type>::type;
        using byte_array_element_type = database_data_type_traits<database_data_type::byte_array_type>::element_type;
        using document_type = database_data_type_traits<database_data_type::document_type>::type;
        using document_array_type = database_data_type_traits<database_data_type::document_array_type>::type;
        using document_array_element_type = database_data_type_traits<database_data_type::document_array_type>::element_type;

        namespace param_type
        {
            using string_type = boost::call_traits<string_type>::param_type;
            using string_array_type = boost::call_traits<string_array_type>::param_type;
            using string_array_element_type = boost::call_traits<string_array_element_type>::param_type;
            using int32_type = boost::call_traits<int32_type>::param_type;
            using int32_count_type = boost::call_traits<int32_count_type>::param_type;
            using int32_array_type = boost::call_traits<int32_array_type>::param_type;
            using int32_array_element_type = boost::call_traits<int32_array_element_type>::param_type;
            using int64_type = boost::call_traits<int64_type>::param_type;
            using int64_count_type = boost::call_traits<int64_count_type>::param_type;
            using int64_array_type = boost::call_traits<int64_array_type>::param_type;
            using int64_array_element_type = boost::call_traits<int64_array_element_type>::param_type;
            using double_type = boost::call_traits<double_type>::param_type;
            using double_array_type = boost::call_traits<double_array_type>::param_type;
            using double_array_element_type = boost::call_traits<double_array_element_type>::param_type;
            using bool_type = boost::call_traits<bool_type>::param_type;
            using byte_array_type = boost::call_traits<byte_array_type>::param_type;
            using byte_array_element_type = boost::call_traits<byte_array_element_type>::param_type;
            using document_type = boost::call_traits<document_type>::param_type;
            using document_array_type = boost::call_traits<document_array_type>::param_type;
            using document_array_element_type = boost::call_traits<document_array_element_type>::param_type;
        }
    }
}