// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#include "announcement_admin_delete_parameter.h"
#include "admin/admin_constant.h"
#include "auth/config/app_secret.h"
#include "common/core/hmac_sha_256.h"
#include "message/basic/game_error_type.h"

celeritas::announcement_admin_delete_parameter::announcement_admin_delete_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, id_{ 0 }, token_{}
{
    init();
}

bool celeritas::announcement_admin_delete_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::http_response celeritas::announcement_admin_delete_parameter::get_response() const
{
    return response_.value();
}

int64_t celeritas::announcement_admin_delete_parameter::get_id() const noexcept
{
    return id_;
}

const std::string& celeritas::announcement_admin_delete_parameter::get_token() const noexcept
{
    return token_;
}

void celeritas::announcement_admin_delete_parameter::init_required_parameter()
{
    const auto optional_id = get_param(announcement_id_describe);
    if (!optional_id || optional_id->empty())
    {
        response_ = http_response{ game_error_type::invalid_parameter, "公告ID不能为空" };
        return;
    }

    const auto optional_token = get_param(admin_token_describe);
    if (!optional_token || optional_token->empty())
    {
        response_ = http_response{ game_error_type::invalid_parameter, "登录token不能为空" };
        return;
    }

    id_ = std::stoll(*optional_id);
    token_ = *optional_token;
}

void celeritas::announcement_admin_delete_parameter::verify_sign()
{
    if (const auto http_response = get_http_parameter())
    {
        response_ = *http_response;
        return;
    }

    const auto secret = app_secret::get_instance().get_key(get_app_id());

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(secret, get_app_id(), get_timestamp(), id_, token_);
        hmac_sha256 != get_sign())
    {
        response_ = http_response{ game_error_type::sign_error };
    }
}
