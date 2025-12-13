#include "mock_database_pool.h"
#include "database/database_entity_change.h"

celeritas::database_pool_base::bool_awaitable_type celeritas::mock_database_pool::execute_changes(const database_entity_change_const_shared_ptr& database, int expiration_time)
{
    ++execute_changes_call_count_;
    co_return true;
}

celeritas::database_pool_base::void_awaitable_type celeritas::mock_database_pool::async_initialize()
{
    co_return;
}

void celeritas::mock_database_pool::cleanup_database_by_duration()
{
}

celeritas::database_pool_base::bool_awaitable_type celeritas::mock_database_pool::is_health()
{
    co_return true;
}

celeritas::database_pool_base::database_entity_change_awaitable_type celeritas::mock_database_pool::select_one(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container)
{
    co_return std::nullopt;
}

celeritas::database_pool_base::result_container_awaitable_type celeritas::mock_database_pool::select_all(const database_entity_change_const_shared_ptr& database, const database_field_container& field_name_container)
{
    co_return result_container{};
}

int celeritas::mock_database_pool::get_execute_changes_call_count() const
{
    return execute_changes_call_count_;
}