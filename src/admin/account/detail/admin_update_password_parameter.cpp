// 创建时间：2026-07-30
// 修改时间：2026-07-30
// 审核时间：2026-08-04

#include "admin_update_password_parameter.h"
#include "admin/admin_constant.h"
#include "auth/config/app_secret.h"
#include "common/core/hmac_sha_256.h"
#include "message/basic/game_error_type.h"

celeritas::admin_update_password_parameter::admin_update_password_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, target_id_{ 0 }, password_{}, token_{}
{
    init();
}

bool celeritas::admin_update_password_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::http_response celeritas::admin_update_password_parameter::get_response() const
{
    return response_.value();
}

int64_t celeritas::admin_update_password_parameter::get_target_id() const noexcept
{
    return target_id_;
}

const std::string& celeritas::admin_update_password_parameter::get_password() const noexcept
{
    return password_;
}

const std::string& celeritas::admin_update_password_parameter::get_token() const noexcept
{
    return token_;
}

void celeritas::admin_update_password_parameter::init_required_parameter()
{
    const auto optional_target_id = get_param(admin_account_id_describe);
    if (!optional_target_id || optional_target_id->empty())
    {
        response_ = http_response{ game_error_type::invalid_parameter, "管理员账号ID不能为空" };
        return;
    }

    target_id_ = std::stoll(*optional_target_id);
    if (target_id_ == 0)
    {
        response_ = http_response{ game_error_type::invalid_parameter, "管理员账号ID不合法" };
        return;
    }

    const auto optional_password = get_param(admin_password_describe);
    if (!optional_password || optional_password->empty())
    {
        response_ = http_response{ game_error_type::invalid_parameter, "新密码不能为空" };
        return;
    }

    const auto optional_token = get_param(admin_token_describe);
    if (!optional_token || optional_token->empty())
    {
        response_ = http_response{ game_error_type::invalid_parameter, "登录token不能为空" };
        return;
    }

    password_ = *optional_password;
    token_ = *optional_token;
}

void celeritas::admin_update_password_parameter::verify_sign()
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
                                                                  target_id_,
                                                                  password_,
                                                                  token_);
        hmac_sha256 != get_sign())
    {
        response_ = http_response{ game_error_type::sign_error };
    }
}
