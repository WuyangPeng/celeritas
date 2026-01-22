#include "mock_database_pool_base.h"
#include "database/basic/database_entity_change.h"

celeritas::database_pool_base::void_awaitable_type celeritas::mock_database_pool_base::async_initialize()
{
    co_return;
}

void celeritas::mock_database_pool_base::cleanup_database_by_duration()
{
    ++cleanup_called_count_;
}

celeritas::database_pool_base::bool_awaitable_type celeritas::mock_database_pool_base::is_health()
{
    co_return true;
}

celeritas::database_pool_base::bool_awaitable_type celeritas::mock_database_pool_base::execute_changes(const const_database_entity_change_shared_ptr&, const int expiration_time)
{
    execute_changes_called_ = true;
    expiration_time_passed_ = expiration_time;
    co_return execute_changes_result_;
}

celeritas::database_pool_base::optional_database_entity_change_awaitable_type celeritas::mock_database_pool_base::select_one(const const_database_entity_change_shared_ptr&, const database_field_container&)
{
    co_return std::nullopt;
}

celeritas::database_pool_base::result_container_awaitable_type celeritas::mock_database_pool_base::select_all(const const_database_entity_change_shared_ptr&, const database_field_container&)
{
    co_return result_container{};
}

void celeritas::mock_database_pool_base::set_execute_changes_result(bool result)
{
    execute_changes_result_ = result;
}

int celeritas::mock_database_pool_base::get_cleanup_called_count() const
{
    return cleanup_called_count_;
}

bool celeritas::mock_database_pool_base::is_execute_changes_called() const
{
    return execute_changes_called_;
}

int celeritas::mock_database_pool_base::get_expiration_time_passed() const
{
    return expiration_time_passed_;
}