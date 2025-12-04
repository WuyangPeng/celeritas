#include "app_secret.h"
#include "guest_login.h"
#include "guest_login_response.h"
#include "common/celeritas_error.h"
#include "config/app_config.h"
#include "database/database_pool_manager.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "database/generated/redis/auth/session_token.h"
#include "detail/guest_login_parameter.h"
#include "message/game_error_type.h"

celeritas::guest_login::guest_login(http_handle_parameter handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::guest_login::void_awaitable_type celeritas::guest_login::response()
{
    guest_login_parameter guest_login_parameter{ get_http_handle_parameter() };

    if (guest_login_parameter.is_failure())
    {
        write(guest_login_parameter.get_response());
        co_return;
    }

    const auto app_id = guest_login_parameter.get_app_id();
    const auto device_id = guest_login_parameter.get_device_id();

    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());
    auto optional_account = co_await get_optional_account(app_id, device_id, mysql_pool);

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    auto account = co_await get_account(app_id, device_id, optional_account, redis_pool, get_app_config());

    if (!account.get_password_hash().empty())
    {
        write(guest_login_response{ game_error_type::no_guest_account });
        co_return;
    }

    // 这里没有删除旧的token，旧的token依赖redis有效时间进行删除。
    if (auto session_token = co_await create_session_token(account, !optional_account, redis_pool))
    {
        write(guest_login_response{ game_error_type::success,
                                    "login successful",
                                    session_token.value().get_token(),
                                    get_app_config()->get_expire_milliseconds(redis_db_name.data()) });
    }
    else
    {
        write(guest_login_response{ game_error_type::redis_error });
    }

    co_return;
}

celeritas::guest_login::account_awaitable_type celeritas::guest_login::get_account(const int64_t app_id,
                                                                                   const std::string& device_id,
                                                                                   const optional_database_entity_change& database_entity_change,
                                                                                   const database_pool_shared_ptr& redis_pool,
                                                                                   const const_app_config_shared_ptr& app_config)
{
    if (database_entity_change)
    {
        account account{ *database_entity_change };

        co_return account;
    }

    co_return co_await create_new_account(app_id, device_id, redis_pool, app_config);
}