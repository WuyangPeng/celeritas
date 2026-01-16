#include "mock_config_database_pool.h"
#include "config/basic/database_type.h"
#include "database/basic/database_change_type.h"
#include "database/generated/mysql/config/time_refresh.h"

celeritas::database_pool_base::result_container_awaitable_type celeritas::mock_config_database_pool::select_all(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    if (const auto result = co_await select_one(database, field_name_container))
    {
        co_return result_container{ *result };
    }

    co_return result_container{};
}

celeritas::database_pool_base::bool_awaitable_type celeritas::mock_config_database_pool::execute_changes(const const_database_entity_change_shared_ptr& database, int expiration_time)
{
    co_return true;
}

celeritas::database_pool_base::void_awaitable_type celeritas::mock_config_database_pool::async_initialize()
{
    co_return;
}

void celeritas::mock_config_database_pool::cleanup_database_by_duration()
{
}

celeritas::database_pool_base::bool_awaitable_type celeritas::mock_config_database_pool::is_health()
{
    co_return true;
}

celeritas::database_pool_base::optional_database_entity_change_awaitable_type celeritas::mock_config_database_pool::select_one(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    const basis_database id{ time_refresh::id_describe, int64_t{ 1 } };
    const basis_database time_refresh{ time_refresh::time_refresh_type_describe, 1 };
    const basis_database parameter{ time_refresh::parameter_describe, 2 };

    database_entity_change database_entity_change{ database_type::mysql,
                                                   time_refresh::database_name,
                                                   database_change_type::update_type,
                                                   std::make_shared<basis_database_container>(basis_database_container::object_container{ id }) };
    database_entity_change.modify(id);
    database_entity_change.modify(time_refresh);
    database_entity_change.modify(parameter);

    co_return database_entity_change;
}