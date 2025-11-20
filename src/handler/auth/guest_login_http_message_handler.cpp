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

    if (accounts.empty())
    {
        const auto account_id = 0;
        account account{ database_type::mysql, account_id };
        account.set_device_id(device_id);
        account.set_account_name("guest_" + std::to_string(account_id));
        account.set_create_time(0);

        // 返回值？？
        co_await pool->execute_changes(account.get_modify());
    }
    else
    {
      //  const auto account_id = accounts[0];
    }

    /*  account::get_select(database_type::mysql,)
      auto session = pool->select_one("account");
      if (!session)
      {
          // Handle error: failed to get database session
          boost::json::object response_json;
          response_json["code"] = static_cast<int>(game_error_type::server_error);
          response_json["message"] = "Failed to get database session";
          handle_parameter.write(boost::json::serialize(response_json));
          co_return;
      }

      auto account_entity = std::make_shared<celeritas::database_entity::account>();
      auto accounts = account_entity->select_all(session, "device_id = '" + device_id + "'");

      int64_t account_id = 0;

      if (accounts.empty())
      {
          // Create a new guest account
          account_id = celeritas::random_helper::generate(); // Assuming snowflake ID generator
          account_entity->set_account_id(account_id);
          account_entity->set_device_id(device_id);
          account_entity->set_account_name("guest_" + std::to_string(account_id));
          account_entity->set_account_type(0); // Guest account type
          account_entity->set_create_time(celeritas::time_helper::get_utc_time());
          account_entity->set_status(0); // Active status
          account_entity->set_password_hash("");
          account_entity->set_salt("");

          if (!account_entity->insert(session))
          {
              // Handle error: failed to insert new account
              boost::json::object response_json;
              response_json["code"] = static_cast<int>(game_error_type::database_error);
              response_json["message"] = "Failed to create guest account";
              handle_parameter.write(boost::json::serialize(response_json));
              co_return;
          }
      }
      else
      {
          // Account exists, use the existing one
          account_id = accounts[0]->get_account_id();
          // Optionally, update last login time or other info here
      }

      // Generate token and send response
      const std::string token = generate_token(account_id);

      boost::json::object response_json;
      response_json["code"] = static_cast<int>(game_error_type::ok);
      response_json["token"] = token;
      response_json["message"] = "Login successful";

      handle_parameter.write(boost::json::serialize(response_json));
  */
    co_return;
}
