#include "email_login.h"
#include "email_login_response.h"
#include "auth/authentication/sdk/sdk_process_type.h"
#include "auth/data/app_secret.h"
#include "auth/detail/email/email_login_parameter.h"
#include "auth/detail/email/email_operation_parameter.tpp"
#include "common/core/celeritas_error.h"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "database/generated/redis/auth/email_code.h"
#include "database/generated/redis/auth/session_token.h"
#include "initializer/account_type.h"
#include "message/game_error_type.h"

celeritas::email_login::email_login(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::email_login::void_awaitable_type celeritas::email_login::response()
{
    email_login_parameter email_login_parameter{ get_http_handle_parameter() };

    if (email_login_parameter.is_failure())
    {
        co_return co_await write_immediately(email_login_parameter.get_response());
    }

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto optional_email_code = co_await email_login_parameter.check_code<email_login_response>(redis_pool, *this);
    if (!optional_email_code)
    {
        co_return;
    }

    const auto email = email_login_parameter.get_email();
    const auto app_id = email_login_parameter.get_app_id();

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account_bind::account_type_describe, static_cast<int>(account_type::email) },
                                                                                                            { account_bind::process_type_describe, static_cast<int>(sdk_process_type::null) },
                                                                                                            { account_bind::auth_key_describe, email },
                                                                                                            { account_bind::app_id_describe, app_id } });

    auto optional_account_bind = co_await mysql_pool->select_one(account_bind::get_select(database_type::mysql, key), account_bind::get_database_field_container());

    auto account = co_await get_account(optional_account_bind, redis_pool, mysql_pool, app_id, email, get_app_config());

    // 这里没有删除旧的token，旧的token依赖redis有效时间进行删除。
    if (auto session_token = co_await create_session_token(account, !optional_account_bind, redis_pool))
    {
        co_return co_await write_immediately(email_login_response{ game_error_type::success,
                                                                   "login successful",
                                                                   session_token->get_token(),
                                                                   get_app_config()->get_expire_milliseconds(redis_db_name.data()) });
    }
    else
    {
        co_return co_await write_immediately(email_login_response{ game_error_type::redis_error });
    }

    if (!co_await redis_pool->execute_changes(optional_email_code->get_delete()))
    {
        LOG_CHANNEL(auth_channel, error) << "delete email code error.";
    }

    co_return;
}

celeritas::email_login::account_awaitable_type celeritas::email_login::get_account(const optional_database_entity_change& database_entity_change,
                                                                                   const database_pool_shared_ptr& redis_pool,
                                                                                   const database_pool_shared_ptr& mysql_pool,
                                                                                   const int64_t app_id,
                                                                                   const std::string& email,
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

    co_return co_await create_new_account(app_id, email, account_type::email, sdk_process_type::null, "email", redis_pool, app_config);
}