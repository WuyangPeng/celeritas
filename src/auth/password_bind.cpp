#include "password_bind.h"
#include "app_secret.h"
#include "password_bind_response.h"
#include "common/hmac_sha_256.h"
#include "common/snowflake_generator.h"
#include "config/app_config.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "database/generated/redis/auth/session_token.h"
#include "server/account_type.h"
#include "../message/game_error_type.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <openssl/evp.h>

#include <regex>

celeritas::password_bind::password_bind(http_handle_parameter handle_parameter)
    : handle_parameter_{ std::move(handle_parameter) }
{
}

celeritas::password_bind::void_awaitable_type celeritas::password_bind::response()
{
    const auto optional_token = handle_parameter_.get_param("token");
    if (!optional_token)
    {
        const password_bind_response response{ game_error_type::invalid_parameter, "token is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_account_parameter = handle_parameter_.get_param("account");
    if (!optional_account_parameter)
    {
        const password_bind_response response{ game_error_type::invalid_parameter, "account is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto& account_parameter = *optional_account_parameter;
    if (const std::regex account_regex{ R"(^[a-zA-Z0-9_.-]{4,32}$)" };
        !std::regex_match(account_parameter, account_regex))
    {
        const password_bind_response response{ game_error_type::invalid_parameter, "account is invalid" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_password = handle_parameter_.get_param("password");
    if (!optional_password)
    {
        const password_bind_response response{ game_error_type::invalid_parameter, "password is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto& password = *optional_password;
    if (const std::regex password_regex{ R"(^[a-zA-Z0-9!@#$%^&*()_+-=]{8,32}$)" };
        !std::regex_match(password, password_regex))
    {
        const password_bind_response response{ game_error_type::invalid_parameter, "password is invalid" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_timestamp = handle_parameter_.get_param("timestamp");
    if (!optional_timestamp)
    {
        const password_bind_response response{ game_error_type::invalid_parameter, "timestamp is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto timestamp = boost::lexical_cast<int64_t>(*optional_timestamp);
    const auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // 检查时间戳，如果请求时间是 5 分钟前的，直接拒绝
    if (current_time - timestamp > minute * 5)
    {
        const password_bind_response response{ game_error_type::timestamp_expired, "timestamp is expired" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_app_id = handle_parameter_.get_param("app_id");
    if (!optional_app_id)
    {
        const password_bind_response response{ game_error_type::invalid_parameter, "app_id is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_sign = handle_parameter_.get_param("sign");
    if (!optional_sign)
    {
        const password_bind_response response{ game_error_type::invalid_parameter, "sign is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_code = handle_parameter_.get_param("code");
    if (!optional_code)
    {
        const password_bind_response response{ game_error_type::invalid_parameter, "code is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto& token = *optional_token;
    const auto app_id = boost::lexical_cast<int64_t>(*optional_app_id);
    const auto secret = app_secret::get_instance().get_key(app_id);
    const auto code = boost::lexical_cast<int>(*optional_code);

    if (const auto hmac_sha256 = calculate_hmac_sha256(app_id, account_parameter, password, token, code, timestamp, secret);
        hmac_sha256 != *optional_sign)
    {
        const password_bind_response response{ game_error_type::sign_error, "sign error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }
    const auto session_token_select = session_token::get_select(database_type::redis, token);
    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto session_token_result = co_await redis_pool->select_one(session_token_select, session_token::get_database_field_container());
    if (!session_token_result)
    {
        const password_bind_response response{ game_error_type::token_error, "token error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    session_token session_token{ *session_token_result };

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto select = account::get_select(database_type::mysql, session_token.get_account_id());
    auto optional_account = co_await mysql_pool->select_one(select, account::get_database_field_container());

    if (!optional_account)
    {
        const password_bind_response response{ game_error_type::account_error, "account error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account_bind::account_type_describe, static_cast<int>(account_type::password) },
                                                                                                            { account_bind::auth_key_describe, account_parameter },
                                                                                                            { account_bind::app_id_describe, app_id } });
    const auto account_bind_select = account_bind::get_select(database_type::mysql, key);
    auto optional_account_bind = co_await mysql_pool->select_one(account_bind_select, account::get_database_field_container());
    if (optional_account_bind)
    {
        const password_bind_response response{ game_error_type::account_bound, "account bound" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    account account{ *optional_account };
    const auto salt = generate_token();
    const auto hashed_password = hmac_sha256::calculate(password, salt);
    account.set_salt(salt);
    account.set_password_hash(hashed_password);

    const auto server_config = handle_parameter_.get_app_config()->get_server_config();
    const auto account_bind_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());
    account_bind account_bind{ database_type::mysql, account_bind_id };
    account_bind.set_account_id(account.get_account_id());
    account_bind.set_auth_key(account_parameter);
    account_bind.set_app_id(app_id);
    account_bind.set_account_type(static_cast<int>(account_type::password));

    if (co_await mysql_pool->execute_changes(account.get_modify()) &&
        co_await mysql_pool->execute_changes(account_bind.get_modify()))
    {
        const password_bind_response response{ game_error_type::success, "phone bind success" };
        handle_parameter_.write(response.to_json_string());
    }
    else
    {
        const password_bind_response response{ game_error_type::mysql_error, "mysql error" };
        handle_parameter_.write(response.to_json_string());
    }
}

std::string celeritas::password_bind::calculate_hmac_sha256(int64_t app_id, const std::string& account, const std::string& password, const std::string& token, int code, int64_t timestamp, const std::string& secret_key)
{
    const auto data = std::format("{}{}{}{}{}", app_id, account, password, token, code, timestamp);

    return hmac_sha256::calculate(secret_key, data);
}

std::string celeritas::password_bind::generate_token()
{
    boost::uuids::random_generator generator{};
    const auto uuid = generator();

    return boost::uuids::to_string(uuid);
}