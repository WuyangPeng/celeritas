#include "app_secret.h"
#include "phone_bind.h"
#include "phone_bind_response.h"
#include "common/snowflake_generator.h"
#include "config/app_config.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/account_bind.h"
#include "server/account_type.h"
#include "server/game_error_type.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include <regex>

celeritas::phone_bind::phone_bind(http_handle_parameter handle_parameter)
    : handle_parameter_{ std::move(handle_parameter) }
{
}

celeritas::phone_bind::void_awaitable_type celeritas::phone_bind::response()
{
    const auto optional_device_id = handle_parameter_.get_param(account::device_id_describe.data());
    if (!optional_device_id)
    {
        const phone_bind_response response{ game_error_type::invalid_parameter, "device_id is required" };
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

    const auto& device_id = *optional_device_id;
    const auto app_id = boost::lexical_cast<int>(*optional_app_id);
    const auto secret = app_secret::get_instance().get_key(app_id);
    const auto code = boost::lexical_cast<int>(*optional_code);

    if (const auto hmac_sha256 = calculate_hmac_sha256(app_id, phone, device_id, code, timestamp, secret);
        hmac_sha256 != *optional_sign)
    {
        const phone_bind_response response{ game_error_type::sign_error, "sign error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto select = account::get_select(database_type::mysql);
    select->add_key(basis_database{ account::device_id_describe, device_id });
    auto optional_account = co_await mysql_pool->select_one(select, account::get_database_field_container());

    if (!optional_account)
    {
        const phone_bind_response response{ game_error_type::account_error, "account error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    account account{ *optional_account };
    account.set_device_id(account.get_account_name());

    const auto server_config = handle_parameter_.get_app_config()->get_server_config();
    const auto account_bind_id = snowflake_generator::get_instance().generate(server_config.get_datacenter_id(), server_config.get_worker_id());
    account_bind account_bind{ database_type::redis, account_bind_id };
    account_bind.set_account_id(account.get_account_id());
    account_bind.set_auth_key(phone);
    account_bind.set_account_type(static_cast<int>(account_type::phone));
    account_bind.set_is_primary(true);

    if (co_await mysql_pool->execute_changes(account_bind.get_modify()) &&
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

std::string celeritas::phone_bind::calculate_hmac_sha256(int app_id, const std::string& phone, const std::string& device_id, int code, int64_t timestamp, const std::string& secret_key)
{
    const auto data = std::format("{}{}{}{}{}", app_id, phone, device_id, code, timestamp);

    std::array<unsigned char, EVP_MAX_MD_SIZE> result{};
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