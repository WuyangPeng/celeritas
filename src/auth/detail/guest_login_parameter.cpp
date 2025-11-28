#include "guest_login_parameter.h"
#include "auth/app_secret.h"
#include "auth/auth_fwd.h"
#include "boost/lexical_cast.hpp"
#include "common/hmac_sha_256.h"
#include "common/time_helper.h"
#include "database/generated/mysql/auth/account.h"

celeritas::guest_login_parameter::guest_login_parameter(const http_handle_parameter& http_handle_parameter)
    : http_handle_parameter_{ http_handle_parameter }, response_{}, device_id_{}, app_id_{}, sign_{}, timestamp_{}
{
    init();
}

std::string celeritas::guest_login_parameter::get_device_id() const
{
    return device_id_;
}

int64_t celeritas::guest_login_parameter::get_app_id() const
{
    return app_id_;
}

bool celeritas::guest_login_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::guest_login_response celeritas::guest_login_parameter::get_response() const
{
    return response_.value();
}

void celeritas::guest_login_parameter::init()
{
    const auto optional_device_id = http_handle_parameter_.get_param(account::device_id_describe.data());
    if (!optional_device_id)
    {
        response_ = guest_login_response{ game_error_type::invalid_parameter, "device_id is required" };
        return;
    }

    const auto optional_app_id = http_handle_parameter_.get_param("app_id");
    if (!optional_app_id)
    {
        response_ = guest_login_response{ game_error_type::invalid_parameter, "app_id is required" };
        return;
    }

    const auto optional_timestamp = http_handle_parameter_.get_param("timestamp");
    if (!optional_timestamp)
    {
        response_ = guest_login_response{ game_error_type::invalid_parameter, "timestamp is required" };

        return;
    }

    const auto optional_sign = http_handle_parameter_.get_param("sign");
    if (!optional_sign)
    {
        response_ = guest_login_response{ game_error_type::invalid_parameter, "sign is required" };

        return;
    }

    device_id_ = *optional_device_id;
    app_id_ = boost::lexical_cast<int64_t>(*optional_app_id);
    const auto secret = app_secret::get_instance().get_key(app_id_);
    timestamp_ = boost::lexical_cast<int64_t>(*optional_timestamp);

    // 检查时间戳，如果请求时间是 5 分钟前的，直接拒绝
    if (const auto current_time = time_helper::get_current_milliseconds();
        current_time - timestamp_ > http_request_timestamp_expired)
    {
        response_ = guest_login_response{ game_error_type::timestamp_expired, "timestamp is expired" };

        return;
    }

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(secret, app_id_, device_id_, timestamp_);
        hmac_sha256 != *optional_sign)
    {
        response_ = guest_login_response{ game_error_type::sign_error, "sign error" };
    }
}