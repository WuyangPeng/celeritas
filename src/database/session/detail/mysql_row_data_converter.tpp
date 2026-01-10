#pragma once

#include "mysql_row_data_converter.h"
#include "common/core/celeritas_error.h"
#include "database/basic/database_field.h"

#include <boost/json.hpp>

#include <ranges>

template <typename ArrayType>
celeritas::basis_database celeritas::mysql_row_data_converter::to_numeric_array_basis(const database_field& field_name, const field_view_type& row_view)
{
    const std::string value{ row_view.as_string() };
    if (value.empty())
    {
        return basis_database{ field_name.get_field_name(), ArrayType{} };
    }

    const auto json = boost::json::parse(value);
    const auto result = boost::json::value_to<ArrayType>(json);

    return basis_database{ field_name.get_field_name(), result };
}
