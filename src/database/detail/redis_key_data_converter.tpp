#pragma once

#include "redis_key_data_converter.h"
#include "database/basic/database_field.h"

#include <boost/lexical_cast.hpp>

#include <ranges>

template <typename ArrayType>
celeritas::basis_database celeritas::redis_key_data_converter::to_numeric_array_basis(const database_field& field_name, const std::string& value)
{
    auto split_view = value | std::views::split('|');

    auto int_view = split_view | std::views::transform([](const auto& subrange) {
        const std::string result{ subrange.begin(), subrange.end() };
        return boost::lexical_cast<typename ArrayType::value_type>(result);
    });
    const basis_database::int32_array result{ int_view.begin(), int_view.end() };

    return basis_database{ field_name.get_field_name(), result };
}