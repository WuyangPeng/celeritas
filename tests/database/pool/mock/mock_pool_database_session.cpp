#include "mock_pool_database_session.h"
#include "common/core/celeritas_error.h"
#include "database/basic/database_entity_change.h"

celeritas::mock_pool_database_session::mock_pool_database_session(const any_io_executor& any_io_executor)
    : base_type{ any_io_executor }
{
}

celeritas::mock_pool_database_session::mock_pool_database_session(const std::string& host,
                                                                  int port,
                                                                  const std::string& user,
                                                                  const std::string& password,
                                                                  const std::string& uri,
                                                                  const std::string& db_name,
                                                                  int expire_seconds,
                                                                  const any_io_executor& any_io_executor)
    : base_type{ any_io_executor }
{
}

celeritas::database_session::void_awaitable_type celeritas::mock_pool_database_session::async_connect()
{
    co_return;
}

celeritas::database_session::bool_awaitable_type celeritas::mock_pool_database_session::is_health()
{
    co_return true;
}

celeritas::database_session::void_awaitable_type celeritas::mock_pool_database_session::execute_changes(const const_database_entity_change_shared_ptr& database, int expiration_time)
{
    co_return;
}

celeritas::database_session::database_entity_change_awaitable_type celeritas::mock_pool_database_session::select_one(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    co_return std::nullopt;
}

celeritas::database_session::result_container_awaitable_type celeritas::mock_pool_database_session::select_all(const const_database_entity_change_shared_ptr& database, const database_field_container& field_name_container)
{
    co_return result_container{};
}

celeritas::database_session::result_container_awaitable_type celeritas::mock_pool_database_session::select_page(const const_database_entity_change_shared_ptr&,
                                                                                                                 const database_field_container&,
                                                                                                                 const database_select_options&)
{
    co_return result_container{};
}

celeritas::database_session::int64_awaitable_type celeritas::mock_pool_database_session::select_count(const const_database_entity_change_shared_ptr&)
{
    co_return 0;
}

celeritas::database_session::int64_awaitable_type celeritas::mock_pool_database_session::select_count(const const_database_entity_change_shared_ptr& database,
                                                                                                      const database_select_options&)
{
    co_return co_await select_count(database);
}

