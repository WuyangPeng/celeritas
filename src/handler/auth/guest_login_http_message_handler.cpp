#include "guest_login_http_message_handler.h"
#include "auth/app_secret.h"
#include "auth/guest_login_response.h"
#include "boost/lexical_cast.hpp"
#include "common/celeritas_error.h"
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

#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string/case_conv.hpp>
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
        LOG_CHANNEL(handler_channel, error) << "guest login error: " << error.what();
    }
    catch (...)
    {
        LOG_CHANNEL(handler_channel, fatal) << "guest login unknown error.";
    }

    const guest_login_response response{ game_error_type::unknown, "unknown error" };
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

    const auto optional_app_id = handle_parameter.get_param("app_id");
    if (!optional_app_id)
    {
        const guest_login_response response{ game_error_type::invalid_parameter, "app_id is required" };
        handle_parameter.write(response.to_json_string());

        co_return;
    }

    const auto optional_timestamp = handle_parameter.get_param("timestamp");
    if (!optional_timestamp)
    {
        const guest_login_response response{ game_error_type::invalid_parameter, "timestamp is required" };
        handle_parameter.write(response.to_json_string());

        co_return;
    }

    const auto optional_sign = handle_parameter.get_param("sign");
    if (!optional_sign)
    {
        const guest_login_response response{ game_error_type::invalid_parameter, "sign is required" };
        handle_parameter.write(response.to_json_string());

        co_return;
    }

    const auto& device_id = *optional_device_id;
    const auto app_id = boost::lexical_cast<int>(*optional_app_id);
    const auto secret = app_secret::get_instance().get_key(app_id);
    const auto timestamp = boost::lexical_cast<int64_t>(*optional_timestamp);

    const auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // 检查时间戳，如果请求时间是 5 分钟前的，直接拒绝
    if (current_time - timestamp > minute * 5)
    {
        const guest_login_response response{ game_error_type::timestamp_expired, "timestamp is expired" };
        handle_parameter.write(response.to_json_string());

        co_return;
    }

    const auto hmac_sha256 = calculate_hmac_sha256(app_id, device_id, timestamp, secret);
    if (hmac_sha256 != *optional_sign)
    {
        const guest_login_response response{ game_error_type::sign_error, "sign error" };
        handle_parameter.write(response.to_json_string());
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto select = account::get_select_all(database_type::mysql);
    select->add_key(basis_database{ account::device_id_describe, device_id });
    auto accounts = co_await mysql_pool->select_all(select, account::get_database_field_container());
    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    auto account = co_await get_account(accounts, redis_pool, device_id, handle_parameter.get_app_config());

    const auto token = generate_token();

    session_token session_token{ database_type::redis, token };
    session_token.set_token(token);
    session_token.set_account_id(account.get_account_id());
    session_token.set_is_new_account(accounts.empty());

    // 这里没有删除旧的token，旧的token依赖redis有效时间进行删除。
    if (co_await redis_pool->execute_changes(session_token.get_modify()))
    {
        const auto database_config = handle_parameter.get_app_config()->get_database_config(redis_db_name.data());
        const auto expire_milliseconds = current_time + database_config.get_expire_seconds() * milliseconds;

        const guest_login_response response{ game_error_type::success, "login successful", token, expire_milliseconds };
        handle_parameter.write(response.to_json_string());
    }
    else
    {
        const guest_login_response response{ game_error_type::redis_error, "redis error" };
        handle_parameter.write(response.to_json_string());
    }

    co_return;
}

celeritas::guest_login_http_message_handler::account_awaitable_type celeritas::guest_login_http_message_handler::get_account(const result_container& accounts, const database_pool_shared_ptr& database_pool, const std::string& device_id, const const_app_config_shared_ptr& app_config)
{
    for (const auto& element : accounts)
    {
        if (element.get_value<database_data_type::int32_type>(account::account_type_describe, static_cast<int>(account_type::invalid)) == static_cast<int>(account_type::guest))
        {
            account account{ element };

            co_return account;
        }
    }

    const auto server_config = app_config->get_server_config();
    const auto account_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());

    // 账号只存入redis，等待玩家真正登陆时再写入mysql
    account account{ database_type::redis, account_id };
    account.set_device_id(device_id);
    account.set_account_name("guest_" + std::to_string(account_id));
    account.set_create_time(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    account.set_account_type(static_cast<int>(account_type::guest));
    account.set_status(static_cast<int>(account_status_type::normal));

    if (co_await database_pool->execute_changes(account.get_modify()))
    {
        co_return account;
    }

    throw celeritas_error("guest login error");
}

std::string celeritas::guest_login_http_message_handler::generate_token()
{
    boost::uuids::random_generator generator{};
    const auto uuid = generator();

    return boost::uuids::to_string(uuid);
}

std::string celeritas::guest_login_http_message_handler::calculate_hmac_sha256(int app_id, const std::string& device_id, int64_t timestamp, const std::string& secret_key)
{
    const auto data = std::format("{}{}{}", app_id, device_id, timestamp);

    std::array<unsigned char,EVP_MAX_MD_SIZE> result{};
    unsigned int result_length{};

    // HMAC 计算
    // 参数: 算法, Key, Key长度, 数据, 数据长度, 输出Buffer, 输出长度指针
    HMAC(EVP_sha256(),
         secret_key.c_str(), secret_key.length(),
         (unsigned char*)data.c_str(), data.length(),
         result.data(), &result_length);

    // Hex 转换
    std::string hex_output{};
    boost::algorithm::hex(result.data(), result.data() + result_length, std::back_inserter(hex_output));
    boost::algorithm::to_lower(hex_output);

    return hex_output;
}
