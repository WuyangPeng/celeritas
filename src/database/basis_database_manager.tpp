#pragma once

#include "basis_database_manager.h"

template <celeritas::database_data_type Type>
celeritas::database_data_Type_traits<Type>::Type celeritas::basis_database_manager::get_value(const std::string_view field_name, typename boost::call_traits<typename database_data_Type_traits<Type>::Type>::param_type default_value) const
{
    if (const auto result = get_any_value(field_name);
        result.has_value())
    {
        return std::any_cast<typename database_data_Type_traits<Type>::Type>(result);
    }

    return default_value;
}