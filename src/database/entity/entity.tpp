#pragma once

#include "database/basic/database_field.h"
#include "database/entity/entity.h"

template <const std::string_view& FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
celeritas::entity<FieldName, Type, Index>::entity(typename boost::call_traits<entity_type>::param_type entity) noexcept(std::is_nothrow_copy_constructible_v<entity_type>)
    : entity_{ entity }
{
}

template <const std::string_view& FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
celeritas::database_field celeritas::entity<FieldName, Type, Index>::get_database_field() noexcept
{
    return database_field{ FieldName, Type, Index };
}

template <const std::string_view& FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
celeritas::entity<FieldName, Type, Index>::entity_type celeritas::entity<FieldName, Type, Index>::get_value() const noexcept(std::is_nothrow_copy_constructible_v<entity_type>)
{
    return entity_;
}

template <const std::string_view& FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
void celeritas::entity<FieldName, Type, Index>::set_value(typename boost::call_traits<entity_type>::param_type entity) noexcept(std::is_nothrow_copy_assignable_v<entity_type>)
{
    entity_ = entity;
}

template <const std::string_view& FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
void celeritas::entity<FieldName, Type, Index>::modify_value(typename boost::call_traits<entity_type>::param_type entity) noexcept(noexcept(std::declval<entity_type&>() += std::declval<typename boost::call_traits<entity_type>::param_type>()))
{
    entity_ += entity;
}

template <const std::string_view& FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
bool celeritas::entity<FieldName, Type, Index>::set_value(int index, typename boost::call_traits<element_type>::param_type entity)
{
    auto& element = entity_.at(index);
    if (element != entity)
    {
        element = entity;
        return true;
    }

    return false;
}

template <const std::string_view& FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
void celeritas::entity<FieldName, Type, Index>::add_value(typename boost::call_traits<element_type>::param_type entity)
{
    entity_.emplace_back(entity);
}

template <const std::string_view& FieldName, celeritas::database_data_type Type, celeritas::database_index_type Index>
void celeritas::entity<FieldName, Type, Index>::remove_value(int index)
{
    if (0 <= index && index < entity_.size())
    {
        entity_.erase(entity_.begin() + index);
    }
}
