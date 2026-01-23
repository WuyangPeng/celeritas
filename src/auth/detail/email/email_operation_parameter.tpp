#pragma once

#include "email_operation_parameter.h"
#include "auth/auth_constant.h"
#include "auth/core/auth_service_base.h"
#include "database/pool/database_pool_base.h"

template <typename ResponseType>
celeritas::email_operation_parameter::optional_email_code_awaitable_type celeritas::email_operation_parameter::check_code(const database_pool_shared_ptr& redis_pool, auth_service_base& auth_service_base) const
{
    const auto optional_email_code = co_await redis_pool->select_one(email_code::get_select(database_type::redis, get_email()), email_code::get_database_field_container());
    if (!optional_email_code)
    {
        co_await auth_service_base.write_immediately(ResponseType{ game_error_type::code_expired });

        co_return std::nullopt;
    }

    if (email_code email_code{ *optional_email_code };
        email_code.get_code() != get_code())
    {
        email_code.modify_retry_count(1);

        if (email_code.get_retry_count() >= email_code_retry_count)
        {
            co_await redis_pool->execute_changes(email_code.get_delete());
        }
        else
        {
            co_await redis_pool->execute_changes(email_code.get_modify());
        }

        co_return email_code;
    }

    co_await auth_service_base.write_immediately(ResponseType{ game_error_type::code_error });

    co_return std::nullopt;
}