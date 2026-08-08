#pragma once

#include <string_view>

namespace celeritas
{
    constexpr std::string_view guest_login_path = "/api/v1/login/guest";
    constexpr std::string_view send_sms_path = "/api/v1/login/send_sms";
    constexpr std::string_view send_email_path = "/api/v1/login/send_email";
    constexpr std::string_view phone_login_path = "/api/v1/login/phone";
    constexpr std::string_view email_login_path = "/api/v1/login/email";
    constexpr std::string_view password_login_path = "/api/v1/login/password";
    constexpr std::string_view sdk_login_path = "/api/v1/login/sdk";
    constexpr std::string_view phone_bind_path = "/api/v1/login/bind/phone";
    constexpr std::string_view email_bind_path = "/api/v1/login/bind/email";
    constexpr std::string_view password_bind_path = "/api/v1/login/bind/password";
    constexpr std::string_view sdk_bind_path = "/api/v1/login/bind/sdk";
    constexpr std::string_view login_servers_path = "/api/v1/login/servers";

    constexpr std::string_view order_create_path = "/api/v1/order/create";
    constexpr std::string_view notify_path = "/api/v1/notify/";

    constexpr std::string_view refund_path = "/refund/";

    constexpr std::string_view admin_login_path = "/admin/login";
    constexpr std::string_view admin_list_path = "/admin/list";
    constexpr std::string_view admin_create_path = "/admin/create";
    constexpr std::string_view admin_update_password_path = "/admin/update_password";
    constexpr std::string_view admin_update_status_path = "/admin/update_status";
    constexpr std::string_view admin_delete_path = "/admin/delete";

    constexpr std::string_view announcement_admin_list_path = "/v1/announcements/admin";
    constexpr std::string_view announcement_admin_create_path = "/v1/announcements/admin/create";
    constexpr std::string_view announcement_admin_update_path = "/v1/announcements/admin/update";
    constexpr std::string_view announcement_admin_delete_path = "/v1/announcements/admin/delete";
    constexpr std::string_view announcement_admin_broadcast_path = "/v1/announcements/admin/broadcast";
    constexpr std::string_view announcement_client_path = "/api/v1/announcements";
    constexpr std::string_view announcement_red_dot_path = "/api/v1/announcements/red_dot";
}