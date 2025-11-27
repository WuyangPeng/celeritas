#include "app_secret.h"
#include "auth_fwd.h"
#include "send_sms.h"
#include "send_sms_response.h"
#include "common/hmac_sha_256.h"
#include "common/random_helper.h"
#include "database/database_pool_manager.h"
#include "database/generated/redis/auth/sms_code.h"
#include "database/generated/redis/auth/sms_limit.h"
#include "../message/game_error_type.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include <regex>

celeritas::send_sms::send_sms(http_handle_parameter handle_parameter)
    : handle_parameter_{ std::move(handle_parameter) }
{
}

celeritas::send_sms::void_awaitable_type celeritas::send_sms::response()
{
    const auto optional_phone = handle_parameter_.get_param("phone");
    if (!optional_phone)
    {
        const send_sms_response response{ game_error_type::invalid_parameter, "phone is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto& phone = *optional_phone;
    if (const std::regex phone_regex{ R"(^1\d{10}$)" };
        !std::regex_match(phone, phone_regex))
    {
        const send_sms_response response{ game_error_type::invalid_parameter, "phone is invalid" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_timestamp = handle_parameter_.get_param("timestamp");
    if (!optional_timestamp)
    {
        const send_sms_response response{ game_error_type::invalid_parameter, "timestamp is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto timestamp = boost::lexical_cast<int64_t>(*optional_timestamp);

    // 检查时间戳，如果请求时间是 5 分钟前的，直接拒绝
    if (const auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        current_time - timestamp > minute * 5)
    {
        const send_sms_response response{ game_error_type::timestamp_expired, "timestamp is expired" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_app_id = handle_parameter_.get_param("app_id");
    if (!optional_app_id)
    {
        const send_sms_response response{ game_error_type::invalid_parameter, "app_id is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_sign = handle_parameter_.get_param("sign");
    if (!optional_sign)
    {
        const send_sms_response response{ game_error_type::invalid_parameter, "sign is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto app_id = boost::lexical_cast<int64_t>(*optional_app_id);
    const auto secret = app_secret::get_instance().get_key(app_id);

    if (const auto hmac_sha256 = calculate_hmac_sha256(app_id, phone, timestamp, secret);
        hmac_sha256 != *optional_sign)
    {
        const send_sms_response response{ game_error_type::sign_error, "sign error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto select = sms_limit::get_select(database_type::redis, phone);
    if (auto sms_limit = co_await redis_pool->select_one(select, sms_limit::get_database_field_container()))
    {
        const send_sms_response response{ game_error_type::sent_too_frequently, "sent too frequently" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    sms_code sms_code{ database_type::redis, phone };
    sms_code.set_code(random_helper::get_random_int(sms_limit_code_begin, sms_limit_code_end));

    sms_limit sms_limit{ database_type::redis, phone };
    sms_limit.set_exist(true);

    co_await redis_pool->execute_changes(sms_code.get_modify(), sms_code_expiration_time);
    co_await redis_pool->execute_changes(sms_limit.get_modify(), sms_limit_expiration_time);

    const send_sms_response response{ game_error_type::success, "send sms success" };
    handle_parameter_.write(response.to_json_string());

    handle_parameter_.submit_task([this,sms_code] {
        send_sdk_sms(sms_code);
    });

    co_return;
}

std::string celeritas::send_sms::calculate_hmac_sha256(int64_t app_id, const std::string& phone, int64_t timestamp, const std::string& secret_key)
{
    const auto data = std::format("{}{}{}", app_id, phone, timestamp);

    return hmac_sha256::calculate(data, secret_key);
}

void celeritas::send_sms::send_sdk_sms(const sms_code& sms_code)
{
}