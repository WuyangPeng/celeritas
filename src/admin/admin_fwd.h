// 创建时间：2026-07-22
// 修改时间：2026-08-05
// 审核时间：2026-08-05

#pragma once

namespace celeritas
{
    class admin_service_base;

    // 账号管理
    enum class admin_role_type;
    enum class admin_status_type;

    class admin_create;
    class admin_delete;
    class admin_list;
    class admin_login;
    class admin_update_password;
    class admin_update_status;

    class admin_create_response;
    class admin_delete_response;
    class admin_list_response;
    class admin_list_item;
    class admin_list_result;
    class admin_login_response;
    class admin_update_password_response;
    class admin_update_status_response;

    // 公告管理
    enum class announcement_status;

    class announcement_admin_list;
    class announcement_admin_create;
    class announcement_admin_update;
    class announcement_admin_delete;
    class announcement_admin_broadcast;

    class announcement_admin_list_item;
    class announcement_admin_list_result;
    class announcement_admin_list_response;
    class announcement_admin_create_response;
    class announcement_admin_update_response;
    class announcement_admin_delete_response;
    class announcement_admin_broadcast_response;

    // 问题反馈管理

    enum class feedback_type;

    class feedback_admin_list;
    class feedback_admin_list_item;
    class feedback_admin_list_response;
    class feedback_admin_list_result;
}
