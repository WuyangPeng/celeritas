// 创建时间：2026-07-25
// 修改时间：2026-07-25
// 审核时间：2026-07-25

#pragma once

namespace celeritas
{
    enum class admin_role_type
    {
        super_admin = 0, // 超级管理员
        operation_admin = 1, // 运营管理员
        devops_admin = 2, // 运维管理员
        developer_admin = 3, // 程序管理员
        readonly_user = 4, // 只读用户
    };
}
