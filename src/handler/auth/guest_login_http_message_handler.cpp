#include "guest_login_http_message_handler.h"
#include "guest_login_response.h"
#include "common/logger.h"
#include "common/random_helper.h"
#include "database/database_pool_manager.h"
#include "database/mysql_database_session.h"
#include "database/generated/mysql/account.h"
#include "message/http_handle_parameter.h"
#include "server/game_error_type.h"

#include <boost/json.hpp>

std::string generate_token(const int64_t account_id)
{
    return "token_for_account_" + std::to_string(account_id);
}

std::string celeritas::guest_login_http_message_handler::get_supported_type_name() const
{
    return "guest_login";
}

bool celeritas::guest_login_http_message_handler::handle(const http_handle_parameter& handle_parameter, const http_message_registry_weak_ptr& message_registry)
{
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
}

celeritas::guest_login_http_message_handler::void_awaitable_type celeritas::guest_login_http_message_handler::do_guest_login(http_handle_parameter handle_parameter)
{
    const auto optional_device_id = handle_parameter.get_param("device_id");
    if (!optional_device_id)
    {
        const guest_login_response response{ game_error_type::invalid_parameter, "device_id is required" };
        handle_parameter.write(response.to_json_string());

        co_return;
    }

    const auto& device_id = *optional_device_id;

    const auto pool = database_pool_manager::get_instance().get_pool("mysql_db");
    const auto select = account::get_select_all(database_type::mysql);
    select->add_key(basis_database{ account::device_id_describe, device_id });
    auto accounts = co_await pool->select_all(select, account::get_database_field_container());

    auto account_id = 0LL;
    if (accounts.empty())
    {
        account account{ database_type::mysql, account_id };
        account.set_device_id(device_id);
        account.set_account_name("guest_" + std::to_string(account_id));
        account.set_create_time(0);

        // 返回值？？
        co_await pool->execute_changes(account.get_modify());
    }
    else
    {
        account account{ accounts[0] };

        account_id = account.get_account_id();
    }

    const auto token = generate_token(account_id);

    const guest_login_response response{ game_error_type::success, "login successful", token };
    handle_parameter.write(response.to_json_string());

    co_return;
}
