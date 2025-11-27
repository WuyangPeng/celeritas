#include "app_secret.h"
#include "auth_fwd.h"
#include "phone_login.h"
#include "phone_login_response.h"
#include "common/celeritas_error.h"
#include "common/snowflake_generator.h"
#include "config/app_config.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "database/generated/redis/auth/session_token.h"
#include "database/generated/redis/auth/sms_code.h"
#include "server/account_status_type.h"
#include "server/account_type.h"
#include "server/game_error_type.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <openssl/evp.h>
#include <openssl/hmac.h>

#include <regex>

celeritas::phone_login::phone_login(http_handle_parameter handle_parameter)
    : handle_parameter_{ std::move(handle_parameter) }
{
}

celeritas::phone_login::void_awaitable_type celeritas::phone_login::response()
{
    const auto optional_phone = handle_parameter_.get_param("phone");
    if (!optional_phone)
    {
        const phone_login_response response{ game_error_type::invalid_parameter, "phone is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto& phone = *optional_phone;
    if (const std::regex phone_regex(R"(^1\d{10}$)");
        !std::regex_match(phone, phone_regex))
    {
        const phone_login_response response{ game_error_type::invalid_parameter, "phone is invalid" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_timestamp = handle_parameter_.get_param("timestamp");
    if (!optional_timestamp)
    {
        const phone_login_response response{ game_error_type::invalid_parameter, "timestamp is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto timestamp = boost::lexical_cast<int64_t>(*optional_timestamp);
    const auto current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    // 检查时间戳，如果请求时间是 5 分钟前的，直接拒绝
    if (current_time - timestamp > minute * 5)
    {
        const phone_login_response response{ game_error_type::timestamp_expired, "timestamp is expired" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_app_id = handle_parameter_.get_param("app_id");
    if (!optional_app_id)
    {
        const phone_login_response response{ game_error_type::invalid_parameter, "app_id is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_sign = handle_parameter_.get_param("sign");
    if (!optional_sign)
    {
        const phone_login_response response{ game_error_type::invalid_parameter, "sign is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto optional_code = handle_parameter_.get_param("code");
    if (!optional_code)
    {
        const phone_login_response response{ game_error_type::invalid_parameter, "code is required" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto app_id = boost::lexical_cast<int64_t>(*optional_app_id);
    const auto secret = app_secret::get_instance().get_key(app_id);
    const auto code = boost::lexical_cast<int>(*optional_code);

    if (const auto hmac_sha256 = calculate_hmac_sha256(app_id, phone, code, timestamp, secret);
        hmac_sha256 != *optional_sign)
    {
        const phone_login_response response{ game_error_type::sign_error, "sign error" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto sms_code_select = sms_code::get_select(database_type::redis, phone);
    auto optional_sms_code = co_await redis_pool->select_one(sms_code_select, sms_code::get_database_field_container());
    if (!optional_sms_code)
    {
        const phone_login_response response{ game_error_type::code_expired, "code is expired" };
        handle_parameter_.write(response.to_json_string());

        co_return;
    }

    sms_code sms_code{ *optional_sms_code };
    if (sms_code.get_code() != code)
    {
        const phone_login_response response{ game_error_type::code_error, "code is error" };
        handle_parameter_.write(response.to_json_string());

        sms_code.modify_retry_count(1);

        if (sms_code.get_retry_count() >= sms_code_retry_count)
        {
            co_await redis_pool->execute_changes(sms_code.get_delete());
        }
        else
        {
            co_await redis_pool->execute_changes(sms_code.get_modify());
        }

        co_return;
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account_bind::account_type_describe, static_cast<int>(account_type::phone) },
                                                                                                            { account_bind::auth_key_describe, phone },
                                                                                                            { account_bind::app_id_describe, app_id } });
    const auto select = account_bind::get_select(database_type::mysql, key);
    auto optional_account_bind = co_await mysql_pool->select_one(select, account_bind::get_database_field_container());

    auto account = co_await get_account(optional_account_bind, redis_pool, mysql_pool, app_id, phone, handle_parameter_.get_app_config());

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

        const phone_login_response response{ game_error_type::success, "login successful", token, expire_milliseconds };
        handle_parameter_.write(response.to_json_string());
    }
    else
    {
        const phone_login_response response{ game_error_type::redis_error, "redis error" };
        handle_parameter_.write(response.to_json_string());
    }

    co_await redis_pool->execute_changes(sms_code.get_delete());

    co_return;
}

std::string celeritas::phone_login::calculate_hmac_sha256(int64_t app_id, const std::string& phone, int code, int64_t timestamp, const std::string& secret_key)
{
    const auto data = std::format("{}{}{}{}", app_id, phone, code, timestamp);

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

std::string celeritas::phone_login::generate_token()
{
    boost::uuids::random_generator generator{};
    const auto uuid = generator();

    return boost::uuids::to_string(uuid);
}

celeritas::phone_login::account_awaitable_type celeritas::phone_login::get_account(const optional_basis_database_manager& basis_database_manager,
                                                                                   const database_pool_shared_ptr& redis_pool,
                                                                                   const database_pool_shared_ptr& mysql_pool,
                                                                                   int64_t app_id,
                                                                                   const std::string& phone,
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
    account.set_account_name("phone_" + std::to_string(account_id));
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
    account_bind.set_auth_key(phone);
    account_bind.set_account_type(static_cast<int>(account_type::phone));
    account_bind.set_app_id(app_id);

    if (co_await redis_pool->execute_changes(account_bind.get_modify()))
    {
        co_return account;
    }

    throw celeritas_error("phone login error");
}