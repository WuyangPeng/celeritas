#pragma once

#include "provider_manager.h"
#include "common/core/celeritas_error.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"

template <typename EntityType, typename KeyType>
celeritas::provider_manager<EntityType, KeyType>::const_entity_shared_ptr celeritas::provider_manager<EntityType, KeyType>::get_entity(const KeyType& key)
{
    std::shared_lock lock{ mutex_ };

    if (const auto iter = entities_.find(key);
        iter != entities_.cend())
    {
        if (!is_entity_active(iter->second))
        {
            throw celeritas_error{ "{} is close. {}", get_entity_name(), format_key_info(key) };
        }
        return iter->second;
    }

    throw celeritas_error{ "{} not registered. {}", get_entity_name(), format_key_info(key) };
}

template <typename EntityType, typename KeyType>
void celeritas::provider_manager<EntityType, KeyType>::reload_from_db(const any_io_executor& any_io_executor, const int64_t id)
{
    if (id == 0)
    {
        load_from_db(any_io_executor);
        return;
    }

    safe_co_spawn(any_io_executor,
                  [self = this->shared_from_this(), id] {
                      return self->load_one_from_db(id);
                  },
                  auth_channel,
                  "load {} from db error, id = {}.",
                  get_entity_name(),
                  id);
}

template <typename EntityType, typename KeyType>
void celeritas::provider_manager<EntityType, KeyType>::load_from_db(const any_io_executor& any_io_executor)
{
    safe_co_spawn(any_io_executor,
                  [self = this->shared_from_this()] {
                      return self->load_all_from_db();
                  },
                  auth_channel,
                  "load {} from db error:",
                  get_entity_name());
}

template <typename EntityType, typename KeyType>
std::string_view celeritas::provider_manager<EntityType, KeyType>::get_database_name() const
{
    return mysql_auth_db_name;
}

template <typename EntityType, typename KeyType>
void celeritas::provider_manager<EntityType, KeyType>::on_entity_loaded(const const_entity_shared_ptr& entity)
{
}

template <typename EntityType, typename KeyType>
void celeritas::provider_manager<EntityType, KeyType>::before_add_entity(entity_container& container, const const_entity_shared_ptr& entity)
{
}

template <typename EntityType, typename KeyType>
celeritas::provider_manager<EntityType, KeyType>::void_awaitable_type celeritas::provider_manager<EntityType, KeyType>::load_all_from_db()
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(get_database_name().data());

    const auto result = co_await mysql_pool->template select_all<EntityType>(database_type::mysql);

    auto container = build_entity_container(result);

    std::lock_guard lock{ mutex_ };
    entities_ = std::move(container);
}

template <typename EntityType, typename KeyType>
celeritas::provider_manager<EntityType, KeyType>::void_awaitable_type celeritas::provider_manager<EntityType, KeyType>::load_one_from_db(const int64_t id)
{
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(get_database_name().data());

    if (const auto optional_entity = co_await mysql_pool->template select_one<EntityType>(database_type::mysql, id))
    {
        add_entity(optional_entity);
    }
    else
    {
        LOG_CHANNEL(auth_channel, warning) << get_entity_name() << " not found in database, id = " << id;
    }
}

template <typename EntityType, typename KeyType>
void celeritas::provider_manager<EntityType, KeyType>::add_entity(const optional_database_entity_change& optional_entity)
{
    auto entity = std::make_shared<EntityType>(*optional_entity);
    const auto key = get_entity_key(entity);

    LOG_CHANNEL(auth_channel, info) << "loaded " << get_entity_name() << " from db, key = " << format_key_info(key);

    std::lock_guard lock{ mutex_ };

    before_add_entity(entities_, entity);
    entities_.insert_or_assign(key, std::move(entity));
}

template <typename EntityType, typename KeyType>
celeritas::provider_manager<EntityType, KeyType>::entity_container celeritas::provider_manager<EntityType, KeyType>::build_entity_container(const database_entity_change_container& result)
{
    entity_container container{};
    container.reserve(result.size());

    for (const auto& row : result)
    {
        auto entity = std::make_shared<EntityType>(row);
        const auto key = get_entity_key(entity);
        container.emplace(key, entity);

        LOG_CHANNEL(auth_channel, info) << "loaded " << get_entity_name() << " from db, key = " << format_key_info(key);
    }

    return container;
}
