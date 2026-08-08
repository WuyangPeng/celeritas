// 创建时间：2026-07-25
// 修改时间：2026-07-31
// 审核时间：2026-07-31

#include "admin_create_parameter.h"
#include "admin/admin_constant.h"
#include "admin/account/admin_role_type.h"
#include "auth/config/app_secret.h"
#include "common/core/enum_cast.h"
#include "common/core/hmac_sha_256.h"
#include "message/basic/game_error_type.h"

celeritas::admin_create_parameter::admin_create_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, username_{}, password_{}, role_{}, nickname_{}, avatar_{}, token_{}
{
    init();
}

bool celeritas::admin_create_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::http_response celeritas::admin_create_parameter::get_response() const
{
    return response_.value();
}

const std::string& celeritas::admin_create_parameter::get_username() const noexcept
{
    return username_;
}

const std::string& celeritas::admin_create_parameter::get_password() const noexcept
{
    return password_;
}

celeritas::admin_role_type celeritas::admin_create_parameter::get_role() const noexcept
{
    return role_;
}

const std::string& celeritas::admin_create_parameter::get_nickname() const noexcept
{
    return nickname_;
}

const std::string& celeritas::admin_create_parameter::get_avatar() const noexcept
{
    return avatar_;
}

const std::string& celeritas::admin_create_parameter::get_token() const noexcept
{
    return token_;
}

void celeritas::admin_create_parameter::init_required_parameter()
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

    const auto optional_role = get_param(admin_role_describe);
    if (!optional_role)
    {
        response_ = http_response{ game_error_type::invalid_parameter, "角色类型不能为空" };
        return;
    }

    const auto optional_token = get_param(admin_token_describe);
    if (!optional_token || optional_token->empty())
    {
        response_ = http_response{ game_error_type::invalid_parameter, "登录token不能为空" };
        return;
    }

    username_ = *optional_username;
    password_ = *optional_password;
    role_ = underlying_cast_enum<admin_role_type>(std::stoi(*optional_role));
    token_ = *optional_token;
}

void celeritas::admin_create_parameter::init_optional_parameter()
{
    if (const auto optional_nickname = get_param(admin_nickname_describe))
    {
        nickname_ = *optional_nickname;
    }

    if (const auto optional_avatar = get_param(admin_avatar_describe))
    {
        avatar_ = *optional_avatar;
    }
}

void celeritas::admin_create_parameter::verify_sign()
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
                                                                  password_,
                                                                  enum_cast_underlying(role_),
                                                                  nickname_,
                                                                  avatar_,
                                                                  token_);
        hmac_sha256 != get_sign())
    {
        response_ = http_response{ game_error_type::sign_error };
    }
}
