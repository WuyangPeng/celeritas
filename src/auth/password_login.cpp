#include "password_login.h"
#include "app_secret.h"
#include "auth_fwd.h"
#include "password_login_response.h"
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
#include "../message/game_error_type.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <regex>
#include <vector>

celeritas::password_login::password_login(http_handle_parameter handle_parameter)
    : handle_parameter_{ std::move(handle_parameter) }
{
}

celeritas::password_login::void_awaitable_type celeritas::password_login::response()
{
    const auto optional_account = handle_parameter_.get_param("account");
    if (!optional_account)
    {
        const password_login_response response{ game_error_type::invalid_parameter, "account is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto& account_parameter = *optional_account;
    if (const std::regex account_regex{ R"(^[a-zA-Z0-9_.-]{4,32}$)" };
        !std::regex_match(account_parameter, account_regex))
    {
        const password_login_response response{ game_error_type::invalid_parameter, "account is invalid" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_password = handle_parameter_.get_param("password");
    if (!optional_password)
    {
        const password_login_response response{ game_error_type::invalid_parameter, "password is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto& password = *optional_password;
    if (const std::regex password_regex{ R"(^[a-zA-Z0-9!@#$%^&*()_+-=]{8,32}$)" };
        !std::regex_match(password, password_regex))
    {
        const password_login_response response{ game_error_type::invalid_parameter, "password is invalid" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_timestamp = handle_parameter_.get_param("timestamp");
    if (!optional_timestamp)
    {
        const password_login_response response{ game_error_type::invalid_parameter, "timestamp is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto timestamp = boost::lexical_cast<int64_t>(*optional_timestamp);
    const auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // 检查时间戳，如果请求时间是 5 分钟前的，直接拒绝
    if (current_time - timestamp > minute * 5)
    {
        const password_login_response response{ game_error_type::timestamp_expired, "timestamp is expired" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_app_id = handle_parameter_.get_param("app_id");
    if (!optional_app_id)
    {
        const password_login_response response{ game_error_type::invalid_parameter, "app_id is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_sign = handle_parameter_.get_param("sign");
    if (!optional_sign)
    {
        const password_login_response response{ game_error_type::invalid_parameter, "sign is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto app_id = boost::lexical_cast<int64_t>(*optional_app_id);
    const auto secret = app_secret::get_instance().get_key(app_id);

    if (const auto hmac_sha256 = calculate_hmac_sha256(app_id, account_parameter, password, timestamp, secret);
        hmac_sha256 != *optional_sign)
    {
        const password_login_response response{ game_error_type::sign_error, "sign error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account_bind::account_type_describe, static_cast<int>(account_type::password) },
                                                                                                            { account_bind::auth_key_describe, account_parameter },
                                                                                                            { account_bind::app_id_describe, app_id } });

    const auto select = account_bind::get_select(database_type::mysql, key);

    auto optional_account_bind = co_await mysql_pool->select_one(select, account_bind::get_database_field_container());
    if (optional_account_bind)
    {
        account_bind account_bind{ *optional_account_bind };

        const auto account_select = account::get_select(database_type::mysql, account_bind.get_account_id());

        auto optional_account_result = co_await mysql_pool->select_one(account_select, account_bind::get_database_field_container());

        if (optional_account_result)
        {
            account account{ *optional_account_result };
            if (account.get_salt().empty())
            {
                const password_login_response response{ game_error_type::password_error, "password error" };
                handle_parameter_.write(response.to_json_string());

                co_return;
            }

            if (hmac_sha256::calculate(password, account.get_salt()) != account.get_password_hash())
            {
                const password_login_response response{ game_error_type::password_error, "password error" };
                handle_parameter_.write(response.to_json_string());

                co_return;
            }

            const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
            co_return co_await login(redis_pool, account, false, current_time);
        }
    }

    const auto server_config = handle_parameter_.get_app_config()->get_server_config();
    const auto account_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());

    // 账号只存入redis，等待玩家真正登陆时再写入mysql
    account account{ database_type::redis, account_id };
    account.set_account_name("password_" + std::to_string(account_id));
    account.set_create_time(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    account.set_status(static_cast<int>(account_status_type::normal));
    account.set_device_id(account.get_account_name());

    const auto salt = generate_token();
    const auto hashed_password = hmac_sha256::calculate(password, salt);
    account.set_salt(salt);
    account.set_password_hash(hashed_password);

    account.set_app_id(app_id);
    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    if (!co_await redis_pool->execute_changes(account.get_modify()))
    {
        const password_login_response response{ game_error_type::redis_error, "redis error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto account_bind_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());
    account_bind account_bind{ database_type::redis, account_bind_id };
    account_bind.set_account_id(account_id);
    account_bind.set_auth_key(account_parameter);
    account_bind.set_account_type(static_cast<int>(account_type::password));
    account_bind.set_app_id(app_id);

    if (!co_await redis_pool->execute_changes(account_bind.get_modify()))
    {
        const password_login_response response{ game_error_type::redis_error, "redis error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    co_return co_await login(redis_pool, account, true, current_time);
}

std::string celeritas::password_login::calculate_hmac_sha256(int64_t app_id, const std::string& account, const std::string& password, int64_t timestamp, const std::string& secret_key)
{
    const auto data = std::format("{}{}{}{}", app_id, account, password, timestamp);

    return hmac_sha256::calculate(secret_key, data);
}

std::string celeritas::password_login::generate_token()
{
    boost::uuids::random_generator generator{};
    const auto uuid = generator();

    return boost::uuids::to_string(uuid);
}

celeritas::password_login::void_awaitable_type celeritas::password_login::login(const database_pool_shared_ptr& redis_pool, const account& account, bool is_new_account, int64_t current_time)
{
    const auto token = generate_token();

    session_token session_token{ database_type::redis, token };
    session_token.set_token(token);
    session_token.set_account_id(account.get_account_id());
    session_token.set_is_new_account(is_new_account);

    // 这里没有删除旧的token，旧的token依赖redis有效时间进行删除。
    if (co_await redis_pool->execute_changes(session_token.get_modify()))
    {
        const auto database_config = handle_parameter_.get_app_config()->get_database_config(redis_db_name.data());
        const auto expire_milliseconds = current_time + database_config.get_expire_seconds() * milliseconds;

        const password_login_response response{ game_error_type::success, "login successful", token, expire_milliseconds };
        handle_parameter_.write(response.to_json_string());
    }
    else
    {
        const password_login_response response{ game_error_type::redis_error, "redis error" };
        handle_parameter_.write(response.to_json_string());
    }

    co_return;
}