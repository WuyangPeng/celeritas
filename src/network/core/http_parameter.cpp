#include "http_parameter.h"
#include "common/common_constant.h"
#include "common/core/time_helper.h"
#include "message/basic/game_error_type.h"
#include "network/network_constant.h"

#include <boost/lexical_cast.hpp>

celeritas::http_parameter::http_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : http_handle_parameter_{ http_handle_parameter }, app_id_{}, sign_{}, timestamp_{}
{
}

int64_t celeritas::http_parameter::get_app_id() const
{
    return app_id_;
}

std::string celeritas::http_parameter::get_sign() const
{
    return sign_;
}

int64_t celeritas::http_parameter::get_timestamp() const
{
    return timestamp_;
}

celeritas::http_parameter::optional_http_response celeritas::http_parameter::get_http_parameter()
{
    const auto optional_app_id = http_handle_parameter_->get_param(app_id_describe.data());
    if (!optional_app_id)
    {
        return http_response{ game_error_type::invalid_parameter, "app id is required" };
    }

    const auto optional_timestamp = http_handle_parameter_->get_param(timestamp_describe.data());
    if (!optional_timestamp)
    {
        return http_response{ game_error_type::invalid_parameter, "timestamp is required" };
    }

    const auto optional_sign = http_handle_parameter_->get_param(sign_describe.data());
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

celeritas::http_parameter::optional_string celeritas::http_parameter::get_param(const std::string& key) const
{
    return http_handle_parameter_->get_param(key);
}

celeritas::http_parameter::optional_string celeritas::http_parameter::get_param(const std::string_view key) const
{
    return http_handle_parameter_->get_param(key.data());
}

celeritas::http_parameter::optional_string celeritas::http_parameter::get_param(const char* key) const
{
    return http_handle_parameter_->get_param(key);
}
