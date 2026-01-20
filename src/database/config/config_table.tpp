#pragma once

#include "config_table.h"
#include "config/basic/database_type.h"

template <typename T>
celeritas::config_table<T>::config_table(const std::string_view name)
    : name_{ name }, container_{}
{
}

template <typename T>
std::string_view celeritas::config_table<T>::get_name() const
{
    return name_;
}

template <typename T>
celeritas::config_table_base::void_awaitable_type celeritas::config_table<T>::load_all(const database_pool_shared_ptr& pool)
{
    const auto result = co_await pool->select_all(entity_type::get_select(database_type::mysql), entity_type::get_database_field_container());

    container_type container{};
    for (const auto& row : result)
    {
        const entity_type item{ row };
        container.emplace(item.get_id(), std::make_shared<const entity_type>(item));
    }

    container_ = std::move(container);
}

template <typename T>
celeritas::config_table_base::void_awaitable_type celeritas::config_table<T>::load_one(const database_pool_shared_ptr& pool, const int64_t id)
{
    if (id == 0)
    {
        co_return co_await load_all(pool);
    }

    if (const auto optional_item = co_await pool->select_one(entity_type::get_select(database_type::mysql, id), entity_type::get_database_field_container()))
    {
        const entity_type item{ *optional_item };

        container_.insert_or_assign(item.get_id(), std::make_shared<const entity_type>(item));
    }
}

template <typename T>
celeritas::config_table<T>::optional_const_entity_shared_ptr celeritas::config_table<T>::get_item(const int64_t id)
{
    if (const auto iter = container_.find(id);
        iter != container_.cend())
    {
        return iter->second;
    }

    return std::nullopt;
}

