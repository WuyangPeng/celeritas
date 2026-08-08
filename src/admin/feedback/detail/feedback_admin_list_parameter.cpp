// 创建时间：2026-08-05
// 修改时间：2026-08-05
// 审核时间：2026-08-05

#include "feedback_admin_list_parameter.h"
#include "admin/admin_constant.h"
#include "auth/config/app_secret.h"
#include "common/core/hmac_sha_256.h"
#include "message/basic/game_error_type.h"

celeritas::feedback_admin_list_parameter::feedback_admin_list_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, page_{ 0 }, page_size_{ 0 }, token_{}
{
    init();
}

bool celeritas::feedback_admin_list_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::http_response celeritas::feedback_admin_list_parameter::get_response() const
{
    return response_.value();
}

int celeritas::feedback_admin_list_parameter::get_page() const noexcept
{
    return page_;
}

int celeritas::feedback_admin_list_parameter::get_page_size() const noexcept
{
    return page_size_;
}

const std::string& celeritas::feedback_admin_list_parameter::get_token() const noexcept
{
    return token_;
}

void celeritas::feedback_admin_list_parameter::init_required_parameter()
{
    const auto optional_page = get_param(admin_page_describe);
    if (!optional_page || optional_page->empty())
    {
        response_ = http_response{ game_error_type::invalid_parameter, "页码不能为空" };
        return;
    }

    page_ = std::stoi(*optional_page);
    if (page_ < 1)
    {
        response_ = http_response{ game_error_type::invalid_parameter, "页码不合法" };
        return;
    }

    const auto optional_page_size = get_param(admin_page_size_describe);
    if (!optional_page_size || optional_page_size->empty())
    {
        response_ = http_response{ game_error_type::invalid_parameter, "分页大小不能为空" };
        return;
    }

    page_size_ = std::stoi(*optional_page_size);
    if (page_size_ < 1)
    {
        response_ = http_response{ game_error_type::invalid_parameter, "分页大小不合法" };
        return;
    }

    const auto optional_token = get_param(admin_token_describe);
    if (!optional_token || optional_token->empty())
    {
        response_ = http_response{ game_error_type::invalid_parameter, "登录token不能为空" };
        return;
    }
    token_ = *optional_token;
}

void celeritas::feedback_admin_list_parameter::verify_sign()
{
    if (const auto http_response = get_http_parameter())
    {
        response_ = *http_response;
        return;
    }

    const auto secret = app_secret::get_instance().get_key(get_app_id());

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(secret, get_app_id(), get_timestamp(), page_, page_size_, token_);
        hmac_sha256 != get_sign())
    {
        response_ = http_response{ game_error_type::sign_error };
    }
}
