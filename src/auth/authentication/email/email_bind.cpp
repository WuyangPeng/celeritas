#include "email_bind.h"
#include "email_bind_response.h"
#include "auth/core/auth_bind.tpp"
#include "auth/data/app_secret.h"
#include "auth/detail/email/email_bind_parameter.h"
#include "auth/detail/email/email_operation_parameter.tpp"
#include "common/logging/logger.h"
#include "config/aggregate/app_config.h"
#include "database/pool/database_pool_manager.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "initializer/account_type.h"
#include "message/basic/game_error_type.h"

celeritas::email_bind::email_bind(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::email_bind::void_awaitable_type celeritas::email_bind::response()
{
    email_bind_parameter email_bind_parameter{ get_http_handle_parameter() };

    if (email_bind_parameter.is_failure())
    {
        co_return co_await write_immediately(email_bind_parameter.get_response());
    }

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto optional_email_code = co_await email_bind_parameter.check_code<email_bind_response>(redis_pool, *this);
    if (!optional_email_code)
    {
        co_return;
    }

    const auto app_id = email_bind_parameter.get_app_id();
    const auto email = email_bind_parameter.get_email();
    const auto token = email_bind_parameter.get_token();
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    auto optional_account = co_await get_account<email_bind_response>(app_id, email, token, account_type::email, redis_pool, mysql_pool);

    if (auto account = *optional_account;
        co_await bind(account, app_id, email, account_type::email, sdk_process_type::null, mysql_pool))
    {
        co_return co_await write_immediately(email_bind_response{ game_error_type::success, "email bind success" });

        if (!co_await redis_pool->execute_changes(optional_email_code->get_delete()))
        {
            LOG_CHANNEL(auth_channel, error) << "delete email code error.";
        }
    }
    else
    {
        co_return co_await write_immediately(email_bind_response{ game_error_type::mysql_error });
    }
}