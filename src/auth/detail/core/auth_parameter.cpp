#include "auth_parameter.h"
#include "auth/detail/auth_internal_fwd.h"
#include "common/hmac_sha_256.h"
#include "common/time_helper.h"
#include "database/generated/mysql/auth/account.h"

#include <boost/lexical_cast.hpp>

celeritas::auth_parameter::auth_parameter(const http_handle_parameter& http_handle_parameter)
    : http_handle_parameter_{ http_handle_parameter }, app_id_{}, sign_{}, timestamp_{}
{
}

int64_t celeritas::auth_parameter::get_app_id() const
{
    return app_id_;
}

std::string celeritas::auth_parameter::get_sign() const
{
    return sign_;
}

int64_t celeritas::auth_parameter::get_timestamp() const
{
    return timestamp_;
}

celeritas::auth_parameter::optional_http_response celeritas::auth_parameter::get_http_parameter()
{
    const auto optional_app_id = http_handle_parameter_.get_param(account::app_id_describe.data());
    if (!optional_app_id)
    {
        return http_response{ game_error_type::invalid_parameter, "app id is required" };
    }

    const auto optional_timestamp = http_handle_parameter_.get_param(timestamp_describe.data());
    if (!optional_timestamp)
    {
        return http_response{ game_error_type::invalid_parameter, "timestamp is required" };
    }

    const auto optional_sign = http_handle_parameter_.get_param(sign_describe.data());
    if (!optional_sign)
    {
        return http_response{ game_error_type::invalid_parameter, "sign is required" };
    }

    app_id_ = boost::lexical_cast<int64_t>(*optional_app_id);
    timestamp_ = boost::lexical_cast<int64_t>(*optional_timestamp);
    sign_ = *optional_sign;

    // 检查时间戳，如果请求时间是 5 分钟前的，直接拒绝
    if (const auto current_time = time_helper::get_current_milliseconds();
        current_time - timestamp_ > http_request_timestamp_expired)
    {
        return http_response{ game_error_type::timestamp_expired, "timestamp is expired" };
    }

    return std::nullopt;
}

celeritas::auth_parameter::optional_string celeritas::auth_parameter::get_param(const std::string& key) const
{
    return http_handle_parameter_.get_param(key);
}