#include "guest_login_http_message_handler.h"
#include "guest_login_response.h"
#include "common/logger.h"
#include "common/snowflake_generator.h"
#include "config/app_config.h"
#include "database/database_pool_manager.h"
#include "database/mysql_database_session.h"
#include "database/generated/mysql/account.h"
#include "database/generated/redis/session_token.h"
#include "message/http_handle_parameter.h"
#include "server/account_status_type.h"
#include "server/account_type.h"
#include "server/game_error_type.h"

#include <boost/json.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

std::string celeritas::guest_login_http_message_handler::get_supported_type_name() const
{
    return guest_login_path.data();
}

bool celeritas::guest_login_http_message_handler::handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry)
{
    if (handle_parameter.get_server_type() != auth_type)
    {
        return false;
    }

    co_spawn(handle_parameter.get_io_context(),
             guest_login(handle_parameter),
             boost::asio::detached);

    return true;
}

celeritas::guest_login_http_message_handler::void_awaitable_type celeritas::guest_login_http_message_handler::guest_login(http_handle_parameter handle_parameter)
{
    try
    {
        co_return co_await do_guest_login(std::move(handle_parameter));
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(handler_channel, error) << "health check error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "health check unknown error.";
    }

    const guest_login_response response{ game_error_type::unknown, get_game_error_description(game_error_type::unknown) };
    handle_parameter.write(response.to_json_string());
}

celeritas::guest_login_http_message_handler::void_awaitable_type celeritas::guest_login_http_message_handler::do_guest_login(http_handle_parameter handle_parameter)
{
    const auto optional_device_id = handle_parameter.get_param(account::device_id_describe.data());
    if (!optional_device_id)
    {
        const guest_login_response response{ game_error_type::invalid_parameter, "device_id is required" };
        handle_parameter.write(response.to_json_string());

        co_return;
    }

    const auto& device_id = *optional_device_id;

    const auto pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto select = account::get_select_all(database_type::mysql);
    select->add_key(basis_database{ account::device_id_describe, device_id });
    auto accounts = co_await pool->select_all(select, account::get_database_field_container());

    auto account = co_await get_account(accounts, pool, device_id, handle_parameter.get_app_config());

    const auto token = generate_token();

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());

    session_token session_token{ database_type::redis, token };
    session_token.set_token(token);

    co_await redis_pool->execute_changes(session_token.get_modify());

    const guest_login_response response{ game_error_type::success, "login successful", token };
    handle_parameter.write(response.to_json_string());

    co_return;
}

celeritas::guest_login_http_message_handler::account_awaitable_type celeritas::guest_login_http_message_handler::get_account(const result_container& accounts, const database_pool_shared_ptr& database_pool, const std::string& device_id, const const_app_config_shared_ptr& app_config)
{
    if (accounts.empty())
    {
        const auto server_config = app_config->get_server_config();
        const auto account_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());

        account account{ database_type::mysql, account_id };
        account.set_device_id(device_id);
        account.set_account_name("guest_" + std::to_string(account_id));
        account.set_create_time(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        account.set_account_type(static_cast<int>(account_type::guest));
        account.set_status(static_cast<int>(account_status_type::normal));

        co_await database_pool->execute_changes(account.get_modify());

        co_return account;
    }

    account account{ accounts[0] };

    co_return account;
}

std::string celeritas::guest_login_http_message_handler::generate_token()
{
    boost::uuids::random_generator generator{};
    const auto uuid = generator();

    return boost::uuids::to_string(uuid);
}
