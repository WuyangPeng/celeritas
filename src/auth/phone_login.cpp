#include "app_secret.h"
#include "auth_fwd.h"
#include "phone_login.h"
#include "phone_login_response.h"
#include "common/celeritas_error.h"
#include "config/app_config.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "database/generated/redis/auth/session_token.h"
#include "database/generated/redis/auth/sms_code.h"
#include "detail/phone_login_parameter.h"
#include "message/game_error_type.h"
#include "server/account_type.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <openssl/evp.h>

#include <regex>

celeritas::phone_login::phone_login(http_handle_parameter handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::phone_login::void_awaitable_type celeritas::phone_login::response()
{
    phone_login_parameter phone_login_parameter{ get_http_handle_parameter() };

    if (phone_login_parameter.is_failure())
    {
        write(phone_login_parameter.get_response());
        co_return;
    }

    const auto app_id = phone_login_parameter.get_app_id();
    const auto code = phone_login_parameter.get_code();
    const auto phone = phone_login_parameter.get_phone();

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto optional_sms_code = co_await check_code(code, phone, redis_pool);
    if (!optional_sms_code)
    {
        write(phone_login_response{ game_error_type::code_error });
        co_return;
    }

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(auth_db_name.data());
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account_bind::account_type_describe, static_cast<int>(account_type::phone) },
                                                                                                            { account_bind::auth_key_describe, phone },
                                                                                                            { account_bind::app_id_describe, app_id } });
    const auto select = account_bind::get_select(database_type::mysql, key);
    auto optional_account_bind = co_await mysql_pool->select_one(select, account_bind::get_database_field_container());

    auto account = co_await get_account(optional_account_bind, redis_pool, mysql_pool, app_id, phone, get_app_config());

    // 这里没有删除旧的token，旧的token依赖redis有效时间进行删除。
    if (auto session_token = co_await create_session_token(account, !optional_account_bind, redis_pool))
    {
        write(phone_login_response{ game_error_type::success,
                                    "login successful",
                                    session_token.value().get_token(),
                                    get_app_config()->get_expire_milliseconds(redis_db_name.data()) });
    }
    else
    {
        write(phone_login_response{ game_error_type::redis_error });
    }

    co_await redis_pool->execute_changes(optional_sms_code->get_delete());

    co_return;
}

celeritas::phone_login::account_awaitable_type celeritas::phone_login::get_account(const optional_basis_database_manager& basis_database_manager,
                                                                                   const database_pool_shared_ptr& redis_pool,
                                                                                   const database_pool_shared_ptr& mysql_pool,
                                                                                   const int64_t app_id,
                                                                                   const std::string& phone,
                                                                                   const const_app_config_shared_ptr& app_config)
{
    if (basis_database_manager)
    {
        const account_bind account_bind{ *basis_database_manager };

        if (const auto optional_account = co_await mysql_pool->select_one(account::get_select(database_type::mysql, account_bind.get_account_id()), account::get_database_field_container()))
        {
            account account{ *optional_account };

            co_return account;
        }
    }

    co_return co_await create_new_account(app_id, phone, account_type::phone, "phone", redis_pool, app_config);
}

celeritas::phone_login::optional_sms_code_awaitable_type celeritas::phone_login::check_code(const int code,
                                                                                            const std::string& phone,
                                                                                            const database_pool_shared_ptr& redis_pool) const
{
    const auto sms_code_select = sms_code::get_select(database_type::redis, phone);
    auto optional_sms_code = co_await redis_pool->select_one(sms_code_select, sms_code::get_database_field_container());
    if (!optional_sms_code)
    {
        write(phone_login_response{ game_error_type::code_expired });

        co_return std::nullopt;
    }

    sms_code sms_code{ *optional_sms_code };
    if (sms_code.get_code() != code)
    {
        sms_code.modify_retry_count(1);

        if (sms_code.get_retry_count() >= sms_code_retry_count)
        {
            co_await redis_pool->execute_changes(sms_code.get_delete());
        }
        else
        {
            co_await redis_pool->execute_changes(sms_code.get_modify());
        }

        co_return sms_code;
    }

    co_return std::nullopt;
}