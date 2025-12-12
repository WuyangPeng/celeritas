#include "password_login.h"
#include "password_login_response.h"
#include "auth/authentication/sdk/sdk_process_type.h"
#include "auth/data/app_secret.h"
#include "auth/detail/password/password_login_parameter.h"
#include "common/hmac_sha_256.h"
#include "config/app_config.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "database/generated/redis/auth/session_token.h"
#include "initializer/account_type.h"
#include "message/game_error_type.h"

celeritas::password_login::password_login(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::password_login::void_awaitable_type celeritas::password_login::response()
{
    const password_login_parameter password_login_parameter{ get_http_handle_parameter() };

    if (password_login_parameter.is_failure())
    {
        co_return co_await write_immediately(password_login_parameter.get_response());
    }

    const auto app_id = password_login_parameter.get_app_id();
    const auto auth_key = password_login_parameter.get_account();
    const auto password = password_login_parameter.get_password();

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());
    const auto key = std::make_shared<basis_database_container>(basis_database_container::object_container{ { account_bind::account_type_describe, static_cast<int>(account_type::password) },
                                                                                                            { account_bind::process_type_describe, static_cast<int>(sdk_process_type::null) },
                                                                                                            { account_bind::auth_key_describe, auth_key },
                                                                                                            { account_bind::app_id_describe, app_id }
    });

    const auto optional_account_bind = co_await mysql_pool->select_one(account_bind::get_select(database_type::mysql, key), account_bind::get_database_field_container());

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());

    co_return co_await login(optional_account_bind, redis_pool, mysql_pool, app_id, auth_key, password);
}

celeritas::password_login::void_awaitable_type celeritas::password_login::login(const optional_database_entity_change& database_entity_change,
                                                                                const database_pool_shared_ptr& redis_pool,
                                                                                const database_pool_shared_ptr& mysql_pool,
                                                                                int64_t app_id,
                                                                                const std::string& auth_key,
                                                                                const std::string& password)
{
    if (database_entity_change)
    {
        account_bind account_bind{ *database_entity_change };

        if (auto optional_account = co_await mysql_pool->select_one(account::get_select(database_type::mysql, account_bind.get_account_id()),
                                                                    account_bind::get_database_field_container()))
        {
            account account{ *optional_account };
            if (account.get_salt().empty())
            {
                co_return co_await write_immediately(password_login_response{ game_error_type::password_error });
            }

            if (hmac_sha256::calculate(password, account.get_salt()) != account.get_password_hash())
            {
                co_return co_await write_immediately(password_login_response{ game_error_type::password_error });
            }

            co_return co_await login(redis_pool, account, false);
        }
    }

    const auto account = co_await create_new_account(app_id, auth_key, password, account_type::password, sdk_process_type::null, "password", redis_pool, get_app_config());

    co_return co_await login(redis_pool, account, true);
}

celeritas::password_login::void_awaitable_type celeritas::password_login::login(const database_pool_shared_ptr& redis_pool, const account& account, const bool is_new_account)
{
    if (const auto session_token = co_await create_session_token(account, is_new_account, redis_pool))
    {
        co_return co_await write_immediately(password_login_response{ game_error_type::success,
                                                                      "login successful",
                                                                      session_token->get_token(),
                                                                      get_app_config()->get_expire_milliseconds(redis_db_name.data()) });
    }
    else
    {
        co_return co_await write_immediately(password_login_response{ game_error_type::redis_error });
    }
}