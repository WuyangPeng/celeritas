#include "login_servers.h"
#include "database/database_pool_manager.h"
#include "database/generated/redis/auth/session_token.h"
#include "detail/login_servers_parameter.h"

celeritas::login_servers::login_servers(http_handle_parameter handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::auth_service_base::void_awaitable_type celeritas::login_servers::response()
{
    login_servers_parameter login_servers_parameter{ get_http_handle_parameter() };
    if (login_servers_parameter.is_failure())
    {
        co_return write(login_servers_parameter.get_response());
    }

    const auto token = login_servers_parameter.get_token();

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());
    const auto mongo_pool = database_pool_manager::get_instance().get_pool(mongo_auth_db_name.data());

    const auto optional_session_token = co_await redis_pool->select_one(session_token::get_select(database_type::redis, token), session_token::get_database_field_container());
    if (!optional_session_token)
    {
        write(login_servers_response{ game_error_type::token_error });
    }
}