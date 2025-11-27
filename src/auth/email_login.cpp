#include "app_secret.h"
#include "auth_fwd.h"
#include "email_login.h"
#include "email_login_response.h"
#include "common/celeritas_error.h"
#include "common/hmac_sha_256.h"
#include "common/snowflake_generator.h"
#include "config/app_config.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "database/generated/redis/auth/email_code.h"
#include "database/generated/redis/auth/session_token.h"
#include "database/generated/redis/auth/sms_code.h"
#include "server/account_status_type.h"
#include "server/account_type.h"
#include "server/game_error_type.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <openssl/evp.h>

#include <regex>

celeritas::email_login::email_login(http_handle_parameter handle_parameter)
    : handle_parameter_{ std::move(handle_parameter) }
{
}

celeritas::email_login::void_awaitable_type celeritas::email_login::response()
{
    const auto optional_email = handle_parameter_.get_param("email");
    if (!optional_email)
    {
        const email_login_response response{ game_error_type::invalid_parameter, "email is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto& email = *optional_email;
    if (const std::regex email_regex{ R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)" };
        !std::regex_match(email, email_regex))
    {
        const email_login_response response{ game_error_type::invalid_parameter, "email is invalid" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_timestamp = handle_parameter_.get_param("timestamp");
    if (!optional_timestamp)
    {
        const email_login_response response{ game_error_type::invalid_parameter, "timestamp is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto timestamp = boost::lexical_cast<int64_t>(*optional_timestamp);
    const auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // 检查时间戳，如果请求时间是 5 分钟前的，直接拒绝
    if (current_time - timestamp > minute * 5)
    {
        const email_login_response response{ game_error_type::timestamp_expired, "timestamp is expired" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_app_id = handle_parameter_.get_param("app_id");
    if (!optional_app_id)
    {
        const email_login_response response{ game_error_type::invalid_parameter, "app_id is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_sign = handle_parameter_.get_param("sign");
    if (!optional_sign)
    {
        const email_login_response response{ game_error_type::invalid_parameter, "sign is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_code = handle_parameter_.get_param("code");
    if (!optional_code)
    {
        const email_login_response response{ game_error_type::invalid_parameter, "code is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto app_id = boost::lexical_cast<int64_t>(*optional_app_id);
    const auto secret = app_secret::get_instance().get_key(app_id);
    const auto code = boost::lexical_cast<int>(*optional_code);

    if (const auto hmac_sha256 = calculate_hmac_sha256(app_id, email, code, timestamp, secret);
        hmac_sha256 != *optional_sign)
    {
        const email_login_response response{ game_error_type::sign_error, "sign error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto email_code_select = email_code::get_select(database_type::redis, email);
    auto optional_email_code = co_await redis_pool->select_one(email_code_select, email_code::get_database_field_container());
    if (!optional_email_code)
    {
        const email_login_response response{ game_error_type::code_expired, "code is expired" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    sms_code email_code{ *optional_email_code };
    if (email_code.get_code() != code)
    {
        const email_login_response response{ game_error_type::code_error, "code is error" };
        handle_parameter_.write(response.to_json_string());

        email_code.modify_retry_count(1);

        if (email_code.get_retry_count() >= email_code_retry_count)
        {
            co_await redis_pool->execute_changes(email_code.get_delete());
        }
        else
        {
            co_await redis_pool->execute_changes(email_code.get_modify());
        }

        co_return;
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account_bind::account_type_describe, static_cast<int>(account_type::email) },
                                                                                                            { account_bind::auth_key_describe, email },
                                                                                                            { account_bind::app_id_describe, app_id } });

    const auto select = account_bind::get_select(database_type::mysql, key);

    auto optional_account_bind = co_await mysql_pool->select_one(select, account_bind::get_database_field_container());

    auto account = co_await get_account(optional_account_bind, redis_pool, mysql_pool, app_id, email, handle_parameter_.get_app_config());

    const auto token = generate_token();

    session_token session_token{ database_type::redis, token };
    session_token.set_token(token);
    session_token.set_account_id(account.get_account_id());
    session_token.set_is_new_account(!optional_account_bind);

    // 这里没有删除旧的token，旧的token依赖redis有效时间进行删除。
    if (co_await redis_pool->execute_changes(session_token.get_modify()))
    {
        const auto database_config = handle_parameter_.get_app_config()->get_database_config(redis_db_name.data());
        const auto expire_milliseconds = current_time + database_config.get_expire_seconds() * milliseconds;

        const email_login_response response{ game_error_type::success, "login successful", token, expire_milliseconds };
        handle_parameter_.write(response.to_json_string());
    }
    else
    {
        const email_login_response response{ game_error_type::redis_error, "redis error" };
        handle_parameter_.write(response.to_json_string());
    }

    co_await redis_pool->execute_changes(email_code.get_delete());

    co_return;
}

std::string celeritas::email_login::calculate_hmac_sha256(int64_t app_id, const std::string& email, int code, int64_t timestamp, const std::string& secret_key)
{
    const auto data = std::format("{}{}{}{}", app_id, email, code, timestamp);

    return hmac_sha256::calculate(data, secret_key);
}

std::string celeritas::email_login::generate_token()
{
    boost::uuids::random_generator generator{};
    const auto uuid = generator();

    return boost::uuids::to_string(uuid);
}

celeritas::email_login::account_awaitable_type celeritas::email_login::get_account(const optional_basis_database_manager& basis_database_manager,
                                                                                   const database_pool_shared_ptr& redis_pool,
                                                                                   const database_pool_shared_ptr& mysql_pool,
                                                                                   int64_t app_id,
                                                                                   const std::string& email,
                                                                                   const const_app_config_shared_ptr& app_config)
{
    if (basis_database_manager)
    {
        account_bind account_bind{ *basis_database_manager };

        if (auto optional_account = co_await mysql_pool->select_one(account::get_select(database_type::mysql, account_bind.get_account_id()), account::get_database_field_container()))
        {
            account account{ *optional_account };

            co_return account;
        }
    }

    const auto server_config = app_config->get_server_config();
    const auto account_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());

    // 账号只存入redis，等待玩家真正登陆时再写入mysql
    account account{ database_type::redis, account_id };
    account.set_account_name("email_" + std::to_string(account_id));
    account.set_create_time(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    account.set_status(static_cast<int>(account_status_type::normal));
    account.set_device_id(account.get_account_name());
    account.set_password_hash(generate_token());
    account.set_app_id(app_id);

    if (co_await redis_pool->execute_changes(account.get_modify()))
    {
        co_return account;
    }

    const auto account_bind_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());
    account_bind account_bind{ database_type::redis, account_bind_id };
    account_bind.set_account_id(account_id);
    account_bind.set_auth_key(email);
    account_bind.set_account_type(static_cast<int>(account_type::email));
    account_bind.set_app_id(app_id);

    if (co_await redis_pool->execute_changes(account_bind.get_modify()))
    {
        co_return account;
    }

    throw celeritas_error("email login error");
}