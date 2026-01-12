#include "mock_database_session.h"
#include "database/basic/database_entity_change.h"

celeritas::mock_database_session::mock_database_session()
    : is_health_called_{ false },
      execute_changes_called_{ false },
      select_one_called_{ false },
      select_all_called_{ false }
{
}

celeritas::database_session::bool_awaitable_type celeritas::mock_database_session::is_health()
{
    is_health_called_ = true;
    co_return true;
}

celeritas::database_session::void_awaitable_type celeritas::mock_database_session::execute_changes(const const_database_entity_change_shared_ptr& database, int expiration_time)
{
    execute_changes_called_ = true;
    co_return;
}

celeritas::database_session::database_entity_change_awaitable_type celeritas::mock_database_session::select_one(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    select_one_called_ = true;
    co_return std::nullopt;
}

celeritas::database_session::result_container_awaitable_type celeritas::mock_database_session::select_all(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    select_all_called_ = true;
    co_return result_container{};
}

bool celeritas::mock_database_session::get_is_health_called() const
{
    return is_health_called_;
}

bool celeritas::mock_database_session::get_execute_changes_called() const
{
    return execute_changes_called_;
}

bool celeritas::mock_database_session::get_select_one_called() const
{
    return select_one_called_;
}

bool celeritas::mock_database_session::get_select_all_called() const
{
    return select_all_called_;
}
