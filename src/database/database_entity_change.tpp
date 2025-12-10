#pragma once

#include "database_entity_change.h"
#include "common/logger.h"

template <celeritas::database_data_type Type>
celeritas::database_data_Type_traits<Type>::Type celeritas::database_entity_change::get_value(const std::string_view field_name, typename boost::call_traits<typename database_data_Type_traits<Type>::Type>::param_type default_value) const
{
    const auto& result = get_any_value(field_name);

    try
    {
        if (result.has_value())
        {
            return std::any_cast<typename database_data_Type_traits<Type>::Type>(result);
        }
    }
    catch (const std::bad_any_cast& error)
    {
        LOG_CHANNEL(database_channel, error)
        << "database entity change get value error, field_name: "
        << field_name
        << ", actual type: "
        << result.type().name()
        << ", expected type: "
        << typeid(database_data_Type_traits<Type>::Type).name()
        << ", what: "
        << error.what();

        throw;
    }

    return default_value;
}