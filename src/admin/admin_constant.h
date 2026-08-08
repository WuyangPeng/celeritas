// 创建时间：2026-07-22
// 修改时间：2026-07-31
// 审核时间：2026-08-07

#pragma once

#include <string_view>

namespace celeritas
{
    // 账号 API 参数键
    constexpr std::string_view admin_account_id_describe = "account_id";
    constexpr std::string_view admin_username_describe = "username";
    constexpr std::string_view admin_password_describe = "password";
    constexpr std::string_view admin_role_describe = "role";
    constexpr std::string_view admin_nickname_describe = "nickname";
    constexpr std::string_view admin_avatar_describe = "avatar";
    constexpr std::string_view admin_status_describe = "status";
    constexpr std::string_view admin_app_id_describe = "app_id";
    constexpr std::string_view default_super_admin_username = "admin";
    constexpr std::string_view admin_token_describe = "token";
    constexpr auto admin_session_ttl_seconds = 86400; // 24 小时（Redis EXPIRE 单位为秒）

    // 公告 API 参数键
    constexpr std::string_view announcement_id_describe = "id";
    constexpr std::string_view announcement_app_id_describe = "app_id";
    constexpr std::string_view announcement_title_describe = "title";
    constexpr std::string_view announcement_tag_describe = "tag";
    constexpr std::string_view announcement_banner_url_describe = "banner_url";
    constexpr std::string_view announcement_content_describe = "content";
    constexpr std::string_view announcement_priority_describe = "priority";
    constexpr std::string_view announcement_publish_time_describe = "publish_time";
    constexpr std::string_view announcement_status_describe = "status";
    constexpr std::string_view announcement_server_describe = "server";

    // 通用查询参数键
    constexpr std::string_view admin_page_describe = "page";
    constexpr std::string_view admin_page_size_describe = "page_size";
    constexpr std::string_view admin_keyword_describe = "keyword";
}
