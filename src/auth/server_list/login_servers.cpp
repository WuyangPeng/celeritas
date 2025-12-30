#include "login_servers.h"
#include "login_servers_response.h"
#include "auth/data/server_cell_repository.h"
#include "auth/detail/login_servers/login_servers_parameter.h"
#include "common/core/time_helper.h"
#include "database/database_pool_manager.h"
#include "database/document/server_role.h"
#include "database/generated/mongo/auth/user_server_roles.h"
#include "database/generated/mysql/auth/account_last_login.h"
#include "database/generated/mysql/auth/server_cell.h"
#include "database/generated/redis/auth/session_token.h"
#include "initializer/initializer_fwd.h"
#include "service_registry/core/service_registry.h"
#include "service_registry/data/service_info.h"

celeritas::login_servers::login_servers(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }, server_role_{}
{
}

celeritas::auth_service_base::void_awaitable_type celeritas::login_servers::response()
{
    const login_servers_parameter login_servers_parameter{ get_http_handle_parameter() };
    if (login_servers_parameter.is_failure())
    {
        co_return co_await write_immediately(login_servers_parameter.get_response());
    }

    const auto token = login_servers_parameter.get_token();

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    const auto optional_session_token = co_await redis_pool->select_one(session_token::get_select(database_type::redis, token), session_token::get_database_field_container());
    if (!optional_session_token)
    {
        co_return co_await write_immediately(login_servers_response{ game_error_type::token_error });
    }

    const session_token session_token{ *optional_session_token };
    co_await create_server_role(login_servers_parameter, session_token);

    if (login_servers_parameter.is_only_preferred())
    {
        co_return co_await response_is_only_preferred(login_servers_parameter, session_token, redis_pool);
    }

    co_return co_await response_is_all(login_servers_parameter);
}

celeritas::login_server_info celeritas::login_servers::get_login_server_info(const login_servers_parameter& login_servers_parameter, const server_cell& server_cell)
{
    const auto optional_services_info = service_registry::get_idle_services(gateway_type.data(), server_cell.get_game_server_id());

    login_server_info login_server_info{ server_cell.get_game_server_id(), server_cell.get_server_name(), optional_services_info ? server_status_type::normal : server_status_type::maintenance };
    if (optional_services_info)
    {
        const auto& services_info = *optional_services_info;
        if (login_servers_parameter.is_websocket())
        {
            if (const auto websocket_secure_port = services_info.get_port(server_network_type::websocket_secure);
                0 < websocket_secure_port)
            {
                login_server_info.set_connection_info(connection_info{ services_info.get_external_host(), websocket_secure_port, server_network_type::websocket_secure });
            }
            else
            {
                if (const auto websocket_port = services_info.get_port(server_network_type::websocket);
                    0 < websocket_port)
                {
                    login_server_info.set_connection_info(connection_info{ services_info.get_external_host(), websocket_port, server_network_type::websocket });
                }
                else
                {
                    login_server_info.set_connection_info(connection_info{ services_info.get_external_host(), 0, server_network_type::websocket_secure });
                }
            }
        }
        else
        {
            if (const auto tcp_ssl_port = services_info.get_port(server_network_type::tcp_ssl);
                0 < tcp_ssl_port)
            {
                login_server_info.set_connection_info(connection_info{ services_info.get_external_host(), tcp_ssl_port, server_network_type::tcp_ssl });
            }
            else
            {
                if (const auto tcp_port = services_info.get_port(server_network_type::tcp);
                    0 < tcp_port)
                {
                    login_server_info.set_connection_info(connection_info{ services_info.get_external_host(), tcp_port, server_network_type::tcp });
                }
                else
                {
                    login_server_info.set_connection_info(connection_info{ services_info.get_external_host(), 0, server_network_type::tcp_ssl });
                }
            }
        }
    }

    if (const auto server_role = server_role_.find(server_cell.get_game_server_id());
        server_role != server_role_.cend())
    {
        login_server_info.set_player_role(player_role{ server_role->second.get_role_name(), server_role->second.get_last_login_time() });
    }

    return login_server_info;
}

celeritas::login_servers::void_awaitable_type celeritas::login_servers::create_server_role(const login_servers_parameter& login_servers_parameter,
                                                                                           const session_token& session_token)
{
    if (!session_token.is_new_account() && login_servers_parameter.is_include_details())
    {
        const auto mongo_pool = database_pool_manager::get_instance().get_pool(mongo_auth_db_name.data());

        if (const auto database_entity_change = co_await mongo_pool->select_one(user_server_roles::get_select(database_type::mongo, session_token.get_account_id()), user_server_roles::get_database_field_container()))
        {
            const user_server_roles user_server_roles{ *database_entity_change };
            for (const auto servers = user_server_roles.get_servers();
                 const auto& element : servers)
            {
                auto server_role = server_role::from_document(element);

                server_role_.emplace(server_role.get_game_server_id(), std::move(server_role));
            }
        }
    }
}

celeritas::auth_service_base::void_awaitable_type celeritas::login_servers::response_is_only_preferred(const login_servers_parameter& login_servers_parameter,
                                                                                                       const session_token& session_token,
                                                                                                       const database_pool_shared_ptr& redis_pool)
{
    if (!session_token.is_new_account())
    {
        auto optional_account_last_login = co_await redis_pool->select_one(account_last_login::get_select(database_type::redis, session_token.get_account_id()), account_last_login::get_database_field_container());
        if (!optional_account_last_login)
        {
            optional_account_last_login = co_await redis_pool->select_one(account_last_login::get_select(database_type::mysql, session_token.get_account_id()), account_last_login::get_database_field_container());
        }

        if (optional_account_last_login)
        {
            account_last_login account_last_login{ *optional_account_last_login };

            const auto optional_server_cell = server_cell_repository::get_instance().get_server_cell(account_last_login.get_game_server_id());
            if (!optional_server_cell)
            {
                co_return co_await write_immediately(login_servers_response{ game_error_type::server_error });
            }

            const auto& server_cell = *optional_server_cell;

            if (server_cell.get_launch_time() >= time_helper::get_current_milliseconds())
            {
                co_return co_await write_immediately(login_servers_response{ game_error_type::server_launch_error });
            }

            auto login_server_info = get_login_server_info(login_servers_parameter, server_cell);

            co_return co_await write_immediately(login_servers_response{ game_error_type::success, "get login servers success.", std::move(login_server_info) });
        }
    }

    const auto optional_server_cell = server_cell_repository::get_instance().get_last_server_cell(login_servers_parameter.get_app_id());
    if (!optional_server_cell)
    {
        co_return co_await write_immediately(login_servers_response{ game_error_type::server_error });
    }

    auto login_server_info = get_login_server_info(login_servers_parameter, *optional_server_cell);

    co_return co_await write_immediately(login_servers_response{ game_error_type::success, "get login servers success.", std::move(login_server_info) });
}

celeritas::auth_service_base::void_awaitable_type celeritas::login_servers::response_is_all(const login_servers_parameter& login_servers_parameter)
{
    const auto server_cell_container = server_cell_repository::get_instance().get_server_cell_by_app_id(login_servers_parameter.get_app_id(), login_servers_parameter.get_zone());
    login_servers_response::container_type container{};
    for (const auto& element : server_cell_container)
    {
        container.emplace_back(get_login_server_info(login_servers_parameter, element));
    }

    co_return co_await write_immediately(login_servers_response{ game_error_type::success, "get login servers success.", std::move(container) });
}