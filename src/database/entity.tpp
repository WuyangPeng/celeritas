#pragma once

#include "database_field.h"
#include "entity.h"

template <const std::string_view& FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
celeritas::entity<FieldName, Type, Index>::entity(typename boost::call_traits<entity_type>::param_type entity) noexcept(std::is_arithmetic_v<entity_type>)
    : entity_{ entity }
{
}

template <const std::string_view&FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
celeritas::database_field celeritas::entity<FieldName, Type, Index>::get_database_field() noexcept
{
    return database_field{ FieldName, Type, Index };
}

template <const std::string_view&FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
celeritas::entity<FieldName, Type, Index>::entity_type celeritas::entity<FieldName, Type, Index>::get_value() const noexcept(std::is_arithmetic_v<entity_type>)
{
    return entity_;
}

template <const std::string_view&FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
void celeritas::entity<FieldName, Type, Index>::set_value(typename boost::call_traits<entity_type>::param_type entity) noexcept(std::is_arithmetic_v<entity_type>)
{
    entity_ = entity;
}

template <const std::string_view&FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
void celeritas::entity<FieldName, Type, Index>::modify_value(typename boost::call_traits<entity_type>::param_type entity) noexcept(std::is_arithmetic_v<entity_type>)
{
    entity_ += entity;
}