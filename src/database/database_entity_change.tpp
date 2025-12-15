#pragma once

#include "database_entity_change.h"
#include "common/logger.h"

#include <boost/core/demangle.hpp>

template <celeritas::database_data_type Type>
celeritas::database_data_Type_traits<Type>::type celeritas::database_entity_change::get_value(const std::string_view field_name, typename boost::call_traits<typename database_data_Type_traits<Type>::type>::param_type default_value) const
{
    const auto& result = get_any_value(field_name);

    try
    {
        if (result.has_value())
        {
            return std::any_cast<typename database_data_Type_traits<Type>::type>(result);
        }
    }
    catch (const std::bad_any_cast& error)
    {
        LOG_CHANNEL(database_channel, error)
        << "database entity change get value error, field_name: "
        << field_name
        << ", actual type: "
        << boost::core::demangle(result.type().name())
        << ", expected type: "
        << boost::core::demangle(typeid(typename database_data_Type_traits<Type>::type).name())
        << ", what: "
        << error.what();

        throw;
    }

    return default_value;
}