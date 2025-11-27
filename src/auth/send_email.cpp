#include "send_email.h"
#include "app_secret.h"
#include "auth_fwd.h"
#include "send_email_response.h"
#include "send_sms.h"
#include "send_sms_response.h"
#include "common/hmac_sha_256.h"
#include "common/random_helper.h"
#include "database/database_pool_manager.h"
#include "database/generated/redis/auth/email_code.h"
#include "database/generated/redis/auth/email_limit.h"
#include "server/game_error_type.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <openssl/evp.h>

#include <regex>

celeritas::send_email::send_email(http_handle_parameter handle_parameter)
    : handle_parameter_{ std::move(handle_parameter) }
{
}

celeritas::send_email::void_awaitable_type celeritas::send_email::response()
{
    const auto optional_email = handle_parameter_.get_param("email");
    if (!optional_email)
    {
        const send_email_response response{ game_error_type::invalid_parameter, "email is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto& email = *optional_email;
    if (const std::regex email_regex{ R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)" };
        !std::regex_match(email, email_regex))
    {
        const send_email_response response{ game_error_type::invalid_parameter, "email is invalid" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_timestamp = handle_parameter_.get_param("timestamp");
    if (!optional_timestamp)
    {
        const send_email_response response{ game_error_type::invalid_parameter, "timestamp is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto timestamp = boost::lexical_cast<int64_t>(*optional_timestamp);

    // 检查时间戳，如果请求时间是 15 分钟前的，直接拒绝
    if (const auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        current_time - timestamp > minute * 15)
    {
        const send_email_response response{ game_error_type::timestamp_expired, "timestamp is expired" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_app_id = handle_parameter_.get_param("app_id");
    if (!optional_app_id)
    {
        const send_email_response response{ game_error_type::invalid_parameter, "app id is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_sign = handle_parameter_.get_param("sign");
    if (!optional_sign)
    {
        const send_email_response response{ game_error_type::invalid_parameter, "sign is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto app_id = boost::lexical_cast<int64_t>(*optional_app_id);
    const auto secret = app_secret::get_instance().get_key(app_id);

    if (const auto hmac_sha256 = calculate_hmac_sha256(app_id, email, timestamp, secret);
        hmac_sha256 != *optional_sign)
    {
        const send_email_response response{ game_error_type::sign_error, "sign error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto select = email_limit::get_select(database_type::redis, email);
    if (auto sms_limit = co_await redis_pool->select_one(select, email_limit::get_database_field_container()))
    {
        const send_email_response response{ game_error_type::sent_too_frequently, "sent too frequently" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    email_code email_code{ database_type::redis, email };
    email_code.set_code(random_helper::get_random_int(sms_limit_code_begin, sms_limit_code_end));

    email_limit email_limit{ database_type::redis, email };
    email_limit.set_exist(true);

    co_await redis_pool->execute_changes(email_code.get_modify(), email_code_expiration_time);
    co_await redis_pool->execute_changes(email_limit.get_modify(), email_limit_expiration_time);

    const send_email_response response{ game_error_type::success, "send email success" };
    handle_parameter_.write(response.to_json_string());

    handle_parameter_.submit_task([this,email_code] {
        send_sdk_email(email_code);
    });

    co_return;
}

std::string celeritas::send_email::calculate_hmac_sha256(int64_t app_id, const std::string& email, int64_t timestamp, const std::string& secret_key)
{
    const auto data = std::format("{}{}{}", app_id, email, timestamp);

    return hmac_sha256::calculate(data, secret_key);
}

void celeritas::send_email::send_sdk_email(const email_code& email_code)
{
}