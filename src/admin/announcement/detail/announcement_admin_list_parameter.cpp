// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#include "announcement_admin_list_parameter.h"
#include "admin/admin_constant.h"
#include "auth/config/app_secret.h"
#include "common/core/enum_cast.h"
#include "common/core/hmac_sha_256.h"
#include "message/basic/game_error_type.h"

celeritas::announcement_admin_list_parameter::announcement_admin_list_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, page_{ 0 }, page_size_{ 0 }, keyword_{}, tag_{}, status_{}, token_{}
{
    init();
}

bool celeritas::announcement_admin_list_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::http_response celeritas::announcement_admin_list_parameter::get_response() const
{
    return response_.value();
}

int celeritas::announcement_admin_list_parameter::get_page() const noexcept
{
    return page_;
}

int celeritas::announcement_admin_list_parameter::get_page_size() const noexcept
{
    return page_size_;
}

const std::string& celeritas::announcement_admin_list_parameter::get_keyword() const noexcept
{
    return keyword_;
}

const celeritas::announcement_admin_list_parameter::optional_int32& celeritas::announcement_admin_list_parameter::get_tag() const noexcept
{
    return tag_;
}

const celeritas::announcement_admin_list_parameter::optional_announcement_status& celeritas::announcement_admin_list_parameter::get_status() const noexcept
{
    return status_;
}

const std::string& celeritas::announcement_admin_list_parameter::get_token() const noexcept
{
    return token_;
}

void celeritas::announcement_admin_list_parameter::init_required_parameter()
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

void celeritas::announcement_admin_list_parameter::init_optional_parameter()
{
    if (const auto optional_keyword = get_param(admin_keyword_describe))
    {
        keyword_ = *optional_keyword;
    }

    if (const auto optional_tag = get_param(announcement_tag_describe);
        optional_tag && !optional_tag->empty())
    {
        tag_ = std::stoi(*optional_tag);
    }

    if (const auto optional_status = get_param(announcement_status_describe);
        optional_status && !optional_status->empty())
    {
        status_ = underlying_cast_enum<announcement_status>(std::stoi(*optional_status));
    }
}

void celeritas::announcement_admin_list_parameter::verify_sign()
{
    if (const auto http_response = get_http_parameter())
    {
        response_ = *http_response;
        return;
    }

    const auto secret = app_secret::get_instance().get_key(get_app_id());

    if (const auto hmac_sha256 = hmac_sha256::calculate_with_args(secret, get_app_id(), get_timestamp(), page_, page_size_, keyword_, token_);
        hmac_sha256 != get_sign())
    {
        response_ = http_response{ game_error_type::sign_error };
    }
}
