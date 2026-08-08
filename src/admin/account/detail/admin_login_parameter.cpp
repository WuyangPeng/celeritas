// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-04

#include "admin_login_parameter.h"
#include "admin/admin_constant.h"
#include "auth/config/app_secret.h"
#include "common/core/hmac_sha_256.h"
#include "message/basic/game_error_type.h"

celeritas::admin_login_parameter::admin_login_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, username_{}, password_{}
{
    init();
}

bool celeritas::admin_login_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::http_response celeritas::admin_login_parameter::get_response() const
{
    return response_.value();
}

const std::string& celeritas::admin_login_parameter::get_username() const noexcept
{
    return username_;
}

const std::string& celeritas::admin_login_parameter::get_password() const noexcept
{
    return password_;
}

void celeritas::admin_login_parameter::init_required_parameter()
{
    const auto optional_username = get_param(admin_username_describe);
    const auto optional_password = get_param(admin_password_describe);

    if (!optional_username ||
        optional_username->empty() ||
        !optional_password ||
        optional_password->empty())
    {
        response_ = http_response{ game_error_type::invalid_parameter, "用户名或密码不能为空" };
        return;
    }

    username_ = *optional_username;
    password_ = *optional_password;
}

void celeritas::admin_login_parameter::verify_sign()
{
    if (const auto http_response = get_http_parameter())
    {
        response_ = *http_response;
        return;
    }

    const auto secret = app_secret::get_instance().get_key(get_app_id());

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(secret,
                                                                  get_app_id(),
                                                                  get_timestamp(),
                                                                  username_,
                                                                  password_);
        hmac_sha256 != get_sign())
    {
        response_ = http_response{ game_error_type::sign_error };
    }
}
