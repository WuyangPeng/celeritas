#pragma once

#include "database/basic/basis_database.h"

#include <boost/json.hpp>

#include <functional>
#include <map>
#include <string_view>

namespace celeritas
{
    class json_value_to_basis_converter
    {
    public:
        using class_type = json_value_to_basis_converter;
        using convert_function = std::function<basis_database(std::string_view, const boost::json::value&)>;
        using container_type = std::map<boost::json::kind, convert_function>;
        using json_value = boost::json::value;

        [[nodiscard]] static basis_database convert(std::string_view key, const json_value& value);

    private:
        [[nodiscard]] static const container_type& get_converters();

        [[nodiscard]] static basis_database convert_object(std::string_view key, const json_value& value);

        [[nodiscard]] static basis_database convert_array(std::string_view key, const json_value& value);

        [[nodiscard]] static basis_database convert_string(std::string_view key, const json_value& value);

        [[nodiscard]] static basis_database convert_int64(std::string_view key, const json_value& value);

        [[nodiscard]] static basis_database convert_uint64(std::string_view key, const json_value& value);

        [[nodiscard]] static basis_database convert_double(std::string_view key, const json_value& value);

        [[nodiscard]] static basis_database convert_bool(std::string_view key, const json_value& value);

        [[nodiscard]] static basis_database convert_null(std::string_view key, const json_value& value);
    };
}
