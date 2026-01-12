#pragma once

#include "redis_to_basis_converter.h"
#include "database/basic/database_field.h"

#include <boost/json.hpp>

template <typename ArrayType>
celeritas::basis_database celeritas::redis_to_basis_converter::to_numeric_array_basis(const database_field& field_name, const std::string& value)
{
    if (value.empty())
    {
        return basis_database{ field_name.get_field_name(), ArrayType{} };
    }

    const auto json = boost::json::parse(value);
    const auto result = boost::json::value_to<ArrayType>(json);

    return basis_database{ field_name.get_field_name(), result };
}
