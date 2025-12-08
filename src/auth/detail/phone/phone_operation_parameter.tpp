#pragma once

#include "phone_operation_parameter.h"
#include "auth/core/auth_service_base.h"
#include "database/database_pool_base.h"

template <typename ResponseType>
celeritas::phone_operation_parameter::optional_sms_code_awaitable_type celeritas::phone_operation_parameter::check_code(const database_pool_shared_ptr& redis_pool, auth_service_base& auth_service_base) const
{
    const auto optional_sms_code = co_await redis_pool->select_one(sms_code::get_select(database_type::redis, get_phone()), sms_code::get_database_field_container());
    if (!optional_sms_code)
    {
        auth_service_base.write(ResponseType{ game_error_type::code_expired });

        co_return std::nullopt;
    }

    if (sms_code sms_code{ *optional_sms_code };
        sms_code.get_code() != get_code())
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

    auth_service_base.write(ResponseType{ game_error_type::code_error });

    co_return std::nullopt;
}