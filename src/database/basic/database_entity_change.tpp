#pragma once

#include "database_entity_change.h"
#include "common/logging/logger.h"

#include <boost/core/demangle.hpp>

#include <variant>

template <celeritas::database_data_type Type>
celeritas::database_data_type_traits<Type>::type celeritas::database_entity_change::get_value(const std::string_view field_name, typename boost::call_traits<typename database_data_type_traits<Type>::type>::param_type default_value) const
{
    const auto& result = get_variant_value(field_name);

    try
    {
        return do_get_value<Type>(result, default_value);
    }
    catch (const std::bad_variant_access& error)
    {
        using target_type = database_data_type_traits<Type>::type;

        LOG_CHANNEL(database_channel, error)
        << "database entity change get value error, field name: "
        << field_name
        << ", actual type index: "
        << result.index()
        << ", expected type: "
        << boost::core::demangle(typeid(target_type).name())
        << ", error: "
        << error.what();

        throw;
    }
}

template <celeritas::database_data_type Type>
celeritas::database_data_type_traits<Type>::type celeritas::database_entity_change::do_get_value(const value_variant& variant, typename boost::call_traits<typename database_data_type_traits<Type>::type>::param_type default_value) const
{
    using target_type = database_data_type_traits<Type>::type;

    if (variant.index() == 0)
    {
        return default_value;
    }

    return std::get<target_type>(variant);
}
