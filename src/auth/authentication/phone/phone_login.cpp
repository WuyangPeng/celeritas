#include "phone_login.h"
#include "phone_login_response.h"
#include "auth/authentication/sdk/sdk_process_type.h"
#include "auth/data/app_secret.h"
#include "auth/detail/phone/phone_login_parameter.h"
#include "auth/detail/phone/phone_operation_parameter.tpp"
#include "common/core/celeritas_error.h"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"
#include "database/pool/database_pool_manager.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "database/generated/redis/auth/session_token.h"
#include "database/generated/redis/auth/sms_code.h"
#include "initializer/account_type.h"
#include "message/basic/game_error_type.h"

celeritas::phone_login::phone_login(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::phone_login::void_awaitable_type celeritas::phone_login::response()
{
    phone_login_parameter phone_login_parameter{ get_http_handle_parameter() };

    if (phone_login_parameter.is_failure())
    {
        co_return co_await write_immediately(phone_login_parameter.get_response());
    }

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto optional_sms_code = co_await phone_login_parameter.check_code<phone_login_response>(redis_pool, *this);
    if (!optional_sms_code)
    {
        co_return;
    }

    const auto app_id = phone_login_parameter.get_app_id();
    const auto phone = phone_login_parameter.get_phone();

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account_bind::account_type_describe, static_cast<int>(account_type::phone) },
                                                                                                            { account_bind::process_type_describe, static_cast<int>(sdk_process_type::null) },
                                                                                                            { account_bind::auth_key_describe, phone },
                                                                                                            { account_bind::app_id_describe, app_id } });

    auto optional_account_bind = co_await mysql_pool->select_one(account_bind::get_select(database_type::mysql, key),
                                                                 account_bind::get_database_field_container());

    auto account = co_await get_account(optional_account_bind, redis_pool, mysql_pool, app_id, phone, get_app_config());

    // 这里没有删除旧的token，旧的token依赖redis有效时间进行删除。
    if (auto session_token = co_await create_session_token(account, !optional_account_bind, redis_pool))
    {
        co_await write_immediately(phone_login_response{ game_error_type::success,
                                                         "login successful",
                                                         session_token->get_token(),
                                                         get_app_config()->get_expire_milliseconds(redis_db_name.data()) });
    }
    else
    {
        co_return co_await write_immediately(phone_login_response{ game_error_type::redis_error });
    }

    if (!co_await redis_pool->execute_changes(optional_sms_code->get_delete()))
    {
        LOG_CHANNEL(auth_channel, error) << "delete sms code error.";
    }

    co_return;
}

celeritas::phone_login::account_awaitable_type celeritas::phone_login::get_account(const optional_database_entity_change& database_entity_change,
                                                                                   const database_pool_shared_ptr& redis_pool,
                                                                                   const database_pool_shared_ptr& mysql_pool,
                                                                                   const int64_t app_id,
                                                                                   const std::string& phone,
                                                                                   const const_app_config_shared_ptr& app_config)
{
    if (database_entity_change)
    {
        const account_bind account_bind{ *database_entity_change };

        if (const auto optional_account = co_await mysql_pool->select_one(account::get_select(database_type::mysql, account_bind.get_account_id()),
                                                                          account::get_database_field_container()))
        {
            account account{ *optional_account };

            co_return account;
        }
    }

    co_return co_await create_new_account(app_id, phone, account_type::phone, sdk_process_type::null, "phone", redis_pool, app_config);
}