// 创建时间：2026-08-06
// 修改时间：2026-08-06
// 审核时间：2026-08-06

#include "announcement_admin_update_parameter.h"
#include "admin/admin_constant.h"
#include "auth/config/app_secret.h"
#include "common/core/enum_cast.h"
#include "common/core/hmac_sha_256.h"
#include "message/basic/game_error_type.h"

celeritas::announcement_admin_update_parameter::announcement_admin_update_parameter(const http_handle_parameter_shared_ptr& http_handle_parameter)
    : base_type{ http_handle_parameter }, response_{}, id_{ 0 }, title_{}, tag_{}, banner_url_{}, content_{}, priority_{}, publish_time_{}, status_{}, server_{}, token_{}
{
    init();
}

bool celeritas::announcement_admin_update_parameter::is_failure() const
{
    return response_.has_value();
}

celeritas::http_response celeritas::announcement_admin_update_parameter::get_response() const
{
    return response_.value();
}

int64_t celeritas::announcement_admin_update_parameter::get_id() const noexcept
{
    return id_;
}

const celeritas::announcement_admin_update_parameter::optional_string& celeritas::announcement_admin_update_parameter::get_title() const noexcept
{
    return title_;
}

const celeritas::announcement_admin_update_parameter::optional_int32& celeritas::announcement_admin_update_parameter::get_tag() const noexcept
{
    return tag_;
}

const celeritas::announcement_admin_update_parameter::optional_string& celeritas::announcement_admin_update_parameter::get_banner_url() const noexcept
{
    return banner_url_;
}

const celeritas::announcement_admin_update_parameter::optional_string& celeritas::announcement_admin_update_parameter::get_content() const noexcept
{
    return content_;
}

const celeritas::announcement_admin_update_parameter::optional_int32& celeritas::announcement_admin_update_parameter::get_priority() const noexcept
{
    return priority_;
}

const celeritas::announcement_admin_update_parameter::optional_int64& celeritas::announcement_admin_update_parameter::get_publish_time() const noexcept
{
    return publish_time_;
}

const celeritas::announcement_admin_update_parameter::optional_announcement_status& celeritas::announcement_admin_update_parameter::get_status() const noexcept
{
    return status_;
}

const celeritas::announcement_admin_update_parameter::optional_string& celeritas::announcement_admin_update_parameter::get_server() const noexcept
{
    return server_;
}

const std::string& celeritas::announcement_admin_update_parameter::get_token() const noexcept
{
    return token_;
}

void celeritas::announcement_admin_update_parameter::init_required_parameter()
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

void celeritas::announcement_admin_update_parameter::init_optional_parameter()
{
    if (const auto optional_title = get_param(announcement_title_describe))
    {
        title_ = *optional_title;
    }

    if (const auto optional_tag = get_param(announcement_tag_describe);
        optional_tag && !optional_tag->empty())
    {
        tag_ = std::stoi(*optional_tag);
    }

    if (const auto optional_banner = get_param(announcement_banner_url_describe))
    {
        banner_url_ = *optional_banner;
    }

    if (const auto optional_content = get_param(announcement_content_describe))
    {
        content_ = *optional_content;
    }

    if (const auto optional_priority = get_param(announcement_priority_describe);
        optional_priority && !optional_priority->empty())
    {
        priority_ = std::stoi(*optional_priority);
    }

    if (const auto optional_publish_time = get_param(announcement_publish_time_describe);
        optional_publish_time && !optional_publish_time->empty())
    {
        publish_time_ = std::stoll(*optional_publish_time);
    }

    if (const auto optional_status = get_param(announcement_status_describe))
    {
        status_ = underlying_cast_enum<announcement_status>(std::stoi(*optional_status));
    }

    if (const auto optional_server = get_param(announcement_server_describe))
    {
        server_ = *optional_server;
    }
}

void celeritas::announcement_admin_update_parameter::verify_sign()
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
