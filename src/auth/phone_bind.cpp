#include "app_secret.h"
#include "phone_bind.h"
#include "phone_bind_response.h"
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

celeritas::phone_bind::phone_bind(http_handle_parameter handle_parameter)
    : handle_parameter_{ std::move(handle_parameter) }
{
}

celeritas::phone_bind::void_awaitable_type celeritas::phone_bind::response()
{
    const auto optional_token = handle_parameter_.get_param("token");
    if (!optional_token)
    {
        const phone_bind_response response{ game_error_type::invalid_parameter, "token is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_phone = handle_parameter_.get_param("phone");
    if (!optional_phone)
    {
        const phone_bind_response response{ game_error_type::invalid_parameter, "phone is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto& phone = *optional_phone;
    if (const std::regex phone_regex(R"(^1\d{10}$)");
        !std::regex_match(phone, phone_regex))
    {
        const phone_bind_response response{ game_error_type::invalid_parameter, "phone is invalid" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_timestamp = handle_parameter_.get_param("timestamp");
    if (!optional_timestamp)
    {
        const phone_bind_response response{ game_error_type::invalid_parameter, "timestamp is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto timestamp = boost::lexical_cast<int64_t>(*optional_timestamp);
    const auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // 检查时间戳，如果请求时间是 5 分钟前的，直接拒绝
    if (current_time - timestamp > minute * 5)
    {
        const phone_bind_response response{ game_error_type::timestamp_expired, "timestamp is expired" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_app_id = handle_parameter_.get_param("app_id");
    if (!optional_app_id)
    {
        const phone_bind_response response{ game_error_type::invalid_parameter, "app_id is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_sign = handle_parameter_.get_param("sign");
    if (!optional_sign)
    {
        const phone_bind_response response{ game_error_type::invalid_parameter, "sign is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_code = handle_parameter_.get_param("code");
    if (!optional_code)
    {
        const phone_bind_response response{ game_error_type::invalid_parameter, "code is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto& token = *optional_token;
    const auto app_id = boost::lexical_cast<int64_t>(*optional_app_id);
    const auto secret = app_secret::get_instance().get_key(app_id);
    const auto code = boost::lexical_cast<int>(*optional_code);

    if (const auto hmac_sha256 = calculate_hmac_sha256(app_id, phone, token, code, timestamp, secret);
        hmac_sha256 != *optional_sign)
    {
        const phone_bind_response response{ game_error_type::sign_error, "sign error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }
    const auto session_token_select = session_token::get_select(database_type::redis, token);
    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto session_token_result = co_await redis_pool->select_one(session_token_select, session_token::get_database_field_container());
    if (!session_token_result)
    {
        const phone_bind_response response{ game_error_type::token_error, "token error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    session_token session_token{ *session_token_result };

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto select = account::get_select(database_type::mysql, session_token.get_account_id());
    auto optional_account = co_await mysql_pool->select_one(select, account::get_database_field_container());

    if (!optional_account)
    {
        const phone_bind_response response{ game_error_type::account_error, "account error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account_bind::account_type_describe, static_cast<int>(account_type::phone) },
                                                                                                            { account_bind::auth_key_describe, phone },
                                                                                                            { account_bind::app_id_describe, app_id } });
    const auto account_bind_select = account_bind::get_select(database_type::mysql, key);
    auto optional_account_bind = co_await mysql_pool->select_one(account_bind_select, account::get_database_field_container());
    if (optional_account_bind)
    {
        const phone_bind_response response{ game_error_type::account_bound, "account bound" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    account account{ *optional_account };
    if (account.get_password_hash().empty())
    {
        account.set_device_id(account.get_account_name());
        account.set_password_hash(generate_token());
    }

    const auto server_config = handle_parameter_.get_app_config()->get_server_config();
    const auto account_bind_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());
    account_bind account_bind{ database_type::mysql, account_bind_id };
    account_bind.set_account_id(account.get_account_id());
    account_bind.set_auth_key(phone);
    account_bind.set_app_id(app_id);
    account_bind.set_account_type(static_cast<int>(account_type::phone));

    if (co_await mysql_pool->execute_changes(account.get_modify()) &&
        co_await mysql_pool->execute_changes(account_bind.get_modify()))
    {
        const phone_bind_response response{ game_error_type::success, "phone bind success" };
        handle_parameter_.write(response.to_json_string());
    }
    else
    {
        const phone_bind_response response{ game_error_type::mysql_error, "mysql error" };
        handle_parameter_.write(response.to_json_string());
    }
}

std::string celeritas::phone_bind::calculate_hmac_sha256(int64_t app_id, const std::string& phone, const std::string& token, int code, int64_t timestamp, const std::string& secret_key)
{
    const auto data = std::format("{}{}{}{}{}", app_id, phone, token, code, timestamp);

    return hmac_sha256::calculate(data, secret_key);
}

std::string celeritas::phone_bind::generate_token()
{
    boost::uuids::random_generator generator{};
    const auto uuid = generator();

    return boost::uuids::to_string(uuid);
}