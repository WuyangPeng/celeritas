#include "phone_bind.h"
#include "phone_bind_response.h"
#include "auth/core/auth_bind.tpp"
#include "auth/detail/phone/phone_bind_parameter.h"
#include "auth/detail/phone/phone_operation_parameter.tpp"
#include "common/logging/logger.h"
#include "common/core/snowflake_generator.h"
#include "config/aggregate/app_config.h"
#include "database/database_constant.h"
#include "database/pool/database_pool_manager.h"
#include "database/generated/mysql/auth/account.h"
#include "database/generated/mysql/auth/account_bind.h"
#include "initializer/account_type.h"
#include "message/basic/game_error_type.h"

celeritas::phone_bind::phone_bind(http_handle_parameter_shared_ptr handle_parameter)
    : base_type{ std::move(handle_parameter) }
{
}

celeritas::phone_bind::void_awaitable_type celeritas::phone_bind::response()
{
    phone_bind_parameter phone_bind_parameter{ get_http_handle_parameter() };

    if (phone_bind_parameter.is_failure())
    {
        co_return co_await write_immediately(phone_bind_parameter.get_response());
    }

    const auto redis_pool = database_pool_manager::get_instance().get_pool(redis_db_name.data());
    const auto optional_sms_code = co_await phone_bind_parameter.check_code<phone_bind_response>(redis_pool, *this);
    if (!optional_sms_code)
    {
        co_return;
    }

    const auto app_id = phone_bind_parameter.get_app_id();
    const auto phone = phone_bind_parameter.get_phone();
    const auto token = phone_bind_parameter.get_token();
    const auto mysql_pool = database_pool_manager::get_instance().get_pool(mysql_auth_db_name.data());

    auto optional_account = co_await get_account<phone_bind_response>(app_id, phone, token, account_type::phone, redis_pool, mysql_pool);

    if (auto account = *optional_account;
        co_await bind(account, app_id, phone, account_type::phone, sdk_process_type::null, mysql_pool))
    {
        co_return co_await write_immediately(phone_bind_response{ game_error_type::success, "phone bind success" });

        if (!co_await redis_pool->execute_changes(optional_sms_code->get_delete()))
        {
            LOG_CHANNEL(auth_channel, error) << "delete sms code error.";
        }
    }
    else
    {
        co_return co_await write_immediately(phone_bind_response{ game_error_type::mysql_error });
    }
}