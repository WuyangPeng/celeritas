#include "app_secret.h"
#include "guest_login.h"
#include "guest_login_response.h"
#include "../message/game_error_type.h"
#include "common/celeritas_error.h"
#include "common/hmac_sha_256.h"
#include "common/snowflake_generator.h"
#include "common/time_helper.h"
#include "config/app_config.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "database/generated/redis/auth/session_token.h"
#include "server/account_status_type.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <openssl/evp.h>

#include <regex>

celeritas::guest_login::guest_login(http_handle_parameter handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::guest_login::void_awaitable_type celeritas::guest_login::response()
{
    const auto optional_device_id = get_param(account::device_id_describe.data());
    if (!optional_device_id)
    {
        const guest_login_response response{ game_error_type::invalid_parameter, "device_id is required" };
        write(response);

        co_return;
    }

    const auto optional_app_id = get_param("app_id");
    if (!optional_app_id)
    {
        const guest_login_response response{ game_error_type::invalid_parameter, "app_id is required" };
        write(response);

        co_return;
    }

    const auto optional_timestamp = get_param("timestamp");
    if (!optional_timestamp)
    {
        const guest_login_response response{ game_error_type::invalid_parameter, "timestamp is required" };
        write(response);

        co_return;
    }

    const auto optional_sign = get_param("sign");
    if (!optional_sign)
    {
        const guest_login_response response{ game_error_type::invalid_parameter, "sign is required" };
        write(response);

        co_return;
    }

    const auto& device_id = *optional_device_id;
    const auto app_id = boost::lexical_cast<int64_t>(*optional_app_id);
    const auto secret = app_secret::get_instance().get_key(app_id);
    const auto timestamp = boost::lexical_cast<int64_t>(*optional_timestamp);

    const auto current_time = time_helper::get_current_milliseconds();

    // 检查时间戳，如果请求时间是 5 分钟前的，直接拒绝
    if (current_time - timestamp > minute * 5)
    {
        const guest_login_response response{ game_error_type::timestamp_expired, "timestamp is expired" };
        write(response);

        co_return;
    }

    const auto hmac_sha256 = calculate_hmac_sha256(app_id, device_id, timestamp, secret);
    if (hmac_sha256 != *optional_sign)
    {
        const guest_login_response response{ game_error_type::sign_error, "sign error" };
        write(response);

        co_return;
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account::device_id_describe, device_id },
                                                                                                            { account::app_id_describe, app_id } });

    const auto select = account::get_select(database_type::mysql, key);
    auto optional_account = co_await mysql_pool->select_one(select, account::get_database_field_container());
    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    auto account = co_await get_account(optional_account, redis_pool, app_id, device_id, get_app_config());

    if (!account.get_password_hash().empty())
    {
        const guest_login_response response{ game_error_type::no_guest_account, "no guest account" };
        write(response);

        co_return;
    }

    const auto token = generate_token();

    session_token session_token{ database_type::redis, token };
    session_token.set_token(token);
    session_token.set_account_id(account.get_account_id());
    session_token.set_is_new_account(!optional_account);

    // 这里没有删除旧的token，旧的token依赖redis有效时间进行删除。
    if (co_await redis_pool->execute_changes(session_token.get_modify()))
    {
        const auto database_config = get_app_config()->get_database_config(redis_db_name.data());
        const auto expire_milliseconds = current_time + database_config.get_expire_seconds() * milliseconds;

        const guest_login_response response{ game_error_type::success, "login successful", token, expire_milliseconds };
        write(response);
    }
    else
    {
        const guest_login_response response{ game_error_type::redis_error, "redis error" };
        write(response);
    }

    co_return;
}

std::string celeritas::guest_login::calculate_hmac_sha256(int64_t app_id, const std::string& device_id, int64_t timestamp, const std::string& secret_key)
{
    const auto data = std::format("{}{}{}", app_id, device_id, timestamp);

    return hmac_sha256::calculate(data, secret_key);
}

celeritas::guest_login::account_awaitable_type celeritas::guest_login::get_account(const optional_basis_database_manager& basis_database_manager,
                                                                                   const database_pool_shared_ptr& redis_pool,
                                                                                   int64_t app_id,
                                                                                   const std::string& device_id,
                                                                                   const const_app_config_shared_ptr& app_config)
{
    if (basis_database_manager)
    {
        account account{ *basis_database_manager };

        co_return account;
    }

    const auto server_config = app_config->get_server_config();
    const auto account_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());

    // 账号只存入redis，等待玩家真正登陆时再写入mysql
    account account{ database_type::redis, account_id };
    account.set_device_id(device_id);
    account.set_app_id(app_id);
    account.set_account_name("guest_" + std::to_string(account_id));
    account.set_create_time(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    account.set_status(static_cast<int>(account_status_type::normal));

    if (co_await redis_pool->execute_changes(account.get_modify()))
    {
        co_return account;
    }

    throw celeritas_error("guest login error");
}