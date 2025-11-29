#include "auth_internal_fwd.h"
#include "phone_operation_parameter.h"
#include "auth/auth_service_base.h"
#include "auth/phone_login_response.h"
#include "database/database_pool_base.h"

#include <boost/lexical_cast.hpp>

celeritas::phone_operation_parameter::phone_operation_parameter(const http_handle_parameter& http_handle_parameter)
    : base_type{ http_handle_parameter }, code_{}
{
}

int celeritas::phone_operation_parameter::get_code() const
{
    return code_;
}

celeritas::auth_parameter::optional_http_response celeritas::phone_operation_parameter::get_http_parameter()
{
    if (const auto http_response = base_type::get_http_parameter())
    {
        return http_response;
    }

    const auto optional_code = get_param(code_describe.data());
    if (!optional_code)
    {
        return http_response{ game_error_type::invalid_parameter, "code is required" };
    }

    code_ = boost::lexical_cast<int>(*optional_code);

    return std::nullopt;
}

celeritas::phone_operation_parameter::optional_sms_code_awaitable_type celeritas::phone_operation_parameter::check_code(const database_pool_shared_ptr& redis_pool, auth_service_base& auth_service_base) const
{
    auto optional_sms_code = co_await redis_pool->select_one(sms_code::get_select(database_type::redis, get_phone()), sms_code::get_database_field_container());
    if (!optional_sms_code)
    {
        auth_service_base.write(phone_login_response{ game_error_type::code_expired });

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

    auth_service_base.write(phone_login_response{ game_error_type::code_error });

    co_return std::nullopt;
}